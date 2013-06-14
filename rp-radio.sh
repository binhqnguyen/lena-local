#!/bin/bash


echo "running radio UDP ..."
./waf --run "e2e-fading-ul-mobile --simTime=100 --dataRate=100000kb/s --isPedestrian=1 --isTcp=0 --s1uLinkDelay=0.001 --p2pLinkDelay=0.001 --traceFile=/Users/binh/Documents/shared/fading_trace_EPA_3kmph-100-3.fad --tag=700m --traceTime=100 --moving_speed=3 --distance=700" > results/tcp/data-scripts/radio/UDP 2>&1

echo "running radio TCP ..."
./waf --run "e2e-fading-ul-mobile --simTime=100 --dataRate=100000kb/s --isPedestrian=1 --isTcp=1 --s1uLinkDelay=0.001 --p2pLinkDelay=0.001 --traceFile=/Users/binh/Documents/shared/fading_trace_EPA_3kmph-100-3.fad --tag=700m --traceTime=100 --moving_speed=3 --distance=700" > results/tcp/data-scripts/radio/TCP 2>&1

echo "plotting ..."
cd ~/Documents/workspace/lena/results/tcp/
./dr-radio.sh


