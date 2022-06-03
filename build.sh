# !/bin/bash

if [[ -z ${SIM_BUILD} || -z ${SIM_ROOT} ]]; then
  exit 1
fi

cmake -S ${SIM_ROOT} -B ${SIM_BUILD}
cd ${SIM_BUILD}
make -j4
