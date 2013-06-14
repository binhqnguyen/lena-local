cd /Users/binh/Documents/workspace/lena
sleep 10m

./waf --run "e2e-fading-ul-mobile --simTime=100 --dataRate=100000kb/s --isPedestrian=1 --isTcp=0 --s1uLinkDelay=0.001 --p2pLinkDelay=0.001 --traceFile=/Users/binh/Documents/shared/fading_trace_EPA_3kmph-100-1.fad --tag=100-1-ul-udp --traceTime=100 --moving_speed=1 --distance=500" > udp-100-1-ul 2>&1
sleep 10m


./waf --run "e2e-fading-ul-mobile --simTime=100 --dataRate=100000kb/s --isPedestrian=1 --isTcp=0 --s1uLinkDelay=0.001 --p2pLinkDelay=0.001 --traceFile=/Users/binh/Documents/shared/fading_trace_EPA_3kmph-100-2.fad --tag=100-2-ul-udp --traceTime=100 --moving_speed=2 --distance=500" > udp-100-2-ul 2>&1
sleep 10m


./waf --run "e2e-fading-ul-mobile --simTime=100 --dataRate=100000kb/s --isPedestrian=1 --isTcp=0 --s1uLinkDelay=0.001 --p2pLinkDelay=0.001 --traceFile=/Users/binh/Documents/shared/fading_trace_EPA_3kmph-100-3.fad --tag=100-3-ul-udp --traceTime=100 --moving_speed=3 --distance=500" > udp-100-3-ul 2>&1
sleep 10m

./waf --run "e2e-fading-ul-mobile --simTime=100 --dataRate=100000kb/s --isPedestrian=1 --isTcp=0 --s1uLinkDelay=0.001 --p2pLinkDelay=0.001 --traceFile=/Users/binh/Documents/shared/fading_trace_EPA_3kmph-100-4.fad --tag=100-4-ul-udp --traceTime=100 --moving_speed=4 --distance=500" > udp-100-4-ul 2>&1
sleep 10m



###ul tcp
./waf --run "e2e-fading-ul-mobile --simTime=100 --dataRate=100000kb/s --isPedestrian=1 --isTcp=1 --s1uLinkDelay=0.001 --p2pLinkDelay=0.001 --traceFile=/Users/binh/Documents/shared/fading_trace_EPA_3kmph-100-1.fad --tag=100-1-ul-tcp --traceTime=100 --moving_speed=1 --distance=500" > tcp-100-1-ul 2>&1
sleep 10m


./waf --run "e2e-fading-ul-mobile --simTime=100 --dataRate=100000kb/s --isPedestrian=1 --isTcp=1 --s1uLinkDelay=0.001 --p2pLinkDelay=0.001 --traceFile=/Users/binh/Documents/shared/fading_trace_EPA_3kmph-100-2.fad --tag=100-2-ul-tcp --traceTime=100 --moving_speed=2 --distance=500" > tcp-100-2-ul 2>&1
sleep 10m


./waf --run "e2e-fading-ul-mobile --simTime=100 --dataRate=100000kb/s --isPedestrian=1 --isTcp=1 --s1uLinkDelay=0.001 --p2pLinkDelay=0.001 --traceFile=/Users/binh/Documents/shared/fading_trace_EPA_3kmph-100-3.fad --tag=100-3-ul-tcp --traceTime=100 --moving_speed=3 --distance=500" > tcp-100-3-ul 2>&1
sleep 10m

./waf --run "e2e-fading-ul-mobile --simTime=100 --dataRate=100000kb/s --isPedestrian=1 --isTcp=1 --s1uLinkDelay=0.001 --p2pLinkDelay=0.001 --traceFile=/Users/binh/Documents/shared/fading_trace_EPA_3kmph-100-4.fad --tag=100-4-ul-tcp --traceTime=100 --moving_speed=4 --distance=500" > tcp-100-4-ul 2>&1
sleep 10m



###REdo ul experiment with 10s-20s traces with high speed tcp
./waf --run "e2e-fading-ul-mobile --simTime=100 --dataRate=100000kb/s --isPedestrian=1 --isTcp=0 --s1uLinkDelay=0.001 --p2pLinkDelay=0.001 --traceFile=/Users/binh/Documents/shared/fading_trace_EPA_3kmph-10-1.fad --tag=10-1-ul-udp-HS --traceTime=10 --moving_speed=1 --distance=500" > udp-10-1-ul-HS 2>&1
sleep 10m


./waf --run "e2e-fading-ul-mobile --simTime=100 --dataRate=100000kb/s --isPedestrian=1 --isTcp=0 --s1uLinkDelay=0.001 --p2pLinkDelay=0.001 --traceFile=/Users/binh/Documents/shared/fading_trace_EPA_3kmph-10-2.fad --tag=10-2-ul-udp-HS --traceTime=10 --moving_speed=2 --distance=500" > udp-10-2-ul-HS 2>&1
sleep 10m


./waf --run "e2e-fading-ul-mobile --simTime=100 --dataRate=100000kb/s --isPedestrian=1 --isTcp=0 --s1uLinkDelay=0.001 --p2pLinkDelay=0.001 --traceFile=/Users/binh/Documents/shared/fading_trace_EPA_3kmph-10-3.fad --tag=10-3-ul-udp-HS --traceTime=10 --moving_speed=3 --distance=500" > udp-10-3-ul-HS 2>&1
sleep 10m

