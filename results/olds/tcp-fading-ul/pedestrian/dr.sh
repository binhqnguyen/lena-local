#!/bin/bash

cd /Users/binh/Desktop/ns3_play/result/tcp-fading-ul/pedestrian
pwd
cat < tcp-put-ul-send-3-20-1.txt | grep 7.0.0.2 > tcp_ul_1.txt
cat < tcp-put-ul-send-3-20-1.txt | grep 7.0.0.3 > tcp_ul_2.txt
cat < tcp-put-ul-send-3-20-1.txt | grep 7.0.0.4 > tcp_ul_3.txt
rm tcp-put-pedestrian.svg
rm tcp-put-pedestrian-3.svg
rm link-cap-3.svg
rm tcp-delay-3.svg
rm tcp-put-pedestrian-3-1.svg
rm tcp-put-pedestrian-3-2.svg
rm tcp-put-pedestrian-3-3.svg
gnuplot plot.gnu.txt
 rm tcp_ul_1.txt
 rm tcp_ul_2.txt
 rm tcp_ul_3.txt