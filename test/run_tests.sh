#!/bin/bash

echo "Running tests..."

# INPUT OUTPUT TESTS
echo "Running IO tests..."
cd input_output

# Test 1
../bin-test/io-test 1sys2wat.nrg
diff output.nrg expected_outputs/1sys2wat_output.nrg
if [ $? -ne 0 ]; then
  echo "TEST ../bin-test/io-test 1sys2wat.nrg FAILED"
  exit
else
  echo "TEST ../bin-test/io-test 1sys2wat.nrg PASSED"
fi

# Test 2
../bin-test/io-test 2sys2wat.nrg
diff output.nrg expected_outputs/2sys2wat_output.nrg
if [ $? -ne 0 ]; then
  echo "TEST ../bin-test/io-test 2sys2wat.nrg FAILED"
  exit
else
  echo "TEST ../bin-test/io-test 2sys2wat.nrg PASSED"
fi

# Test 3
../bin-test/io-test 1sys1so4.nrg
diff output.nrg expected_outputs/1sys1so4_output.nrg
if [ $? -ne 0 ]; then
  echo "TEST ../bin-test/io-test 1sys1so4.nrg FAILED"
  exit
else
  echo "TEST ../bin-test/io-test 1sys1so4.nrg PASSED"
fi
rm output.nrg

echo "Running 1b Energy tests..."
cd ../energies
# Test 4
../bin-test/energy-test 1sys1so4.nrg > energy_grads.log
diff energy_grads.log expected_outputs/1sys1so4_output.log
if [ $? -ne 0 ]; then
  echo "TEST ../bin-test/energy-test 1sys1so4.nrg FAILED"
  exit
else
  echo "TEST ../bin-test/energy-test 1sys1so4.nrg PASSED"
fi
rm energy_grads.log
# Test 5
../bin-test/energy-test 1sys1h2o.nrg > energy_grads.log
diff energy_grads.log expected_outputs/1sys1h2o_output.log
if [ $? -ne 0 ]; then
  echo "TEST ../bin-test/energy-test 1sys1h2o.nrg FAILED"
  exit
else
  echo "TEST ../bin-test/energy-test 1sys1h2o.nrg PASSED"
fi
rm energy_grads.log
