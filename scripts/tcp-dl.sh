cd /Users/binh/Documents/workspace/lena
SIMTIME=20
UE=1
PACKETSIZE=900
ULBANDWIDTH=25     #radioUlBandwidth
DLBANDWIDTH=100     #radioDlBandwidth, should be HIGHER than ulink for ulink experiments.
DISTANCE=100
RATE=10   #10kb/s
declare -i S1URATE=1000 #0.1 
declare -i P2PRATE=1000 #0.1
KB=kb/s
PCAP="$TIMEINT"
echo "output directory .... /Users/binh/Desktop/ns3_play/e2e_results/tcp/1ue-dl"
#rm /Users/binh/Desktop/ns3_play/output-ul.txt
 #rm /Users/binh/Desktop/ns3_play/output-dl.txt


while [ $RATE -le 50000 ]; do         #rate up to 50Mb/s
          #####UPLINK
          PCAP=$TIMEINT
#          sudo ./waf --run "scratch/e2e_udp_simple --pcapName="ul-$PCAP" --radioDlBandwidth=$DLBANDWIDTH --radioUlBandwidth=$ULBANDWIDTH --s1uLinkDataRate=$S1URATE$MB --p2pLinkDataRate=$P2PRATE$MB --numberOfUeNodes=$UE --interPacketInterval=$TIMEINT --packetSize=$PACKETSIZE --numberOfPackets=$NOP --simTime=$SIMTIME" > /Users/binh/Desktop/ns3_play/e2e_results/25-ul-25-dl/S1R-$S1URATE-P2PR-$P2PRATE-Ue-$UE-interval-$TIMEINT-pacS-$PACKETSIZE-pacN-$NOP.txt
          
          #####DOWNLINK
          ./waf --run "scratch/e2e-tcp-dl --dataRate=$RATE"kb/s" --numberOfUeNodes=$UE --packetSize=$PACKETSIZE --simTime=$SIMTIME" > /Users/binh/Desktop/ns3_play/e2e_results/tcp/1ue-dl/Ue-$UE-rate-$RATE-pacS-$PACKETSIZE.txt 2>&1
          #echo -e ./waf --run \"scratch/e2e-tcp-ul --dataRate=$RATE"kb/s" --s1uLinkDataRate=$S1URATE$MB --p2pLinkDataRate=$P2PRATE$MB --numberOfUeNodes=$UE --packetSize=$PACKETSIZE --simTime=$SIMTIME\"
          # sudo ./waf --run "scratch/e2e-udp-dl --pcapName="dl-$PCAP" --radioDlBandwidth=$DLBANDWIDTH --radioUlBandwidth=$ULBANDWIDTH --s1uLinkDataRate=$S1URATE$MB --p2pLinkDataRate=$P2PRATE$MB --numberOfUeNodes=$UE --interPacketInterval=$TIMEINT --packetSize=$PACKETSIZE --numberOfPackets=$NOP --simTime=$SIMTIME" > /Users/binh/Desktop/ns3_play/e2e_results/ul-25-dl-100/S1R-$S1URATE-P2PR-$P2PRATE-Ue-$UE-interval-$TIMEINT-pacS-$PACKETSIZE-pacN-$NOP.txt
          #echo scratch/e2e-tcp-ul --dataRate=$RATE$KB --pcapName=dl-$PCAP --s1uLinkDataRate=$S1URATE$MB --p2pLinkDataRate=$P2PRATE$MB --numberOfUeNodes=$UE --packetSize=$PACKETSIZE --simTime=$SIMTIME > /Users/binh/Desktop/ns3_play/e2e_results/tcp/1ue-ul/Ue-$UE-rate-$RATE-pacS-$PACKETSIZE.txt 2>&1"

          echo $RATE$KB
#          echo $TIMEINT
          if [ $RATE -ge 1000 ]; then
                    let RATE=RATE+200
          fi
          if [ $RATE -ge 5000 ]; then
                    let RATE=RATE+300
          fi
          if [ $RATE -ge 8000 ]; then  #radio peak rate
                    let RATE=RATE+500
          fi
#          echo $TIMEINT
         let RATE=RATE+100
done