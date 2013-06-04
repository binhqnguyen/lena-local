#!/bin/bash

cd /Users/binh/Documents/workspace/lena/results/tcp
pwd

TCP='TcpNewReno'
cd graphs
rm *.svg

cd ..

cd data-scripts/radio
pwd

cat < TCP | grep ID1 | grep -v 7.0.0.2 | grep -v enb | grep -v RLC_buffer | grep -v 'lost=' > tcp-queue.txt
cat < TCP | grep "RLC_buffer_discarded" > tcp_buffer_discarded.txt
cat < UDP | grep ID1 | grep -v 7.0.0.2 | grep -v enb | grep -v RLC_buffer | grep -v 'lost=' > udp-queue.txt
cat < TCP | grep 7.0.0.2 | grep -v ID1 | grep -v enb | grep -v RLC_buffer | grep -v 'lost=' > tcp-put.txt
cat < TCP | grep cwnd_from | grep -v ID1 | grep -v enb | grep -v RLC_buffer | grep -v 'lost=' | grep -v '7.0.0.2' > cwnd.txt
cat < TCP | grep UlTx | grep -v ID1 | grep -v enb | grep -v RLC_buffer | grep -v 'lost=' | grep -v '7.0.0.2' > harq.txt
cat < TCP | grep "$TCP:DupAck" | grep -v '7.0.0.2' | grep "Triple dupack" > 3ACKs.txt
cat < TCP | grep "$TCP:DupAck" | grep -v '7.0.0.2' | grep "Dupack in fast recovery mode" > Dupack_FASTRECOVERY.txt
cat < TCP | grep "$TCP:NewAck" | grep 'Partial ACK' > newack_partial.txt
cat < TCP | grep "$TCP:NewAck" | grep 'Received full ACK' > newack_fullack.txt
cat < TCP | grep "$TCP:Retransmit" | grep RTO > RTO.txt
#cat < TCP | grep "QQQQ:" > queues.txt
#cat < TCP | grep "ENQUEUED:" > enqueued.txt

cat < UDP | grep 7.0.0.2 | grep -v ID1 | grep -v enb | grep -v RLC_buffer | grep -v 'lost=' > udp-put.txt

cat < TCP-dl | grep ID1 | grep -v 7.0.0.2 | grep -v enb | grep -v RLC_buffer | grep -v 'lost=' > tcp-queue-dl.txt
cat < TCP-dl | grep "RLC_buffer_discarded" > tcp_buffer_discarded_dl.txt
cat < TCP-dl | grep "Tcp lost=" > tcp_lost_dl.txt
cat < UDP-dl | grep ID1 | grep -v 7.0.0.2 | grep -v enb | grep -v RLC_buffer | grep -v 'lost=' > udp-queue-dl.txt
#cat < tcp-100-3-ul-700m-nofading | grep ID1 > tcp-100-3-ul-700m-buffer-nofading.txt
#cat < udp-100-3-ul-700m-nofading | grep ID1 > udp-100-3-ul-700m-buffer-nofading.txt
cat < TCP-dl | grep 7.0.0.2 | grep -v ID1 | grep -v enb | grep -v RLC_buffer | grep -v 'lost=' > tcp-put-dl.txt
cat < TCP-dl | grep cwnd_from | grep -v ID1 | grep -v enb | grep -v RLC_buffer | grep -v 'lost=' | grep -v '7.0.0.2' > cwnd-dl.txt
cat < TCP-dl | grep DlTx | grep -v ID1 | grep -v enb | grep -v RLC_buffer | grep -v 'lost=' | grep -v '7.0.0.2' > harq-dl.txt
cat < UDP-dl | grep 7.0.0.2 | grep -v ID1 | grep -v enb | grep -v RLC_buffer | grep -v 'lost=' > udp-put-dl.txt



./avg_queue_delay.py
#./avg_tcp_delay.py

#gnuplot plot-buffer.txt
#gnuplot plot-aggregate.txt
#gnuplot plot-small-interval.txt
gnuplot plot-averaged.txt
#gnuplot plot-averaged-dl.txt

#gnuplot plot-ul-200m.gnu.txt
#gnuplot plot-ul-300m.gnu.txt
#gnuplot plot-ul-400m.gnu.txt
#gnuplot plot-ul-500m.gnu.txt
#gnuplot plot-ul-600m.gnu.txt
#gnuplot plot-ul-700m.gnu.txt
#gnuplot plot-ul-800m.gnu.txt
#gnuplot plot-ul-900m.gnu.txt
#gnuplot plot-ul-1000m.gnu.txt
#gnuplot plot-ul-1100m.gnu.txt
#gnuplot plot-ul-1200m.gnu.txt
#gnuplot plot-ul-1300m.gnu.txt

#gnuplot plot-3kmph.gnu.txt
#gnuplot plot-2ue.gnu.txt

cp *.svg ../../graphs/radio
rm *.svg

