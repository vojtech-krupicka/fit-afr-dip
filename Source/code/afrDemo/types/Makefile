# **************************************************************************** #
# @project	DIP 2012 - AFR - Adaptive Frameless Rendering					   #
# @author   Vojtech Krupicka, 	xkrupi06									   #
# @date 	01.12.2011														   #
# @note		http://www.stud.fit.vutbr.cz/~xkrupi06/DIP/ 					   #
# @note		ISO-8859-2														   #
# @location	./src/types/													   #
# @version	v1.0a                                                       	   #
# **************************************************************************** #

# -------------------------------------------------------------------------
# Init auxiliary variables for compilation
# -------------------------------------------------------------------------

# C++ compiler
CXX := g++

# Stardard flags for C++ compiler
CXXFLAGS := 

# Standard flags for preprocessor
CPPFLAGS := 

# Standard flags for linker
LDFLAGS := 

# All flags
FLAGS =  -I.  $(CPPFLAGS) $(CXXFLAGS)

# -------------------------------------------------------------------------
# Direktives for compiling
# -------------------------------------------------------------------------


# Install and uninstall
all: color_rgb.o image.o matrix.o quaternion.o ray.o sample_buffer.o tile.o vector2.o vector3.o vector4.o

install: all

uninstall: clean

clean: 
	rm -f ./*.o
	rm -f ./*.d

# Create object files
color_rgb.o: ./ColorRGB.cpp
	$(CXX) -c -o $@ $(FLAGS) $<
	
image.o: ./Image.cpp
	$(CXX) -c -o $@ $(FLAGS) $<
	
matrix.o: ./Matrix.cpp
	$(CXX) -c -o $@ $(FLAGS) $<
	
quaternion.o: ./Quaternion.cpp
	$(CXX) -c -o $@ $(FLAGS) $<
	
ray.o: ./Ray.cpp
	$(CXX) -c -o $@ $(FLAGS) $<

sample_buffer.o: ./SampleBuffer.cpp
	$(CXX) -c -o $@ $(FLAGS) $<
	
tile.o: ./Tile.cpp
	$(CXX) -c -o $@ $(FLAGS) $<
	
vector2.o: ./Vector2.cpp
	$(CXX) -c -o $@ $(FLAGS) $<
	
vector3.o: ./Vector3.cpp
	$(CXX) -c -o $@ $(FLAGS) $<
	
vector4.o: ./Vector4.cpp
	$(CXX) -c -o $@ $(FLAGS) $<

.PHONY: all install uninstall clean

# end of Makefile
