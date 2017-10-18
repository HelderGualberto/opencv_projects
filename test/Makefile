

CPP = g++

# OpenCV trunk
#CPPFLAGS = -L/opt/opencv-3.3.0/build/lib \
	   -I/opt/opencv-3.3.0/build/include

# Opencv 2.4.8
#CPPFLAGS = -L/home/krystof/libs/opencv-2.4.8/release/installed/libs \
	   -I/home/krystof/libs/opencv-2.4.8/release/installed/include

all: test

test: main.cpp
	$(CPP) $(CPPFLAGS) $^ -o $@
