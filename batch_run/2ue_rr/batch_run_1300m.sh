cd /Users/binh/Documents/workspace/lena
sleep 10m

./waf --run "e2e-fading-ul-mobile --simTime=100 --dataRate=100000kb/s --isPedestrian=1 --isTcp=0 --s1uLinkDelay=0.001 --p2pLinkDelay=0.001 --traceFile=/Users/binh/Documents/shared/fading_trace_EPA_3kmph-100-1.fad --tag=100-1-ul-udp-1300m --traceTime=100 --moving_speed=1 --distance=1300" > udp-100-1-ul-1300m 2>&1
sleep 10m


./waf --run "e2e-fading-ul-mobile --simTime=100 --dataRate=100000kb/s --isPedestrian=1 --isTcp=0 --s1uLinkDelay=0.001 --p2pLinkDelay=0.001 --traceFile=/Users/binh/Documents/shared/fading_trace_EPA_3kmph-100-2.fad --tag=100-2-ul-udp-1300m --traceTime=100 --moving_speed=2 --distance=1300" > udp-100-2-ul-1300m 2>&1
sleep 10m


./waf --run "e2e-fading-ul-mobile --simTime=100 --dataRate=100000kb/s --isPedestrian=1 --isTcp=0 --s1uLinkDelay=0.001 --p2pLinkDelay=0.001 --traceFile=/Users/binh/Documents/shared/fading_trace_EPA_3kmph-100-3.fad --tag=100-3-ul-udp-1300m --traceTime=100 --moving_speed=3 --distance=1300" > udp-100-3-ul-1300m 2>&1
sleep 10m

./waf --run "e2e-fading-ul-mobile --simTime=100 --dataRate=100000kb/s --isPedestrian=1 --isTcp=0 --s1uLinkDelay=0.001 --p2pLinkDelay=0.001 --traceFile=/Users/binh/Documents/shared/fading_trace_EPA_3kmph-100-4.fad --tag=100-4-ul-udp-1300m --traceTime=100 --moving_speed=4 --distance=1300" > udp-100-4-ul-1300m 2>&1
sleep 10m



###ul tcp
./waf --run "e2e-fading-ul-mobile --simTime=100 --dataRate=100000kb/s --isPedestrian=1 --isTcp=1 --s1uLinkDelay=0.001 --p2pLinkDelay=0.001 --traceFile=/Users/binh/Documents/shared/fading_trace_EPA_3kmph-100-1.fad --tag=100-1-ul-tcp-1300m --traceTime=100 --moving_speed=1 --distance=1300" > tcp-100-1-ul-1300m 2>&1
sleep 10m


./waf --run "e2e-fading-ul-mobile --simTime=100 --dataRate=100000kb/s --isPedestrian=1 --isTcp=1 --s1uLinkDelay=0.001 --p2pLinkDelay=0.001 --traceFile=/Users/binh/Documents/shared/fading_trace_EPA_3kmph-100-2.fad --tag=100-2-ul-tcp-1300m --traceTime=100 --moving_speed=2 --distance=1300" > tcp-100-2-ul-1300m 2>&1
sleep 10m


./waf --run "e2e-fading-ul-mobile --simTime=100 --dataRate=100000kb/s --isPedestrian=1 --isTcp=1 --s1uLinkDelay=0.001 --p2pLinkDelay=0.001 --traceFile=/Users/binh/Documents/shared/fading_trace_EPA_3kmph-100-3.fad --tag=100-3-ul-tcp-1300m --traceTime=100 --moving_speed=3 --distance=1300" > tcp-100-3-ul-1300m 2>&1
sleep 10m

./waf --run "e2e-fading-ul-mobile --simTime=100 --dataRate=100000kb/s --isPedestrian=1 --isTcp=1 --s1uLinkDelay=0.001 --p2pLinkDelay=0.001 --traceFile=/Users/binh/Documents/shared/fading_trace_EPA_3kmph-100-4.fad --tag=100-4-ul-tcp-1300m --traceTime=100 --moving_speed=4 --distance=1300" > tcp-100-4-ul-1300m 2>&1
sleep 10m

