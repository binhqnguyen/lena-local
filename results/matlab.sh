#!/bin/bash

cd /Users/binh/Desktop/ns3_play/
pwd
cat < tcp-put-ul-send.txt | grep 7.0.0.2 > /Users/binh/Documents/shared/tcp_ul_1.txt
cat < tcp-put-ul-send.txt | grep 7.0.0.3 > /Users/binh/Documents/shared/tcp_ul_2.txt
cat < tcp-put-ul-send.txt | grep 7.0.0.4 > /Users/binh/Documents/shared/tcp_ul_3.txt
echo "throughput files copied to ~/Documents/shared/" 
