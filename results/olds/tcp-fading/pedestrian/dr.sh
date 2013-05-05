#!/bin/bash

cd /Users/binh/Desktop/ns3_play/result/tcp-fading/pedestrian
pwd
cat < link_cap_dl_3.txt | grep UE1 > c_dl_1.txt
cat < link_cap_dl_3.txt | grep UE2 > c_dl_2.txt
cat < link_cap_dl_3.txt | grep UE3 > c_dl_3.txt
cat < tcp-put-dl-send_3.txt | grep 7.0.0.2 > tcp_dl_1.txt
cat < tcp-put-dl-send_3.txt | grep 7.0.0.3 > tcp_dl_2.txt
cat < tcp-put-dl-send_3.txt | grep 7.0.0.4 > tcp_dl_3.txt
rm tcp-put-pedestrian.svg
rm tcp-cap.svg
gnuplot plot.gnu.txt
 rm c_dl_1.txt
 rm c_dl_2.txt
 rm c_dl_3.txt
 rm tcp_dl_1.txt
 rm tcp_dl_2.txt
 rm tcp_dl_3.txt