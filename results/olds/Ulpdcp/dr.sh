#!/bin/bash

cd /Users/binh/Desktop/ns3_play/result/ulpdcp
pwd
rm ul-pdcp-um.svg
rm ul-pdcp-am.svg
gnuplot plot.gnu.txt
gnuplot am.gnu.txt
