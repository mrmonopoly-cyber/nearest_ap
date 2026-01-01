#!/bin/sh

node_base=4

latex_base=latex_plot

for nodes_it in $(seq 1 5); do
  nodes=$((nodes_it * node_base))
  echo "==========="
  echo "nodes: ${nodes}"
  echo "gnuplot -c ./plot.gp ${nodes}"
  gnuplot -c ./plot.gp ${nodes}
done
