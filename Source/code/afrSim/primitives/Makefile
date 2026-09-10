# **************************************************************************** #
# @project	DIP 2012 - AFR - Adaptive Frameless Rendering					   #
# @author   Vojtech Krupicka, 	xkrupi06									   #
# @date 	01.12.2011														   #
# @note		http://www.stud.fit.vutbr.cz/~xkrupi06/DIP/ 					   #
# @note		ISO-8859-2														   #
# @location	./src/primitives/												   #
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
all: animated_triangle.o bbox.o cone.o cylinder.o polygon.o primitive.o sphere.o triangle.o

install: all

uninstall: clean

clean: 
	rm -f ./*.o
	rm -f ./*.d

# Create object files
animated_triangle.o: ./AnimatedTriangle.cpp
	$(CXX) -c -o $@ $(FLAGS) $<

bbox.o: ./BBox.cpp
	$(CXX) -c -o $@ $(FLAGS) $<
	
cone.o: ./Cone.cpp
	$(CXX) -c -o $@ $(FLAGS) $<
	
cylinder.o: ./Cylinder.cpp
	$(CXX) -c -o $@ $(FLAGS) $<
	
polygon.o: ./Polygon.cpp
	$(CXX) -c -o $@ $(FLAGS) $<
	
primitive.o: ./Primitive.cpp
	$(CXX) -c -o $@ $(FLAGS) $<
	
sphere.o: ./Sphere.cpp
	$(CXX) -c -o $@ $(FLAGS) $<
	
triangle.o: ./Triangle.cpp
	$(CXX) -c -o $@ $(FLAGS) $<


.PHONY: all install uninstall clean

# end of Makefile
