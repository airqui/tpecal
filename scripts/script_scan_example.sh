#!/bin/bash                                                                                                                                                                    
FOLDER_INPUT="/home/irles/llr_testbench_data/sk2/20170202_165616/scurves_by1/"
FOLDER_OUTPUT="/home/irles/llr_testbench_data/sk2/20170202_165616/scurves_by1/results/"
#mkdir $FOLDER_OUTPUT
#./../bin/tpecalana  $FOLDER_INPUT $FOLDER_OUTPUT PlaneEventsScan 16 8

#xterm -e "time ./../bin/tpecalana  $FOLDER_INPUT $FOLDER_OUTPUT PlaneEventsScan 16 0" &
xterm -e "time ./../bin/tpecalana  $FOLDER_INPUT $FOLDER_OUTPUT scurves 1 0 0" &
xterm -e "time ./../bin/tpecalana  $FOLDER_INPUT $FOLDER_OUTPUT scurves 1 0 1" &
xterm -e "time ./../bin/tpecalana  $FOLDER_INPUT $FOLDER_OUTPUT scurves 1 0 15" 

#xterm -e "time ./../bin/tpecalana  $FOLDER_INPUT $FOLDER_OUTPUT scurves 16 0 2" 

