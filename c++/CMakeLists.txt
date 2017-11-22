PROJECT(nome_projeto)

CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
if(COMMAND cmake_policy)
      cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

FIND_PACKAGE( OpenCV REQUIRED )

#INCLUDE_DIRECTORIES("/opt/opencv-2.4.13.3/release/include")

#LINK_DIRECTORIES("/opt/opencv-2.4.13.3/release/lib")

#add_library(LIBS opt/opencv-2.4.13.3/release/lib)

# Declare the target (an executable)
ADD_EXECUTABLE(nome_projeto arquivocpp.cpp)

TARGET_LINK_LIBRARIES(nome_projeto ${OpenCV_LIBS})

SET(GCC_COVETAGE_COMPILE_FLAGS "-std=c++11")
ADD_DEFINITIONS(${GCC_COVERAGE_COMPILE_FLAGS})

#MESSAGE(STATUS "OpenCV_LIBS: ${OpenCV_LIBS}")

