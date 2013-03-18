#!/bin/bash
cd /Users/binh/Documents/workspace/ns-3-dev
NOP=1000
SIMTIME=1
UE=1
PACKETSIZE=50
TIMEINT=1
declare -i S1URATE=1000 #0.1 
declare -i P2PRATE=1000 #0.1
MB=Mb/s
while [ $P2PRATE -le 10000  ]; do
          NOP=1000
          UE=1
          PACKETSIZE=50
          TIMEINT=1
          S1URATE=1000
          while [ $S1URATE -le 10000 ]; do
                    NOP=1000
                    UE=1
                    PACKETSIZE=50
                    TIMEINT=1
                    while [ $UE -le 30 ]; do
                              NOP=1000
                              PACKETSIZE=50
                              TIMEINT=1
                    	while [ $TIMEINT -le 200 ]; do
                    	         NOP=1000
                    	         PACKETSIZE=50
                    		while [ $PACKETSIZE -le 1450 ]; do
                    		        NOP=1000
                    			while [ $NOP -lt 1001 ]; do
                    			          #echo UE=$UE TIME=$TIMEINT PACS=$PACKETSIZE NOP=$NOP S1=$S1URATE P2P=$P2PRATE
                    			          echo "scratch/e2e_udp_simple --s1uLinkDataRate=$S1URATE$MB --p2pLinkDataRate=$P2PRATE$MB --numberOfUeNodes=$UE --interPacketInterval=$TIMEINT --packetSize=$PACKETSIZE --numberOfPackets=$NOP --simTime=$SIMTIME"
                    			          #echo "/Users/binh/Desktop/ns3_play/e2e_udp_simple_result/S1R-$S1URATE-P2PR-$P2PRATE-Ue-$UE-interval-$TIMEINT-pacS-$PACKETSIZE-pacN-$NOP.txt"
                    				sudo ./waf --run "scratch/e2e_udp_simple --s1uLinkDataRate=$S1URATE$MB --p2pLinkDataRate=$P2PRATE$MB --numberOfUeNodes=$UE --interPacketInterval=$TIMEINT --packetSize=$PACKETSIZE --numberOfPackets=$NOP --simTime=$SIMTIME" > /Users/binh/Desktop/ns3_play/e2e_udp_simple_result/S1R-$S1URATE-P2PR-$P2PRATE-Ue-$UE-interval-$TIMEINT-pacS-$PACKETSIZE-pacN-$NOP.txt
                    				let NOP=NOP+1000
                    				#sleep $SIMTIME+0.5
                    			done
                    			let PACKETSIZE=PACKETSIZE+200
                    		done
                    		if [ $TIMEINT>20 ]; then
                    	                   let TIMEINT=TIMEINT+5
                    	          else
                                                 if [ $TIMEINT>100 ]; then
          	                                       let TIMEINT=TIMEINT+25
          	                             else
          	                                       let TIMEINT=TIMEINT+1
          	                             fi
                    	          fi
                    	done
                              let UE=UE+5
                    done
                    let S1URATE=S1URATE+500
          done
          let P2PRATE=P2PRATE+500
done