cd /Users/binh/Documents/workspace/lena
./waf --run "e2e-fading-ul-mobile --simTime=100 --dataRate=100000kb/s --isPedestrian=1 --isTcp=0 --s1uLinkDelay=0.001 --p2pLinkDelay=0.001 --traceFile=/Users/binh/Documents/shared/fading_trace_EPA_3kmph-100-3.fad --tag=100-3-ul-udp-700m-3 --traceTime=100 --moving_speed=50 --distance=700" > udp-100-3-ul-700m-3 2>&1
./waf --run "e2e-fading-ul-mobile --simTime=100 --dataRate=100000kb/s --isPedestrian=1 --isTcp=1 --s1uLinkDelay=0.001 --p2pLinkDelay=0.001 --traceFile=/Users/binh/Documents/shared/fading_trace_EPA_3kmph-100-3.fad --tag=100-3-ul-tcp-700m-3 --traceTime=100 --moving_speed=50 --distance=700" > tcp-100-3-ul-700m-3 2>&1
