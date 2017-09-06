#!/bin/bash

./run_tests.sh > run_tests.out
if ! grep -q "FAILED" run_tests.out ; then
  exit 0
else
  exit 1
fi
