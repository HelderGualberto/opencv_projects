#Arquivo CmakeLists.txt
cmake_minimum_required(VERSION 2.8)
project( backdetect )
find_package( OpenCV REQUIRED )
ADD_EXECUTABLE(backdetect backdetect.cpp)
target_link_libraries( backdetect ${OpenCV_LIBS} )

SET(GCC_COVETAGE_COMPILE_FLAGS "-std=c++11")
ADD_DEFINITIONS(${GCC_COVERAGE_COMPILE_FLAGS})


