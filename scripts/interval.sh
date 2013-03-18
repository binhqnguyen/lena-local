cd /Users/binh/Documents/workspace/lena
NOP=10000000
SIMTIME=20
UE=1
PACKETSIZE=900
TIMEINT=240          #microseconds  (69us-100Mbps, 240us-30Mbps, 360us-20Mbps)
ULBANDWIDTH=25     #radioUlBandwidth
DLBANDWIDTH=100     #radioDlBandwidth, should be HIGHER than ulink for ulink experiments.
declare -i S1URATE=1000 #0.1 
declare -i P2PRATE=1000 #0.1
MB=Mb/s
PCAP="$TIMEINT"
echo "output directory .... /Users/binh/Desktop/ns3_play/e2e_results/25-ul-100-dl/"
#rm /Users/binh/Desktop/ns3_play/output-ul.txt
 #rm /Users/binh/Desktop/ns3_play/output-dl.txt


while [ $TIMEINT -le 15000 ]; do         #down to 0.3 Mbps (15000)
          #####UPLINK
          PCAP=$TIMEINT
#          sudo ./waf --run "scratch/e2e_udp_simple --pcapName="ul-$PCAP" --radioDlBandwidth=$DLBANDWIDTH --radioUlBandwidth=$ULBANDWIDTH --s1uLinkDataRate=$S1URATE$MB --p2pLinkDataRate=$P2PRATE$MB --numberOfUeNodes=$UE --interPacketInterval=$TIMEINT --packetSize=$PACKETSIZE --numberOfPackets=$NOP --simTime=$SIMTIME" > /Users/binh/Desktop/ns3_play/e2e_results/25-ul-25-dl/S1R-$S1URATE-P2PR-$P2PRATE-Ue-$UE-interval-$TIMEINT-pacS-$PACKETSIZE-pacN-$NOP.txt
          
          #####DOWNLINK
           sudo ./waf --run "scratch/e2e-udp-ul --isAMRLC=0 --pcapName="dl-$PCAP" --radioDlBandwidth=$DLBANDWIDTH --radioUlBandwidth=$ULBANDWIDTH --s1uLinkDataRate=$S1URATE$MB --p2pLinkDataRate=$P2PRATE$MB --numberOfUeNodes=$UE --interPacketInterval=$TIMEINT --packetSize=$PACKETSIZE --numberOfPackets=$NOP --simTime=$SIMTIME" > /Users/binh/Desktop/ns3_play/e2e_results/ul-25-dl-100/S1R-$S1URATE-P2PR-$P2PRATE-Ue-$UE-interval-$TIMEINT-pacS-$PACKETSIZE-pacN-$NOP.txt
          # sudo ./waf --run "scratch/e2e-udp-dl --pcapName="dl-$PCAP" --radioDlBandwidth=$DLBANDWIDTH --radioUlBandwidth=$ULBANDWIDTH --s1uLinkDataRate=$S1URATE$MB --p2pLinkDataRate=$P2PRATE$MB --numberOfUeNodes=$UE --interPacketInterval=$TIMEINT --packetSize=$PACKETSIZE --numberOfPackets=$NOP --simTime=$SIMTIME" > /Users/binh/Desktop/ns3_play/e2e_results/ul-25-dl-100/S1R-$S1URATE-P2PR-$P2PRATE-Ue-$UE-interval-$TIMEINT-pacS-$PACKETSIZE-pacN-$NOP.txt

#          echo $TIMEINT
          if [ $TIMEINT -ge 1000 ]; then
                    let TIMEINT=TIMEINT+200
          fi
          if [ $TIMEINT -ge 2000 ]; then
                    let TIMEINT=TIMEINT+300
          fi
          if [ $TIMEINT -ge 470 ]; then  #radio peak rate
                    let TIMEINT=TIMEINT+150
          fi
#          echo $TIMEINT
         let TIMEINT=TIMEINT+30
done