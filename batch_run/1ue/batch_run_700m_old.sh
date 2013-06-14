cd /Users/binh/Documents/workspace/lena


./waf --run "e2e-re --simTime=100 --dataRate=100000kb/s --isPedestrian=1 --isTcp=0 --s1uLinkDelay=0.001 --p2pLinkDelay=0.001 --traceFile=/Users/binh/Documents/shared/fading_trace_EPA_3kmph-100-3.fad --tag=700m --traceTime=100 --moving_speed=3 --distance=1000" > out/udp-1000m 2>&1


./waf --run "e2e-re --simTime=100 --dataRate=100000kb/s --isPedestrian=1 --isTcp=1 --s1uLinkDelay=0.001 --p2pLinkDelay=0.001 --traceFile=/Users/binh/Documents/shared/fading_trace_EPA_3kmph-100-3.fad --tag=700m --traceTime=100 --moving_speed=3 --distance=1000" > out/tcp-1000m 2>&1

