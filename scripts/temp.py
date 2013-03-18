#!/usr/bin/python

import os
import sys
import FileHandle

INPUT_FILE = "/Users/binh/Desktop/ns3_play/back-up/Multiple_ues_latency.txt"
OUTPUT_FILE = "/Users/binh/Desktop/ns3_play/back-up/Multiple_ues_latency_1.txt"
INTERVAL = 1    ###1ms

if __name__ == "__main__":
    tokens = []
    result_list = []
    file = open (INPUT_FILE)
    line = file.readline()
    ctr = 0
    outfile = open (OUTPUT_FILE,'w+')
    outfile.write("#UEs    100B    200B    400B    600B    800B    900B\n")
    while (line):
        print ("line = "+line)
        if (ctr == 0):
            line = file.readline()
            ctr += 1
            continue
        tokens = {}
        tokens = line.split()
        for t in tokens:
            outfile.write(str (t)+"    ")
        outfile.write("\n")
        line = file.readline()
    file.close()
    outfile.close()