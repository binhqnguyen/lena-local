#!/bin/bash

cd /Users/binh/Documents/workspace/lena/results/tcp-link/pedestrian/const_vel/cumulative_avg
pwd

cd graphs
rm *.svg

cd ..

cd data-scripts
pwd
cat < tcp-700m | grep ID1 | grep -v 7.0.0.2 > tcp-700m-queue.txt
cat < tcp-700m | grep "RLC_buffer_discarded" > tcp_buffer_discarded.txt
cat < tcp-700m | grep "Tcp lost=" > tcp_lost.txt
cat < udp-700m | grep ID1 | grep -v 7.0.0.2 > udp-700m-queue.txt
#cat < tcp-100-3-ul-700m-nofading | grep ID1 > tcp-100-3-ul-700m-buffer-nofading.txt
#cat < udp-100-3-ul-700m-nofading | grep ID1 > udp-100-3-ul-700m-buffer-nofading.txt
cat < tcp-700m | grep 7.0.0.2 | grep -v ID1 > tcp-700m-put.txt
cat < udp-700m | grep 7.0.0.2 | grep -v ID1 > udp-700m-put.txt

cat < tcp-700m-dl | grep ID1 | grep -v 7.0.0.2 > tcp-700m-queue-dl.txt
cat < tcp-700m-dl | grep "RLC_buffer_discarded" > tcp_buffer_discarded_dl.txt
cat < tcp-700m-dl | grep "Tcp lost=" > tcp_lost_dl.txt
cat < udp-700m-dl | grep ID1 | grep -v 7.0.0.2 > udp-700m-queue-dl.txt
#cat < tcp-100-3-ul-700m-nofading | grep ID1 > tcp-100-3-ul-700m-buffer-nofading.txt
#cat < udp-100-3-ul-700m-nofading | grep ID1 > udp-100-3-ul-700m-buffer-nofading.txt
cat < tcp-700m-dl | grep 7.0.0.2 | grep -v ID1 | grep -v enb > tcp-700m-put-dl.txt
cat < udp-700m-dl | grep 7.0.0.2 | grep -v ID1 | grep -v enb > udp-700m-put-dl.txt

./avg_queue_delay.py
#./avg_tcp_delay.py

#gnuplot plot-buffer.txt
#gnuplot plot-aggregate.txt
#gnuplot plot-small-interval.txt
gnuplot plot-averaged.txt
gnuplot plot-averaged-dl.txt

#gnuplot plot-ul-200m.gnu.txt
#gnuplot plot-ul-300m.gnu.txt
#gnuplot plot-ul-400m.gnu.txt
#gnuplot plot-ul-500m.gnu.txt
#gnuplot plot-ul-600m.gnu.txt
#gnuplot plot-ul-700m.gnu.txt
#gnuplot plot-ul-800m.gnu.txt
#gnuplot plot-ul-900m.gnu.txt
#gnuplot plot-ul-1000m.gnu.txt
#gnuplot plot-ul-1100m.gnu.txt
#gnuplot plot-ul-1200m.gnu.txt
#gnuplot plot-ul-1300m.gnu.txt

#gnuplot plot-3kmph.gnu.txt
#gnuplot plot-2ue.gnu.txt

cp *.svg ../graphs
rm *.svg

