#!/bin/bash

cd /Users/binh/Desktop/ns3_play/result/delay-amrlc-ul
pwd
rm delay-amrlc-ul.svg
rm compare-um-am.svg
gnuplot plot.gnu.txt
gnuplot compare.gnu.txt
