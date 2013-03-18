cd /Users/binh/Documents/workspace/ns-3-dev
OUT="/Users/binh/Desktop/ns3_play/rates.txt"
UE=20
sudo ./waf --run "scratch/e2e_udp_simple --numberOfPackets=1000000 --interPacketInterval=100 --numberOfUeNodes=$UE"
cd scripts
sudo ./readpcap.py $UE $OUT