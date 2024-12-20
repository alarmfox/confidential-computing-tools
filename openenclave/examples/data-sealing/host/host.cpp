// Copyright (c) Open Enclave SDK contributors.
// Licensed under the MIT License.

#include <assert.h>
#include <common/shared.h>
#include <limits.h>
#include <openenclave/host.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <iostream>
#include <fstream>
#include <vector>
#include "datasealing_u.h"

using namespace std;

#define GET_POLICY_NAME(policy) \
    ((policy == POLICY_UNIQUE) ? "POLICY_UNIQUE" : "POLICY_PRODUCT")

const char* g_plain_text = "test plaintext";
const char* g_opt_msg = "optional sealing message";

oe_enclave_t* create_enclave(const char* enclavePath)
{
    oe_enclave_t* enclave = NULL;

    printf("Host: Loading enclave library %s\n", enclavePath);
    oe_result_t result = oe_create_datasealing_enclave(
        enclavePath,
        OE_ENCLAVE_TYPE_SGX,
        OE_ENCLAVE_FLAG_DEBUG,
        NULL,
        0,
        &enclave);

    if (result != OE_OK)
    {
        printf(
            "Host: oe_create_datasealing_enclave failed. %s",
            oe_result_str(result));
    }
    else
    {
        printf("Host: Enclave created.\n");
    }
    return enclave;
}

void terminate_enclave(oe_enclave_t* enclave)
{
    oe_terminate_enclave(enclave);
    printf("Host: enclave terminated.\n");
}


int main(int argc, const char* argv[])
{
    unsigned char* data = NULL;
    oe_result_t result = OE_OK;
    oe_enclave_t* enclave = NULL;
    size_t data_size = 0;
    data_t sealed_data, unsealed_data;
    int ret = 1;
    int policy = POLICY_UNIQUE;
    ofstream sealed_file;

    argc = 0;
    enclave = create_enclave(argv[1]);
    if (enclave == NULL)
    {
        goto exit;
    }
    cout << "Host: Seal data into enclave with " << GET_POLICY_NAME(policy)
         << endl;

    data = (unsigned char*)g_plain_text;
    data_size = strlen((const char*)data) + 1;

    ret = 0;
    result = seal_data(
        enclave,
        &ret,
        policy,
        (unsigned char*)g_opt_msg,
        strlen(g_opt_msg),
        data,
        data_size,
        &sealed_data);
    if ((result != OE_OK) || (ret != 0))
    {
        cout << "Host: seal_data failed with " << oe_result_str(result)
             << " ret = " << ret << endl;
        goto exit;
    }
    // save to file encrypted data
    cout << "Host: data successfully sealed" << endl;

    // Save sealed data to file
    sealed_file.open("sealed", ios::out | ios::binary);
    if (!sealed_file)
    {
        cout << "Host: Failed to open file for writing sealed data" << endl;
        goto exit;
    }
    sealed_file.write(reinterpret_cast<const char*>(sealed_data.data), sealed_data.size);
    sealed_file.close();
    cout << "Host: Sealed data saved to file 'sealed'" << endl;


    // read from file and decrypt
    result = unseal_data(
        enclave, &ret, &sealed_data, 1, &unsealed_data);
    if ((result != OE_OK) || (ret != 0))
    {
        cout << "Host: ecall unseal_data returned " << oe_result_str(result)
             << " ret = " << ret << (ret ? " (failed)" : " (success)") << endl;
        ret = ERROR_SIGNATURE_VERIFY_FAIL;
        goto exit;
    }

    // print unsealed data
    cout << "Host: Unsealed result:" << endl;
    printf("data=%s --- ", unsealed_data.data);

    printf("data_size=%zd\n", unsealed_data.size);

exit:
    cout << "Host: Terminating enclaves" << endl;
    if (enclave)
        terminate_enclave(enclave);

    if (ret == 0)
        cout << "Host: Sample completed successfully." << endl;

    return ret;
}
