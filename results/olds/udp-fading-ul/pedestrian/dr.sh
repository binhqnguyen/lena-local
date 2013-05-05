#!/bin/bash

cd /Users/binh/Desktop/ns3_play/result/udp-fading-ul/pedestrian
pwd
cat < tcp-put-ul-send-3-20-1.txt | grep 7.0.0.2 > tcp_ul_1.txt
cat < tcp-put-ul-send-3-20-1.txt | grep 7.0.0.3 > tcp_ul_2.txt
cat < tcp-put-ul-send-3-20-1.txt | grep 7.0.0.4 > tcp_ul_3.txt
rm udp-put-pedestrian.svg
rm udp-cap.svg
rm udp-put-pedestrian-3.svg
gnuplot plot.gnu.txt