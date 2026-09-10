# **************************************************************************** #
# @project	DIP 2012 - AFR - Adaptive Frameless Rendering					   #
# @author   Vojtech Krupicka, 	xkrupi06									   #
# @date 	01.12.2011														   #
# @note		http://www.stud.fit.vutbr.cz/~xkrupi06/DIP/ 					   #
# @note		ISO-8859-2														   #
# @version	v1.0a                                                       	   #
# **************************************************************************** #


# Make program name
ifeq ($(OS), Windows_NT)
  MAKE=mingw32-make
else
  MAKE := make
endif


# -------------------------------------------------------------------------
# Direktives for compiling
# -------------------------------------------------------------------------


# Install
all: full demo sim

# Install all
install: all

# Uninstall applications (clean all subprojects and remove binaries and docs)
uninstall: clean
	rm -f ./bin/*.exe
	rm -rf ./trunk/*
	
# Clean all subprojects
clean: 
	$(MAKE) clean -C ./code/afrFull
	$(MAKE) clean -C ./code/afrDemo
	$(MAKE) clean -C ./code/afrSim
	
# Make full HassaAFR
full: 
	$(MAKE) -C ./code/afrFull

# Make HassaAFR Demo
demo: 
	$(MAKE) -C ./code/afrDemo

# Make HassaAFR Simulator
sim: 
	$(MAKE) -C ./code/afrSim
	
# Make documentation via Doxygen
docs: 
	doxygen Doxyfile

.PHONY: all install uninstall clean

# end of Makefile
