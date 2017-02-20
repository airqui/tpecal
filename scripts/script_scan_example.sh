#!/bin/bash      

source script1.sh &

# ----------------------------------------------------------
FOLDER_INPUT="/home/irles/llr_testbench_data/sk2/20170202_165616/scurves_by1/"
FOLDER_OUTPUT= "/home/irles/llr_testbench_data/sk2/20170202_165616/scurves_by1/results/"

./../bin/tpecalana  $FOLDER_INPUT $FOLDER_OUTPUT scurves 1 0 15 1245 
./../bin/tpecalana  $FOLDER_INPUT $FOLDER_OUTPUT scurves 1 0 15 1246 &
./../bin/tpecalana  $FOLDER_INPUT $FOLDER_OUTPUT scurves 1 0 15 1247 
./../bin/tpecalana  $FOLDER_INPUT $FOLDER_OUTPUT scurves 1 0 15 1250 &
./../bin/tpecalana  $FOLDER_INPUT $FOLDER_OUTPUT scurves 1 0 15 1255 &
./../bin/tpecalana  $FOLDER_INPUT $FOLDER_OUTPUT scurves 1 0 15 1260 

# ----------------------------------------------------------
FOLDER_INPUT="/home/irles/llr_testbench_data/sk2/20170202_165616/scurves_by8/"
FOLDER_OUTPUT=" /home/irles/llr_testbench_data/sk2/20170202_165616/scurves_by8/results/"

./../bin/tpecalana  $FOLDER_INPUT $FOLDER_OUTPUT scurves 1 0 15 1245
./../bin/tpecalana  $FOLDER_INPUT $FOLDER_OUTPUT scurves 1 0 15 1246 &
./../bin/tpecalana  $FOLDER_INPUT $FOLDER_OUTPUT scurves 1 0 15 1247 
./../bin/tpecalana  $FOLDER_INPUT $FOLDER_OUTPUT scurves 1 0 15 1250 &
./../bin/tpecalana  $FOLDER_INPUT $FOLDER_OUTPUT scurves 1 0 15 1255 &
./../bin/tpecalana  $FOLDER_INPUT $FOLDER_OUTPUT scurves 1 0 15 1260 


FOLDER_INPUT="/home/irles/llr_testbench_data/sk2a/201701/"
FOLDER_OUTPUT="/home/irles/llr_testbench_data/sk2a/201701/results/"

./../bin/tpecalana  $FOLDER_INPUT $FOLDER_OUTPUT scurves 1 0 15 1245 
./../bin/tpecalana  $FOLDER_INPUT $FOLDER_OUTPUT scurves 1 0 15 1246 &
./../bin/tpecalana  $FOLDER_INPUT $FOLDER_OUTPUT scurves 1 0 15 1247 
./../bin/tpecalana  $FOLDER_INPUT $FOLDER_OUTPUT scurves 1 0 15 1250 &
./../bin/tpecalana  $FOLDER_INPUT $FOLDER_OUTPUT scurves 1 0 15 1255 &
./../bin/tpecalana  $FOLDER_INPUT $FOLDER_OUTPUT scurves 1 0 15 1260 
