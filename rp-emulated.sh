#!/bin/bash
echo "running emulated UDP ..."
./waf --run "scratch/emulated --sim_time=200 --is_tcp=0" > ~/Documents/workspace/lena/results/tcp/data-scripts/emulated/UDP 2>&1 

echo "running emulated TCP ..."
./waf --run "scratch/emulated --sim_time=200 --is_tcp=1" > ~/Documents/workspace/lena/results/tcp/data-scripts/emulated/TCP 2>&1 

cp ~/Documents/workspace/lena/results/tcp/data-scripts/emulated/UDP ~/Documents/workspace/lena/results/tcp/graphs/emulated 
cp ~/Documents/workspace/lena/results/tcp/data-scripts/emulated/TCP ~/Documents/workspace/lena/results/tcp/graphs/emulated 

echo "plotting ..."
cd ~/Documents/workspace/lena/results/tcp/
pwd
./dr-emulated.sh



