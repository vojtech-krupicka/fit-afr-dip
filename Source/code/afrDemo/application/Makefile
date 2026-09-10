# **************************************************************************** #
# @project	DIP 2012 - AFR - Adaptive Frameless Rendering					   #
# @author   Vojtech Krupicka, 	xkrupi06									   #
# @date 	24.03.2012														   #
# @note		http://www.stud.fit.vutbr.cz/~xkrupi06/DIP/ 					   #
# @note		ISO-8859-2														   #
# @location	./src/application/												   #
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
LDFLAGS := -lopengl32 -lglu32 -lglut32

# All flags
FLAGS =  -I. $(CPPFLAGS) $(CXXFLAGS)

# -------------------------------------------------------------------------
# Direktives for compiling
# -------------------------------------------------------------------------


# Install and uninstall
all: application.o configurator.o glcontroller.o sampler.o tilemanager.o

install: all

uninstall: clean

clean: 
	rm -f ./*.o
	rm -f ./*.d

# Create object files
application.o: ./Application.cpp
	$(CXX) -c -o $@ $(FLAGS) $<
	
configurator.o: ./Configurator.cpp
	$(CXX) -c -o $@ $(FLAGS) $<
	
glcontroller.o: ./GLController.cpp
	$(CXX) -c -o $@ $(FLAGS) $(LDFLAGS) $<
	
sampler.o: ./Sampler.cpp
	$(CXX) -c -o $@ $(FLAGS) $<
	
tilemanager.o: ./TileManager.cpp
	$(CXX) -c -o $@ $(FLAGS) $<


.PHONY: all install uninstall clean

# end of Makefile
