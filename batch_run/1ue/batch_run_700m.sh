cd /Users/binh/Documents/workspace/lena
sleep 10m
./waf --run "e2e-fading-ul-mobile --simTime=100 --dataRate=100000kb/s --isPedestrian=1 --isTcp=0 --s1uLinkDelay=0.001 --p2pLinkDelay=0.001 --traceFile=/Users/binh/Documents/shared/fading_trace_EPA_3kmph-100-3.fad --tag=700m --traceTime=100 --moving_speed=3 --distance=700" > out/udp-700m 2>&1
./waf --run "e2e-fading-ul-mobile --simTime=100 --dataRate=100000kb/s --isPedestrian=1 --isTcp=1 --s1uLinkDelay=0.001 --p2pLinkDelay=0.001 --traceFile=/Users/binh/Documents/shared/fading_trace_EPA_3kmph-100-3.fad --tag=700m --traceTime=100 --moving_speed=3 --distance=700" > out/tcp-700m 2>&1



