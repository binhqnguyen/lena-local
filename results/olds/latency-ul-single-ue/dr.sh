#!/bin/bash

cd /Users/binh/Desktop/ns3_play/result/latency-ul-single-ue
pwd
rm latency-ul-single-ue.svg
gnuplot plot.gnu.txt
