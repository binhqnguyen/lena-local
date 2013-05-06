cd /Users/binh/Documents/workspace/lena

./waf --run "e2e-fading-ul-mobile --simTime=100 --dataRate=100000kb/s --isPedestrian=1 --numberOfUeNodes=2 --isTcp=0 --s1uLinkDelay=0.001 --p2pLinkDelay=0.001 --traceFile=/Users/binh/Documents/shared/fading_trace_EPA_3kmph-100-3.fad --tag=100-3-2ue-udp-700m --traceTime=100 --moving_speed=3 --distance=700" > udp-100-3-2ue-700m 2>&1
sleep 10m

###ul tc
./waf --run "e2e-fading-ul-mobile --simTime=100 --dataRate=100000kb/s --isPedestrian=1 --numberOfUeNodes=2 --isTcp=1 --s1uLinkDelay=0.001 --p2pLinkDelay=0.001 --traceFile=/Users/binh/Documents/shared/fading_trace_EPA_3kmph-100-3.fad --tag=100-3-2ue-tcp-700m --traceTime=100 --moving_speed=3 --distance=700" > tcp-100-3-2ue-700m 2>&1
sleep 10m


