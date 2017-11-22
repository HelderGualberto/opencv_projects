PROJECT(deformation_analisys_point)

CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
if(COMMAND cmake_policy)
      cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

FIND_PACKAGE( OpenCV REQUIRED )

# Declare the target (an executable)
ADD_EXECUTABLE(deformation_analisys_point deformation_analisys_point.cpp)

TARGET_LINK_LIBRARIES(deformation_analisys_point ${OpenCV_LIBS})

SET(GCC_COVETAGE_COMPILE_FLAGS "-std=c++11")
ADD_DEFINITIONS(${GCC_COVERAGE_COMPILE_FLAGS})

#MESSAGE(STATUS "OpenCV_LIBS: ${OpenCV_LIBS}")

