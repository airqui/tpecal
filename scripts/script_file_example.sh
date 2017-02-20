#!/bin/bash                                                                                                                                                                    
# A. Irles, January 2017, 
# 
# Example script to run monitoring and analysis over one file

#FOLDER_INPUT="/home/irles/llr_testbench_data/sk2/20170202_165616/scurves_by8/"
#FOLDER_OUTPUT="/home/irles/llr_testbench_data/sk2/20170202_165616/scurves_by8/results/"

FOLDER_INPUT="/home/irles/WorkArea/TestBench/2016/rawdata/daq_tests/20161215_123714/scurves_by8_masknoisy/"
FOLDER_OUTPUT="/home/irles/WorkArea/TestBench/2016/rawdata/daq_tests/20161215_123714/scurves_by8_masknoisy/output_gaussScurve/"

for i in $(seq 181 3 205)
do
    FILE="scurve_trig${i}_by_dif0.raw"
    
    echo "Input file: " $FOLDER_INPUT
    echo "Output Folder:: " $FOLDER_OUTPUT
    
    if [ -d "$FOLDER_INPUT" ];then
	FULLPATH=$FOLDER_INPUT$FILE".root"
	echo $FULLPATH
	if [ -f "$FULLPATH" ];then  
	    if [ ! -d "$FOLDER_OUTPUT" ];then
		echo "Output directory " $FOLDER_OUTPUT "does not exist, I am creating it"
		mkdir $FOLDER_OUTPUT
	    fi
	    FILE_OUTPUT=$FOLDER_OUTPUT$FILE
	  #  xterm -geometry 93x31+100+10 -e "./../bin/tpecalana  $FULLPATH $FILE_OUTPUT MonitorChannel 16" &
	    xterm -geometry 93x31+700+10 -e  "./../bin/tpecalana  $FULLPATH $FILE_OUTPUT MonitorChip 16" &
	fi
    else
	echo "Input file " $FOLDER_INPUT "does not exist, I am stopping"
    fi
    sleep 20
done
    
