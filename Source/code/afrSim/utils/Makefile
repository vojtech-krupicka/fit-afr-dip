# **************************************************************************** #
# @project	DIP 2012 - AFR - Adaptive Frameless Rendering					   #
# @author   Vojtech Krupicka, 	xkrupi06									   #
# @date 	01.12.2011														   #
# @note		http://www.stud.fit.vutbr.cz/~xkrupi06/DIP/ 					   #
# @note		ISO-8859-2														   #
# @location	./src/utils/													   #
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
all: aff_parser.o position_info.o rotation_info.o

install: all

uninstall: clean

clean: 
	rm -f ./*.o
	rm -f ./*.d

# Create object files
aff_parser.o: ./AffParser.cpp
	$(CXX) -c -o $@ $(FLAGS) $<
	
position_info.o: ./PositionInfo.cpp
	$(CXX) -c -o $@ $(FLAGS) $<
	
rotation_info.o: ./RotationInfo.cpp
	$(CXX) -c -o $@ $(FLAGS) $<

.PHONY: all install uninstall clean

# end of Makefile
