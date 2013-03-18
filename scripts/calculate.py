#!/usr/bin/python

import os
import sys
import FileHandle

INPUT_FILE = "/Users/binh/Desktop/ns3_play/data/interval-S1R1000-P2PR1000-Ue1-interval0-pacS900-pacN1000000.txt"
OUTPUT_FILE_PREFIX = "/Users/binh/Desktop/ns3_play/back-up/traffic-latency-25dl"
OUTPUT_FILE_ALL = "/Users/binh/Desktop/ns3_play/back-up/rate-latency-25-dl.txt"
INTERVAL = 1    ###1ms

if __name__ == "__main__":
    result1 = {}         ##result[traffic] = latency
#    result2 = {}
#    result3 = {}
#    result4 = {}
#    result5 = {}
#    result6 = {}
    result_list = []
    value_list = []
    values = []
    file = open (INPUT_FILE)
    line = file.readline()
    
    
    if (os.path.isfile(OUTPUT_FILE_ALL)):      ##if output file not exist
        open(OUTPUT_FILE_ALL,'w').close()
    outfile_all = open (OUTPUT_FILE_ALL,'w+')
    outfile_all.write("#Traffic (MBps)    Latency(ms)    ULPDCP    DLPDCP    ULRLC    DLRLC    %    xx\n")
    
#    ctr = 0
    while (line):
#        if (ctr == 0):
#            line = file.readline()
#            ctr += 1
#            continue
        tokens = {}
        values = {}
        tokens = line.split()
        rest = str (float (tokens[1])*1000)+"    "+str (float (tokens[2])*1000)+"    "+str (float (tokens[3])*1000)+"    "+str (float (tokens[4])*1000 )
        rest += "    "+ str (tokens[7])
        result1[ 900*8*1000000/float (tokens[0])/(1024*1024) ] = tokens[5]      ##result[200B/1ms*# of UEs] = first column value of the file
        outfile_all.write(str (900*8*1000000/float (tokens[0])/(1024*1024)) +"    "+tokens[5]+"    "+rest+"\n")
#        result2[400*1000*float (tokens[0])/1/(1024*1024)] = tokens[2]
#        result3[600*1000*float (tokens[0])/1/(1024*1024)] = tokens[3] 
#        result4[600*1000*float (tokens[0])/1/(1024*1024)] = tokens[4] 
#        result5[800*1000*float (tokens[0])/1/(1024*1024)] = tokens[5] 
#        result6[900*1000*float (tokens[0])/1/(1024*1024)] = tokens[6] 
        line = file.readline()
        value_list.append(rest)
        
    result_list.append(result1)
#    result_list.append(result2)
#    result_list.append(result3)
#    result_list.append(result4)
#    result_list.append(result5)
#    result_list.append(result6)
    
#    outfile_all = open (OUTPUT_FILE_ALL, 'w+')
#    outfile_all.write("#Traffic (MBps)    Latency(ms)\n")
    
#    for r in result_list:
##        o_fn = OUTPUT_FILE_PREFIX+str (c)+".txt"
#        if (os.path.isfile(OUTPUT_FILE_ALL)):      ##if output file not exist
#            open(OUTPUT_FILE_ALL,'w').close()
#        outfile_all = open (OUTPUT_FILE_ALL,'w+')
#        outfile_all.write("#Traffic (MBps)    Latency(ms)    ULPDCP    DLPDCP    ULRLC    DLRLC\n")
#        c = 0
#        for key in r:
#            outfile_all.write(str (key)+"    "+str (r[key]))
#            outfile_all.write("    "+value_list[c]+"\n")
##            outfile_all.write(str (key)+"    "+str (r[key])+"\n")
#            c += 1
    print ("Outfile = "+OUTPUT_FILE_ALL)
    file.close()
#    outfile.close()
    outfile_all.close()


        