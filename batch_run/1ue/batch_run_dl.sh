cd /Users/binh/Documents/workspace/lena
###REdo dl experiment with 10s-20s traces with high speed tcp
./waf --run "e2e-fading-dl --simTime=100 --dataRate=100000kb/s --isPedestrian=1 --isTcp=0 --s1uLinkDelay=0.001 --p2pLinkDelay=0.001 --traceFile=/Users/binh/Documents/shared/fading_trace_EPA_3kmph-10-2-d.fad --tag=10-2-dl-udp-HS --traceTime=10" > udp-10-2-dl-HS 2>&1
sleep 10m


./waf --run "e2e-fading-dl --simTime=100 --dataRate=100000kb/s --isPedestrian=1 --isTcp=0 --s1uLinkDelay=0.001 --p2pLinkDelay=0.001 --traceFile=/Users/binh/Documents/shared/fading_trace_EPA_3kmph-10-3-d.fad --tag=10-3-dl-udp-HS --traceTime=10" > udp-10-3-dl-HS 2>&1
sleep 10m


./waf --run "e2e-fading-dl --simTime=100 --dataRate=100000kb/s --isPedestrian=1 --isTcp=0 --s1uLinkDelay=0.001 --p2pLinkDelay=0.001 --traceFile=/Users/binh/Documents/shared/fading_trace_EPA_3kmph-10-4-d.fad --tag=10-4-dl-udp-HS --traceTime=10" > udp-10-4-dl-HS 2>&1
sleep 10m

./waf --run "e2e-fading-dl --simTime=100 --dataRate=100000kb/s --isPedestrian=1 --isTcp=0 --s1uLinkDelay=0.001 --p2pLinkDelay=0.001 --traceFile=/Users/binh/Documents/shared/fading_trace_EPA_3kmph-20-1-d.fad --tag=20-1-dl-udp-HS --traceTime=20" > udp-20-1-dl-HS 2>&1
sleep 10m

./waf --run "e2e-fading-dl --simTime=100 --dataRate=100000kb/s --isPedestrian=1 --isTcp=0 --s1uLinkDelay=0.001 --p2pLinkDelay=0.001 --traceFile=/Users/binh/Documents/shared/fading_trace_EPA_3kmph-20-2-d.fad --tag=20-2-dl-udp-HS --traceTime=20" > udp-20-2-dl-HS 2>&1
sleep 10m


./waf --run "e2e-fading-dl --simTime=100 --dataRate=100000kb/s --isPedestrian=1 --isTcp=0 --s1uLlinkDelay=0.001 --p2pLinkDelay=0.001 --traceFile=/Users/binh/Documents/shared/fading_trace_EPA_3kmph-20-3-d.fad --tag=20-3-dl-udp-HS --traceTime=20" > udp-20-3-dl-HS 2>&1
sleep 10m





###dl tcp
./waf --run "e2e-fading-dl --simTime=100 --dataRate=100000kb/s --isPedestrian=1 --isTcp=1 --s1uLinkDelay=0.001 --p2pLinkDelay=0.001 --traceFile=/Users/binh/Documents/shared/fading_trace_EPA_3kmph-10-2-d.fad --tag=10-2-dl-tcp-HS --traceTime=10" > tcp-10-2-dl-HS 2>&1
sleep 10m


./waf --run "e2e-fading-dl --simTime=100 --dataRate=100000kb/s --isPedestrian=1 --isTcp=1 --s1uLinkDelay=0.001 --p2pLinkDelay=0.001 --traceFile=/Users/binh/Documents/shared/fading_trace_EPA_3kmph-10-3-d.fad --tag=10-3-dl-tcp-HS --traceTime=10" > tcp-10-3-dl-HS 2>&1
sleep 10m


./waf --run "e2e-fading-dl --simTime=100 --dataRate=100000kb/s --isPedestrian=1 --isTcp=1 --s1uLinkDelay=0.001 --p2pLinkDelay=0.001 --traceFile=/Users/binh/Documents/shared/fading_trace_EPA_3kmph-20-1-d.fad --tag=20-1-dl-tcp-HS --traceTime=20" > tcp-20-1-dl-HS 2>&1
sleep 10m

./waf --run "e2e-fading-dl --simTime=100 --dataRate=100000kb/s --isPedestrian=1 --isTcp=1 --s1uLinkDelay=0.001 --p2pLinkDelay=0.001 --traceFile=/Users/binh/Documents/shared/fading_trace_EPA_3kmph-20-2-d.fad --tag=20-2-dl-tcp-HS --traceTime=20" > tcp-20-2-dl-HS 2>&1
sleep 10m

./waf --run "e2e-fading-dl --simTime=100 --dataRate=100000kb/s --isPedestrian=1 --isTcp=1 --s1uLinkDelay=0.001 --p2pLinkDelay=0.001 --traceFile=/Users/binh/Documents/shared/fading_trace_EPA_3kmph-20-3-d.fad --tag=20-3-dl-tcp-HS --traceTime=20" > tcp-20-3-dl-HS 2>&1
sleep 10m

