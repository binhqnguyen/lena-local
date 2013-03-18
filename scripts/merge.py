#!/usr/bin/python

INPUT = "/Users/binh/Desktop/ns3_play/server_seq.txt"
OUTPUT = "/Users/binh/Desktop/ns3_play/server_seq_o.txt"

if __name__ == "__main__":
    cnt = 0
    input_f = open (INPUT)
    output_f = open (OUTPUT,'w')
    line = input_f.readline()
    while line:
        #if cnt % 10 == 0:
        while cnt < int (line):
            #output_f.write (str (cnt)+"\n")
            cnt += 1 
        if cnt % 50 == 0:
            output_f.write(str (cnt)+"    "+line)
        line = input_f.readline()
        cnt += 1
    input_f.close()
    output_f.close()
    
