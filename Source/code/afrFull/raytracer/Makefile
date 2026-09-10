# **************************************************************************** #
# @project	DIP 2012 - AFR - Adaptive Frameless Rendering					   #
# @author   Vojtech Krupicka, 	xkrupi06									   #
# @date 	01.12.2011														   #
# @note		http://www.stud.fit.vutbr.cz/~xkrupi06/DIP/ 					   #
# @note		ISO-8859-2														   #
# @location	./src/raytracer/												   #
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
all: raytracer.o kdtreenode.o kdtree.o

install: all

uninstall: clean

clean: 
	rm -f ./*.o
	rm -f ./*.d

# Create object files
raytracer.o: ./Raytracer.cpp
	$(CXX) -c -o $@ $(FLAGS) $<

kdtreenode.o: ./KdTreeNode.cpp
	$(CXX) -c -o $@ $(FLAGS) $<
	
kdtree.o: ./KdTree.cpp
	$(CXX) -c -o $@ $(FLAGS) $<

.PHONY: all install uninstall clean

# end of Makefile
