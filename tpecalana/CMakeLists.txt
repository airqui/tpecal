SET(name "tpecalana")
SET(sourcefiles src/global.cpp src/Channel.cpp src/Chip.cpp src/Dif.cpp src/Mapping.cpp src/simple_file_parser.cpp src/ASU.cpp src/ChipBuffer.cpp src/EventManager.cpp src/ExperimentalSetup.cpp src/RunManager.cpp src/FitHistograms.cpp src/ADCManager.cpp src/AnaManager.cpp src/main.cpp ) 
SET(ext_inc_paths "./include" )

INCLUDE(../cmake/FindROOT.cmake)

LINK_DIRECTORIES( ${ROOT_LIBRARY_DIR})
INCLUDE_DIRECTORIES( ${ext_inc_paths} ${ROOT_INCLUDE_DIR})
ADD_EXECUTABLE(${name} ${sourcefiles})
TARGET_LINK_LIBRARIES(${name} ${ROOT_LIBRARIES} -lSpectrum)

INSTALL(TARGETS ${name}
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib)