#!/bin/bash

OBIN=$1
DIR_SIM_FILE=$2

RANGE_NODE_NUM=$(seq 1 5)
RANGE_PROB_ERR=$(seq 0 9)
RANGE_NUM_REPETITION=$(seq 0 32)

mkdir -p ${DIR_SIM_FILE}

run_simulation() {
  echo "executing simulation for node: ${1}, err ${2}"
  rm -f /tmp/nearest_ap_*.bus
  ./${OBIN} ${node} ${err_p} $log_file;
  sleep 1;
}

for node_mul in $RANGE_NODE_NUM; do
  node=$((node_mul * 4))
  for prob in $RANGE_PROB_ERR; do
    err_p=$((prob * 10));
    log_file=${DIR_SIM_FILE}/nodes_${node}__perr_${err_p}.log

    if [[ -e ${log_file} ]]; then
      num_simulations=$(cat $log_file | wc -l);
      if [[ $num_simulations -lt 33 ]]; then
        rm -f ${log_file}
        for rep in $RANGE_NUM_REPETITION; do
          echo "info simulation: nodes: ${node}  err: ${err_p}, rep: ${rep}";
          run_simulation  $node $err_p
        done;
      fi
    else
      for rep in $RANGE_NUM_REPETITION; do
        echo "info simulation: nodes: ${node}  err: ${err_p}, rep: ${rep}";
        run_simulation  $node $err_p
      done;
    fi
  done;
done; 
