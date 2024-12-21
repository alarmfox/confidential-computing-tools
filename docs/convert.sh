pandoc main.tex \
  -f latex \
  -t mediawiki \
  --metadata link-citations \
  --bibliography=refs.bib \
  --csl=ieee.csl  \
  --citeproc | python scripts/adjust.py | tee output.mediawiki | xclip -selection clipboard

