outfile = ARG0
node_num = ARG1

# ./linux_data_log/nodes_4__perr_0.log
base_name = "./linux_data_log/nodes_" . node_num. "__perr_"
suffix = ".log"

outfile = "./plot/latex_plot_" . node_num . ".tex"

print "base_name:" . base_name
print "outfile:" . outfile 

set terminal epslatex color colortext font "Times, 16"
set output outfile

set style data histograms
set style fill solid 1 border 0
set boxwidth 5
set grid ytics

set xlabel "percentage error"
set ylabel "time (seconds)"
set grid
set size 1,1
set xrange [0:*]   # x starts at 0, max is auto


set termoption noenhanced
unset title

array mean_time[10]

do for [i=1:9] {
  filename = sprintf("%s%d%s", base_name, i*10, suffix)
  stats filename using 1 nooutput
  mean_time[i] = STATS_mean
}

do for [j=1:9]{
  filename = sprintf("mean %d: %d", j, mean_time[j])
  print filename
}

plot for [i=1:9] \
      error_rate = i*10 \
     '+' using (error_rate):(mean_time[i]) \
     with boxes notitle
     # with boxes title sprintf("err: %d\\%",error_rate)

# plot for [i=1:10] mean_time[i] notitle

print "writing to: " . outfile
set output
