#!/bin/bash

cd /Users/binh/Desktop/ns3_play/result/udp-fading-ul/pedestrian-u
pwd
cat < link_cap_ul_3.txt | grep UE1 > c_ul_1.txt
cat < link_cap_ul_3.txt | grep UE2 > c_ul_2.txt
cat < link_cap_ul_3.txt | grep UE3 > c_ul_3.txt
cat < tcp-put-ul-send_3.txt | grep 7.0.0.2 > tcp_ul_1.txt
cat < tcp-put-ul-send_3.txt | grep 7.0.0.3 > tcp_ul_2.txt
cat < tcp-put-ul-send_3.txt | grep 7.0.0.4 > tcp_ul_3.txt
rm udp-put-pedestrian.svg
rm udp-cap.svg
rm udp-put-pedestrian-3.svg
gnuplot plot.gnu.txt
 rm c_ul_1.txt
 rm c_ul_2.txt
 rm c_ul_3.txt
 rm tcp_ul_1.txt
 rm tcp_ul_2.txt
 rm tcp_ul_3.txt