#!/bin/bash                                                                                                                                                                    
FOLDER_INPUT="/home/irles/WorkArea/TestBench/2016/rawdata/daq_tests/20161215_123714/scurves_by8_masknoisy/"
FOLDER_OUTPUT="/home/irles/WorkArea/TestBench/2017/results/20161215_123714/by8_masknoisy/"
#mkdir $FOLDER_OUTPUT
#./../bin/tpecalana  $FOLDER_INPUT $FOLDER_OUTPUT PlaneEventsScan 16 8

xterm -e "time ./../bin/tpecalana  $FOLDER_INPUT $FOLDER_OUTPUT PlaneEventsScan 16 8" &
xterm -e "time ./../bin/tpecalana  $FOLDER_INPUT $FOLDER_OUTPUT scurves 16 8 0" &
xterm -e "time ./../bin/tpecalana  $FOLDER_INPUT $FOLDER_OUTPUT scurves 16 8 1" &
xterm -e "time ./../bin/tpecalana  $FOLDER_INPUT $FOLDER_OUTPUT scurves 16 8 2" 

