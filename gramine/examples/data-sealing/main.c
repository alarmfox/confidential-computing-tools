#include <stdio.h>
#include <stdlib.h>

int write_to_file(const char* fname) {
  FILE* f;
  int ret = 0;
  f = fopen(fname, "w");

  if (!f) {
    ret = -1;
    goto exit;
  }
  fprintf(f, "%s - %s", fname, "hello world");

  fclose(f);
exit:
  return ret;
}

int main(void) {
  const char* files[3] = {"db1/a.txt", "db2/a.txt", "db3/a.txt"};
  int rc = 0;

  for (int i = 0; i < 3; i++) {
    rc = write_to_file(files[i]);
    if (rc < 0) {
      printf("%s ", files[i]);
      perror("error writing file");
    }
  }


  return 0;
}
