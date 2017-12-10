#!/bin/bash


cd tests

chmod 775 *.sh

EXIT_CODE=0
for i in `ls test_*.sh`; do 
  bash $i
  filename=$(basename "$i")
  filename="${filename%.*}"
  python ../compare_outputs.py expected/${filename}.out outputs/${filename}.out
  if [ $? -eq 0 ]; then
    printf "TEST %-40s %-10s\n" "$i" "PASSED" 
  else
    printf "TEST %-40s %-10s\n" "$i" "FAILED" 
    EXIT_CODE=1
  fi  
done

exit $EXIT_CODE



