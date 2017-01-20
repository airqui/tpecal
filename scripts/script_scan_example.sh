#!/bin/bash                                                                                                                                                                    
FOLDER_INPUT="/airqui/LAL/testbench_nov2016/rawdata/Scurves_FEV8_COB_HG_8channels_0"
FOLDER_OUTPUT="/airqui/LAL/testbench_nov2016/rawdata/output_tests_HG_8C_0/"
mkdir "/home/"$FOLDER_OUTPUT

#./../bin/tpecalana  $FOLDER_INPUT $FOLDER_OUTPUT PlaneEventsScan 1 0
#./../tpecalana  $FOLDER_INPUT $FOLDER_OUTPUT scurves 1 0 0 
#./../tpecalana  $FOLDER_INPUT $FOLDER_OUTPUT scurves 1 0 1

FOLDER_INPUT="/airqui/LAL/testbench_nov2016/rawdata/Scurves_FEV8_COB_HG_8channels_1"
FOLDER_OUTPUT="/airqui/LAL/testbench_nov2016/rawdata/output_tests_HG_8C_1/"
mkdir "/home/"$FOLDER_OUTPUT

xterm -e "./../tpecalana  $FOLDER_INPUT $FOLDER_OUTPUT PlaneEventsScan 1 0" &
xterm -e "./../tpecalana  $FOLDER_INPUT $FOLDER_OUTPUT scurves 1 0 0" &
xterm -e "./../tpecalana  $FOLDER_INPUT $FOLDER_OUTPUT scurves 1 0 1" &
