#!/usr/bin/python
import os
import sys
import re

INPUT = "/Users/binh/Documents/workspace/ns-3-dev/pcaps/e2e-udp-simple-0-2.pcap"
TMP = "/Users/binh/Documents/workspace/ns-3-dev/pcaps/"
OUT = "/Users/binh/Desktop/ns3_play/rates.txt"
N_UE = 3

def filter():
    retval  = []
    port = 2000
    for i in range (0, int (N_UE) ):
        port += 1
        rs = os.popen("tshark -r "+INPUT+" -R \"udp.dstport=="+ str (port)+"\" -w "+TMP+str (i)+".pcap").read()
        rs = os.popen ("capinfos -i "+TMP+str (i)+".pcap").read()
        m = re.search(r'Data bit rate: (.*) bits/sec',str (rs))
        if m:
            p = m.group(1).split()[0]
        retval.append(p)
    return retval
        
if __name__ == "__main__":
    N_UE = sys.argv[1]
    OUT = sys.argv[2]
    outf = open(OUT,'w')
    retval = filter()
    print ("input = "+INPUT)
    print ("ouput = "+OUT)
    outf.write("#UE#    rate (eNB dump) Mbps\n")
    print ("#UE#    rate (eNB dump) Mbps\n")
    cnt = 1
    for i in retval:
        i = float (i)/(1024*1024)*900/930
        outf.write(str (cnt)+"    "+ str (i) +"\n")
        print (str (cnt)+"    "+ str (i) +"\n")
        cnt += 1
    
