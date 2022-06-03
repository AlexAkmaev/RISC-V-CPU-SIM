# !/bin/bash

if [[ -z ${SIM_BUILD} || -z ${SIM_ROOT} ]]; then
  exit 1
fi

cd ${SIM_BUILD}

run_test () {
  cd ${SIM_BUILD}
  ./tests/base_instructions_tests
  ./tests/blocks_tests
  ./tests/hazard_unit_tests

  ./cpu ${SIM_ROOT}/tests/data/loop1.dat
  ./cpu ${SIM_ROOT}/tests/data/loop2.dat
  ./cpu ${SIM_ROOT}/tests/data/loop3.dat
  ./cpu ${SIM_ROOT}/tests/data/loop4.dat
  ./cpu ${SIM_ROOT}/tests/data/loop10k.dat
}


for i in {1..5}
do
  run_test
done
