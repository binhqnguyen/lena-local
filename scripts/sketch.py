#!/usr/bin/python

import os
import sys

###This class is used to handle (read/write) file/folder.
class FileHandle(object):
    '''
    classdocs
    '''
    input_dir = ""
    output_file = ""
    old_file_path_list = []

    #constructor
    def __init__(self,input_dir, output_file = ""):
        if input_dir is "":
            print("Warning: No input directory explicit! quit now.")
            exit()
        self.input_dir = input_dir
        self.output_file = output_file
        
    #read a file, return the Y-axis values (line 3) of the file
    def __read_single_file__(self, file_name = ""):
        print("***reading file ... " + file_name)
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
        file.close()
        print("return = " + values_str)
        
        return return_value
    
    #write a string to OUTPUT_FILE
    def write_output(self, write_content):
        file = open(self.output_file,"a")
        file.write(write_content)
        file.close()
   
    #read all files of a specified directory, return a list of names of files in the directory.
    def read_dir(self, extension = ""):
#        print ("input dir = " + self.input_dir)
        f_list = os.listdir(self.input_dir)
        if not f_list:
            return self.old_file_path_list
        for file in f_list:
            ###if not match the extension, skip the file
            if (os.path.splitext(file)[1] != extension):
                continue
            file = self.input_dir + "/"+file
            self.old_file_path_list.append(file)
        return self.old_file_path_list
            
    def clear_output_file(self):
        open(self.output_file,"w").close()
        
    ##################
INPUT_DIR = "/Users/binh/Desktop/ns3_play/data"
OUTPUT_DIR = "/Users/binh/Desktop/ns3_play/graph/"
file_path_list = []
cmd = []
title = {1:"PdcpUL",
         2:"PdcpDl",
         3:"RlcUl",
         4:"RlcDl",
         5:"Latency",
         6:"LTELatency"
         }
if __name__ == '__main__':
    if (len (sys.argv) > 1):
        INPUT_DIR = sys.argv[1]
        OUTPUT_DIR = sys.argv[2] 
    
    print ("$$$Reading data from ....." + INPUT_DIR)
    print ("$$$Graph is in ...." + OUTPUT_DIR)
    
    fh = FileHandle(INPUT_DIR)
    plot_bach_tmp_file = "tmp"
    file_path_list = fh.read_dir(".txt");
    fh.output_file = plot_bach_tmp_file
    
    for f in file_path_list:
        ###create a directory for graph
        graph_dir = OUTPUT_DIR+os.path.splitext(os.path.basename(f))[0]
        
        ####graph_dir will present the configuration. 
        ####For example: if the graphs inside the graph_dir are sketching the case when S1R=100, UE=5
        ####Then the graph_dir will have a name S1R-100-UE-5.
        ####And inside the graph_dir folder, we have graphs for each parameter. E.g, interval-S1R-100-UE-5, etc 
        if (os.path.splitext(os.path.basename(f))[0].find("-") != -1):
             graph_dir = OUTPUT_DIR+os.path.splitext(os.path.basename(f))[0][os.path.splitext(os.path.basename(f))[0].index("-")+1:]
        if not os.path.exists(graph_dir):
            os.makedirs(graph_dir)
        plot_str = ""
        """
        for c in range(1,len(title)+1):
            if (c != len(title)):
                 plot_str += "\""+f+"\"" +" using 1:"+str(c+1)+" title \""+title[c]+"\"; " 
            else:
                 plot_str += "\""+f+"\"" +" using 1:"+str(c+1)+" title \""+title[c]+"\" "
        """
        plot_str += "set xlabel \"sending time-interval (ms)\"\n"    ###set graph's x-axis label
        plot_str += "set ylabel \"latency (ms)\"\n"                  ###set graph's y-axis label
        plot_str += "set terminal svg\n"      ###set output format
        plot_str += "set output \""+graph_dir+"/"+os.path.splitext(os.path.basename(f))[0]+".svg\"\n"    ###set output graph's location
        plot_str += "plot "
        plot_str += "\""+f+"\"" +" using 1:6"+" title \"latency - time interval\""
        #write the gnuplot string to file
        fh.clear_output_file()
        fh.write_output(plot_str)
        os.system("gnuplot "+plot_bach_tmp_file)
    