./waf --run "e2e-fading-ul-mobile --simTime=100 --dataRate=100000kb/s --isPedestrian=1 --isTcp=0 --s1uLinkDelay=0.001 --p2pLinkDelay=0.001 --traceFile=/Users/binh/Documents/shared/fading_trace_EPA_3kmph-10-4.fad --tag=10-4-ul-udp-HS --traceTime=10 --moving_speed=4 --distance=500" > udp-10-4-ul-HS 2>&1
sleep 10m

./waf --run "e2e-fading-ul-mobile --simTime=100 --dataRate=100000kb/s --isPedestrian=1 --isTcp=0 --s1uLinkDelay=0.001 --p2pLinkDelay=0.001 --traceFile=/Users/binh/Documents/shared/fading_trace_EPA_3kmph-20-1.fad --tag=20-1-ul-udp-HS --traceTime=20 --moving_speed=1 --distance=500" > udp-20-1-ul-HS 2>&1
sleep 10m


./waf --run "e2e-fading-ul-mobile --simTime=100 --dataRate=100000kb/s --isPedestrian=1 --isTcp=0 --s1uLinkDelay=0.001 --p2pLinkDelay=0.001 --traceFile=/Users/binh/Documents/shared/fading_trace_EPA_3kmph-20-2.fad --tag=20-2-ul-udp-HS --traceTime=20 --moving_speed=2 --distance=500" > udp-20-2-ul-HS 2>&1
sleep 10m


./waf --run "e2e-fading-ul-mobile --simTime=100 --dataRate=100000kb/s --isPedestrian=1 --isTcp=0 --s1uLinkDelay=0.001 --p2pLinkDelay=0.001 --traceFile=/Users/binh/Documents/shared/fading_trace_EPA_3kmph-20-3.fad --tag=20-3-ul-udp-HS --traceTime=20 --moving_speed=3 --distance=500" > udp-20-3-ul-HS 2>&1
sleep 10m




###ul tcp
./waf --run "e2e-fading-ul-mobile --simTime=100 --dataRate=100000kb/s --isPedestrian=1 --isTcp=1 --s1uLinkDelay=0.001 --p2pLinkDelay=0.001 --traceFile=/Users/binh/Documents/shared/fading_trace_EPA_3kmph-10-1.fad --tag=10-1-ul-tcp-HS --traceTime=10 --moving_speed=1 --distance=500" > tcp-10-1-ul-HS 2>&1
sleep 10m


./waf --run "e2e-fading-ul-mobile --simTime=100 --dataRate=100000kb/s --isPedestrian=1 --isTcp=1 --s1uLinkDelay=0.001 --p2pLinkDelay=0.001 --traceFile=/Users/binh/Documents/shared/fading_trace_EPA_3kmph-10-2.fad --tag=10-2-ul-tcp-HS --traceTime=10 --moving_speed=2 --distance=500"  > tcp-10-2-ul-HS 2>&1
sleep 10m


./waf --run "e2e-fading-ul-mobile --simTime=100 --dataRate=100000kb/s --isPedestrian=1 --isTcp=1 --s1uLinkDelay=0.001 --p2pLinkDelay=0.001 --traceFile=/Users/binh/Documents/shared/fading_trace_EPA_3kmph-10-3.fad --tag=10-3-ul-tcp-HS --traceTime=10 --moving_speed=3 --distance=500" > tcp-10-3-ul-HS 2>&1
sleep 10m

./waf --run "e2e-fading-ul-mobile --simTime=100 --dataRate=100000kb/s --isPedestrian=1 --isTcp=1 --s1uLinkDelay=0.001 --p2pLinkDelay=0.001 --traceFile=/Users/binh/Documents/shared/fading_trace_EPA_3kmph-10-4.fad --tag=10-4-ul-tcp-HS --traceTime=10 --moving_speed=4 --distance=500" > tcp-10-4-ul-HS 2>&1
sleep 10m

./waf --run "e2e-fading-ul-mobile --simTime=100 --dataRate=100000kb/s --isPedestrian=1 --isTcp=1 --s1uLinkDelay=0.001 --p2pLinkDelay=0.001 --traceFile=/Users/binh/Documents/shared/fading_trace_EPA_3kmph-20-1.fad --tag=20-1-ul-tcp-HS --traceTime=20 --moving_speed=1 --distance=500" > tcp-20-2-ul-HS 2>&1
sleep 10m


./waf --run "e2e-fading-ul-mobile --simTime=100 --dataRate=100000kb/s --isPedestrian=1 --isTcp=1 --s1uLinkDelay=0.001 --p2pLinkDelay=0.001 --traceFile=/Users/binh/Documents/shared/fading_trace_EPA_3kmph-20-2.fad --tag=20-2-ul-tcp-HS --traceTime=20 --moving_speed=2 --distance=500" > tcp-20-2-ul-HS 2>&1
sleep 10m

./waf --run "e2e-fading-ul-mobile --simTime=100 --dataRate=100000kb/s --isPedestrian=1 --isTcp=1 --s1uLinkDelay=0.001 --p2pLinkDelay=0.001 --traceFile=/Users/binh/Documents/shared/fading_trace_EPA_3kmph-20-3.fad --tag=20-3-ul-tcp-HS --traceTime=20 --moving_speed=3 --distance=500" > tcp-20-3-ul-HS 2>&1
sleep 10m





