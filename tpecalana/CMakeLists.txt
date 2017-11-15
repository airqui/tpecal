SET(name "tpecalana")
SET(sourcefiles src/global.cpp src/Channel.cpp src/Chip.cpp src/Dif.cpp src/Mapping.cpp src/simple_file_parser.cpp src/ASU.cpp src/ChipBuffer.cpp src/EventManager.cpp src/ExperimentalSetup.cpp src/RunManager.cpp src/FitHistograms.cpp src/FitGraphs.cpp src/ScanManager.cpp src/main.cpp ) 
SET(ext_inc_paths "./include" )

INCLUDE(../cmake/FindROOT.cmake)

find_package(Boost REQUIRED COMPONENTS system filesystem)
LINK_DIRECTORIES( ${Boost_LIBRARY_DIRS} ${ROOT_LIBRARY_DIR})
INCLUDE_DIRECTORIES( ${ext_inc_paths} ${Boost_INCLUDE_DIRS} ${ROOT_INCLUDE_DIR})
ADD_EXECUTABLE(${name} ${sourcefiles})
TARGET_LINK_LIBRARIES(${name} ${Boost_LIBRARIES} ${ROOT_LIBRARIES} -lSpectrum)

INSTALL(TARGETS ${name}
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib)