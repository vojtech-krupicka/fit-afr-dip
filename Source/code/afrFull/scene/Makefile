# **************************************************************************** #
# @project	DIP 2012 - AFR - Adaptive Frameless Rendering					   #
# @author   Vojtech Krupicka, 	xkrupi06									   #
# @date 	01.12.2011														   #
# @note		http://www.stud.fit.vutbr.cz/~xkrupi06/DIP/ 					   #
# @note		ISO-8859-2														   #
# @location	./src/scene/													   #
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
all: animation.o animation_manager.o scene.o scene_node.o

install: all

uninstall: clean

clean: 
	rm -f ./*.o
	rm -f ./*.d

# Create object files
animation.o: ./Animation.cpp
	$(CXX) -c -o $@ $(FLAGS) $<
	
animation_manager.o: ./AnimationManager.cpp
	$(CXX) -c -o $@ $(FLAGS) $<
	
scene.o: ./Scene.cpp
	$(CXX) -c -o $@ $(FLAGS) $<
	
scene_node.o: ./SceneNode.cpp
	$(CXX) -c -o $@ $(FLAGS) $<


.PHONY: all install uninstall clean

# end of Makefile
