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
            if i == LINE_TO_READ:
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
INPUT_DIR = "/Users/binh/Desktop/ns3_play/e2e_results/dl-25-ul-25-dl"
OUTPUT_DIR = "/Users/binh/Desktop/ns3_play/data/"
#INPUT_DIR = "/Users/binh/Desktop/ns3_play/x"
##Array that stores name of all files in the "INPUT_DIR"
path_list = []
##A list that contains all Y-axis values in each result file 
values = []
##FileHanle, initialized by the INPUTDIR
fh = FileHandle(INPUT_DIR)
##Store the configuration of a file (S1URate, P2PRate, NumberOfUeNodes, etc...)
extract_map = {}
##Specify files that will be included in the sketching.
##If the parameter is set to 0, it means this parameter WILL BE X-Axis.
#The configuration for this extraction. Configuration with 0 means the parameter that is being extracting.       
#S1R = 100
#P2PR = 100
#Ue = 1
#interval = 0
#pacS = 50
#pacN = 1000
target_map ={"S1R":0,
             "P2PR":0,
             "Ue":0,
             "interval":0,
             "pacS":0,
             "pacN":0
             }
para = "interval"
PARAMETER_LIST = ["S1R","P2PR","Ue","interval","pacS","pacN"]


LINE_TO_READ = 6

##Read the file path, determine whether the information of THIS FILE WILL BE INCLUDED IN THE OUTPUT FOR SKETCHING. 
def file_name_filter(file_path):
    is_match = True
    tokens = []
    file_name = os.path.basename(file_path)
    file_name = os.path.splitext(file_name)[0]      ##get the name of file only (without extension).
#    print ("filename = "+file_name)
    tokens = file_name.split('-')
    ctr = 0;
    while (ctr < len(tokens)):
        extract_map[tokens[ctr]] = tokens[ctr+1];
        ctr += 2;
    ##test if skip this file
    
    for key in extract_map:
        ##skip the parameter which is set to 0
        if ( (extract_map[key] != str(target_map[key])) & (target_map[key] != 0)):
            is_match = False
    ##if the file is matched, store the x-value
    if(is_match):
        for key in extract_map:
            if (target_map[key] == 0):
                return extract_map[key]
    
    ##doesn't match, return -1
    return -1
    
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
    
def parse_dir():
    try:
        path_list = fh.read_dir()
    except Exception:
        sys.stderr.write('ERROR: ')
        sys.stderr.write("no such dir " + INPUT_DIR + "\n")
        exit()
    
    isGood = False
        
    ##For each file in the directory
    for file_path in path_list:
        if os.path.splitext(file_path)[1] != ".txt":   #skip all not .txt files.
            continue
        ####If the file name is appropriate, its values will be included in the sketching.
        x = file_name_filter(file_path)
        if (x != -1):
            values = fh.__read_single_file__(file_path)
            #print ("scrutinizing... x =  "+x+" "+os.path.basename(file_path))
            ##write values from the above file to the .dat file for sketching.
            write_to_file(x, values)
            isGood = True
    if (isGood):
        return 0
    else:
        return -1 
def set_target_map(S1R_, P2PR_, Ue_, interval_, pacS_, pacN_):
  target_map["S1R"] = S1R_
  target_map["P2PR"] = P2PR_
  target_map["Ue"] = Ue_
  target_map["interval"] = interval_
  target_map["pacS"] = pacS_
  target_map["pacN"] = pacN_
  
####values captured by experiments and available in files.
S1R_LIST = []
P2PR_LIST = []
UE_LIST = []
INTERVAL_LIST = []
PACS_LIST = []
PACN_LIST = []

def get_experiment_value():
    map = {}
    try:
        path_list = fh.read_dir()
    except Exception:
        sys.stderr.write('ERROR: ')
        sys.stderr.write("no such dir " + INPUT_DIR + "\n")
        exit()
    for file in path_list:
        if (os.path.splitext(file)[1] != ".txt"): ##filter out non ".txt" file.
            continue
        fn = os.path.splitext(os.path.basename(file))[0]
        tokens = fn.split('-')
        ctr = 0;
        while (ctr < len(tokens)):
            map[tokens[ctr]] = tokens[ctr+1];
            ctr += 2;
        for key in map:
            if (key == "S1R" and map[key] not in S1R_LIST):
                S1R_LIST.append(map[key])
            if (key == "P2PR" and map[key] not in P2PR_LIST ):
                P2PR_LIST.append(map[key])
            if (key == "Ue" and map[key] not in UE_LIST ):
                UE_LIST.append(map[key])
            if (key == "interval" and map[key] not in INTERVAL_LIST):
                INTERVAL_LIST.append(map[key])
            if (key == "pacS" and map[key] not in PACS_LIST):
                PACS_LIST.append(map[key])
            if (key == "pacN" and map[key] not in PACN_LIST):
                PACN_LIST.append(map[key])
                
        
###Main function.
if __name__ == '__main__':
    get_experiment_value()
    if (len (sys.argv) > 1):
        INPUT_DIR = sys.argv[1]
        OUTPUT_DIR = sys.argv[2]
        para = sys.argv[3]
    
    print ("***Extracting from .... "+INPUT_DIR)
    print ("***Data writing into .... "+OUTPUT_DIR)
    print ("***Argument = "+para)
    s1r = 1000
    p2pr = 1000
    ue = 1
    itv = 0
    ps = 900
    pn = 1000000
    
    print ("***Para = "+para)
    
    set_target_map(s1r, p2pr, ue, itv, ps, pn)
    target_map[para] = 0        #set the parameter to be investigated to 0.
    fh.output_file = OUTPUT_DIR+para+"-S1R"+str(s1r)+"-P2PR"+str(p2pr)+"-Ue"+str(ue)+"-interval"+str(itv)+"-pacS"+str(ps)+"-pacN"+str(pn)+".txt"
    print ("***Out = "+fh.output_file)
    fh.clear_output_file()
#                    if (parse_dir() == -1): ###remove empty file
#                        os.remove(fh.output_file)
    parse_dir()
                    
#    if (para == "pacS"):
#        for ue in UE_LIST:
#            for itv in INTERVAL_LIST:
#                for pn in PACN_LIST:
#                    set_target_map(s1r, p2pr, ue, itv, ps, pn)
#                    target_map[para] = 0        #set the parameter to be investigated to 0.
#                    fh.output_file = OUTPUT_DIR+para+"-S1R"+str(s1r)+"-P2PR"+str(p2pr)+"-Ue"+str(ue)+"-interval"+str(itv)+"-pacS"+str(ps)+"-pacN"+str(pn)+".txt"
##                    print ("****Out = "+fh.output_file)
#                    fh.clear_output_file()
#                    if (parse_dir() == -1): ###remove empty file
#                        os.remove(fh.output_file)
#
#    if (para == "pacN"):
#        for ue in UE_LIST:
#            for itv in INTERVAL_LIST:
#                for ps in PACS_LIST:
#                    set_target_map(s1r, p2pr, ue, itv, ps, pn)
#                    target_map[para] = 0        #set the parameter to be investigated to 0.
#                    fh.output_file = OUTPUT_DIR+para+"-S1R"+str(s1r)+"-P2PR"+str(p2pr)+"-Ue"+str(ue)+"-interval"+str(itv)+"-pacS"+str(ps)+"-pacN"+str(pn)+".txt"
##                    print ("****Out = "+fh.output_file)
#                    fh.clear_output_file()
#                    if (parse_dir() == -1): ###remove empty file
#                        os.remove(fh.output_file)
#                    