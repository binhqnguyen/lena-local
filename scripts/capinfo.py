#!/usr/bin/python
"""
This script read the result files from a folder. 
Extract all information of the each files inside the folder to a file for gnuplot.
There are 6 variable (X-axis) that this script will sketch: 

S1uLinkDataRate: The rate of the S1U link.
P2PLinkDataRate: The rate of the P2P link.
numberOfUeNodes: Number of Ue nodes in 1 cell.
packetTimeInterval: The time interval of sending 2 packets.
packetSize: Size of sendding packet.
packetNumber: Number of packets sent.

The Y-axis of the graph sketch all information recorded in the files. They are:

PdcpUlDelay: Delay from UePdcp layer-to-EnodeBPdcp layer.
PdcpDlDelay: same as above but for downlink direction.
RlcUlDelay: Delay from UeRlc layer-to-EnodeBRlc layer.
RlcDlDelay: same as above but for downlink direction.
e2e-delay: The latency of sending a udp packet from Ue to Endhost.
LTELatency: The latency of sending a packet from UE to EnodeB (LTE part only).
**Note: LTELatency is not recorded by the experiment. It is calculated by a formula: 
    LTELatency = e2e-delay - (PdcpULDelay+PdcpDlDelay).
    
The results of this script are data files in the /Users/binh/Desktop/ns3_play/sketch_data/ folder.
These files contains data for sketching script (sketch.py) which draw the graph of data on columns of the data file.  
"""

import os
import sys

###This class is used to handle (read/write) file/folder.
class FileHandle(object):
    '''
    classdocs
    '''
#    OUTPUT_FILE = "/Users/binh/Desktop/ns3_play/sketch_data/time_interval.dat"
    input_dir = ""
    output_file = "";
    old_file_path_list = []

    #constructor
    def __init__(self,input_dir, output_file = ""):
        if input_dir is "":
            print("Warning: No input directory explicit! quit now.")
            exit()
        self.input_dir = input_dir
        self.output_file = output_file
        
    #read a file, return the Y-axis values (line #3) of the file
    def __read_single_file__(self, file_name = ""):
#        print("***reading ... " + file_name)
        return_value = []
        values_str = ""
        file = open(file_name)
        line = file.readline()
        i = 0
        while line:
            i += 1
            if i == 4:
                values_str += line
                break
            line = file.readline()
        file.close()
#        print("return = " + values_str)
        return_value = values_str.split()
#        print("values = "+str(return_value))
        return return_value
    
    #write a string to OUTPUT_FILE
    def write_output(self, write_content):
        file = open(self.output_file,'a')
        file.write(write_content)
        file.close()
   
    #read all files of a specified directory, return a list of names of files in the directory.
    def read_dir(self): 
        self.old_file_path_list = []                ######EMPTY the return value. SUPER IMPORTANT!!!
        #print ("input dir = " + self.input_dir)
        f_list = os.listdir(self.input_dir)
        if not f_list:
            return self.old_file_path_list
        for file in f_list:
            file = self.input_dir + "/"+file
            self.old_file_path_list.append(file)
        return self.old_file_path_list
            
    #rename a file name, replace all SPACES by UNDERSCORES, this helps unix prevent a break path due to spaces in file name. Return the changed file path
    def __eliminate_spaces__(self, file_path):   
        new_file_path = self.input_dir + os.path.basename(file_path).replace(' ','_')
        #print(new_file_path)
        if new_file_path is not file_path:
            os.rename(file_path, new_file_path)
            #print("renamed " + file_path + " into " + new_file_path)
        return new_file_path
    
    def clear_output_file(self):
        open(self.output_file,"w").close()###################

##path of the directory that contains result files.
INPUT_DIR = "/Users/binh/Documents/workspace/ns-3-dev/pcaps/"
OUT_FILE = "/Users/binh/Documents/workspace/ns-3-dev/pcaps/total-ul-25.txt"

def write_to_file(x, values):
    fh.write_output(x+"    ")
    ctr = 0
    while (ctr < len(values)):
        ##skip the cellID and Imsi
        if (ctr >= 2):
            fh.write_output(values[ctr][:-2]+"    ")
        ctr += 1;
    ##LTElatency = PdcpULDelay + PdcpDlDelay
    #LTELatency = float(values[2][:-2]) + float(values[3][:-2])
    #fh.write_output(str(LTELatency))
    fh.write_output("\n")
    
###Main function.
if __name__ == '__main__':
    fh = FileHandle(INPUT_DIR)
    fh.output_file = OUT_FILE
    fh.clear_output_file()
    
    print ("***Extracting from .... "+INPUT_DIR)
    print ("***Data writing into .... "+OUT_FILE)
    
    try:
        path_list = fh.read_dir()
    except Exception:
        sys.stderr.write('ERROR: ')
        sys.stderr.write("no such dir " + INPUT_DIR + "\n")
        exit()
        
    ##For each file in the directory
#    for file_path in path_list:
#        if os.path.splitext(file_path)[1] != ".pcap":   #skip all not .pcap files.
#            continue
##        print ("tshark -r "+file_path+" -R \"udp.srcport==49153 and udp.dstport==2001\" -w "+INPUT_DIR+"ul-"+os.path.basename(file_path) )
#        #UL
#        os.system("tshark -r "+file_path+" -R \"udp.srcport==49153 and udp.dstport==2001\" -w "+INPUT_DIR+"ul-"+os.path.basename(file_path) )
#        #DL
#        os.system("tshark -r "+file_path+" -R \"udp.srcport==2001 and udp.dstport==49153\" -w "+INPUT_DIR+"dl-"+os.path.basename(file_path) )

        
    file = []
    app_rate = []
    ul = []
    dl = []
        
    fh.write_output("#file name    App rate     UL radio rate\n")    
    ##For each file in the directory
    for file_path in path_list:
        if (os.path.splitext(file_path)[1] != ".pcap" or ( os.path.basename(file_path).split('-')[0] != "ul" and os.path.basename(file_path).split('-')[0] != "dl") or os.path.basename(file_path).split('-')[2] != "1"):   #skip all not .pcap files.
            continue
        rs = os.popen("capinfos -i "+file_path)
        rs.readline()
        
        print ( os.path.basename(file_path) )
        
        if (os.path.basename(file_path).split('-')[0] == "ul"):
            file.append(os.path.basename(file_path))
            app_rate.append(str ( 900*8*1000000/float (os.path.basename(file_path).split('-')[1])/(1024*1024) ))
            ul.append(str (float (rs.readline().split()[3])/(1024*1024)))
#        if (os.path.basename(file_path).split('-')[0] == "dl"):
#            file.append(os.path.basename(file_path))
#            app_rate.append(str ( 900*8*1000000/float (os.path.basename(file_path).split('-')[1])/(1024*1024) ))
#            dl.append(str (float (rs.readline().split()[3])/(1024*1024)))
            
    print (len (file) )
    print (len (ul))
    print (len (app_rate))
    print (len (dl))
    for i in range(0, len (file) ):
        fh.write_output(file[i]+"    "+app_rate[i]+"    "+ul[i]+"\n")