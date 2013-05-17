#!/bin/bash

cd /Users/binh/Documents/workspace/lena/results/tcp-link/pedestrian/const_vel/cumulative_avg
pwd

cd graphs
rm *.svg

cd ..

cd data-scripts
pwd

cat < TCP-700m | grep ID1 | grep -v 7.0.0.2 > tcp-700m-queue.txt
cat < TCP-700m | grep "RLC_buffer_discarded" > tcp_buffer_discarded_700m.txt
cat < TCP-700m | grep "Tcp lost=" > tcp_lost_700m.txt
cat < UDP-700m | grep ID1 | grep -v 7.0.0.2 > udp-700m-queue.txt
#cat < tcp-100-3-ul-700m-nofading | grep ID1 > tcp-100-3-ul-700m-buffer-nofading.txt
#cat < udp-100-3-ul-700m-nofading | grep ID1 > udp-100-3-ul-700m-buffer-nofading.txt
cat < TCP-700m | grep 7.0.0.2 | grep -v ID1 > tcp-700m-put.txt
cat < UDP-700m | grep 7.0.0.2 | grep -v ID1 > udp-700m-put.txt

cat < TCP-700m-dl | grep ID1 | grep -v 7.0.0.2 > tcp-700m-queue-dl.txt
cat < TCP-700m-dl | grep "RLC_buffer_discarded" > tcp_buffer_discarded_dl_700m.txt
cat < TCP-700m-dl | grep "Tcp lost=" > tcp_lost_dl_700m.txt
cat < UDP-700m-dl | grep ID1 | grep -v 7.0.0.2 > udp-700m-queue-dl.txt
#cat < tcp-100-3-ul-700m-nofading | grep ID1 > tcp-100-3-ul-700m-buffer-nofading.txt
#cat < udp-100-3-ul-700m-nofading | grep ID1 > udp-100-3-ul-700m-buffer-nofading.txt
cat < TCP-700m-dl | grep 7.0.0.2 | grep -v ID1 | grep -v enb > tcp-700m-put-dl.txt
cat < UDP-700m-dl | grep 7.0.0.2 | grep -v ID1 | grep -v enb > udp-700m-put-dl.txt


cat < TCP-200m | grep ID1 | grep -v 7.0.0.2 > tcp-200m-queue.txt
cat < TCP-200m | grep "RLC_buffer_discarded" > tcp_buffer_discarded_200m.txt
cat < TCP-200m | grep "Tcp lost=" > tcp_lost_200m.txt
cat < UDP-200m | grep ID1 | grep -v 7.0.0.2 > udp-200m-queue.txt
cat < TCP-200m | grep 7.0.0.2 | grep -v ID1 > tcp-200m-put.txt
cat < UDP-200m | grep 7.0.0.2 | grep -v ID1 > udp-200m-put.txt

cat < TCP-200m-dl | grep ID1 | grep -v 7.0.0.2 > tcp-200m-queue-dl.txt
cat < TCP-200m-dl | grep "RLC_buffer_discarded" > tcp_buffer_discarded_dl_200m.txt
cat < TCP-200m-dl | grep "Tcp lost=" > tcp_lost_dl_200m.txt
cat < UDP-200m-dl | grep ID1 | grep -v 7.0.0.2 > udp-200m-queue-dl.txt
cat < TCP-200m-dl | grep 7.0.0.2 | grep -v ID1 | grep -v enb > tcp-200m-put-dl.txt
cat < UDP-200m-dl | grep 7.0.0.2 | grep -v ID1 | grep -v enb > udp-200m-put-dl.txt



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

