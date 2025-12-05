if [ "$#" -ne 1 ]; then
  echo "Illegal number of parameters"
  exit  
fi
rsync --progress -r -u Source/* ../code/Source
cd ../code/Source
git add --all
git commit -m "$1"
git push