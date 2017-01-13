# tpecal

Semionline Monitoring and Analaysis Software for the technollogical prototype ECAL for ILC 

R. Poeschl & A. Irles 

Contact:
irles@lal.in2p3.fr
LAL, France, 2016


-------------------------------
Installation requirements:
root (5.34.X),
libboost-devel,
cmake (at least 2.8)

------------------------------
INSTALL:
git clone https://github.com/airqui/tpecal
cd tpecal
mkdir build
cd build
cmake ..
make install

------------------------------
RUN:

- Requires root raw files (produced, for example, with the RAW2ROOT.C macro present in the root_macros folder)
- Requires chip mapping (txt file)

in the bin folder:

-> for scans (threshold scan, holdscan, etc) 
./tpecalana 
    input_folder  == full path last subrun folder, containing converted root files. For No scan runs, this is the 
    output_folder == relative path to the ouptut folder, which should be in input_folder/output_folder. 
    analysis_type == scurves, holdscan" << std::endl;
    enabled_chips == Number of enabled chips per dif (default 16) 
    step          == in case of scans, the number of channels in each subrun (default 8) 
    buffer        == buffer to analyze (for scans) 0-14. If 15, all are analyzed together. (default 0)

-> for single file monitoring or analysis
./tpecalana 
    input_file    == input root file 
    output_folder == output folder (full path) 
    analysis_type == Pedestal, PedestalSignal, MonitorChannel, MonitorChip
    enabled_chips == Number of enabled chips per dif (default 16) 
    value         == free parameter...  