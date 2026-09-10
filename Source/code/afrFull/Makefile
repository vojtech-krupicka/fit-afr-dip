# **************************************************************************** #
# @project	DIP 2012 - AFR - Adaptive Frameless Rendering					   #
# @author   Vojtech Krupicka, 	xkrupi06									   #
# @date 	01.12.2011														   #
# @note		http://www.stud.fit.vutbr.cz/~xkrupi06/DIP/ 					   #
# @note		ISO-8859-2														   #
# @location	./src/scr/														   #
# @version	v1.0a                                                       	   #
# **************************************************************************** #

# -------------------------------------------------------------------------
# Init auxiliary variables for compilation
# -------------------------------------------------------------------------

# Make program name
ifeq ($(OS), Windows_NT)
  MAKE=mingw32-make
else
  MAKE := make
endif

# C++ compiler
CXX := g++

# Stardard flags for C++ compiler
CXXFLAGS := 

# Standard flags for preprocessor
CPPFLAGS := 

# Standard flags for linker
ifeq ($(OS), Windows_NT)
  LDFLAGS=-lopengl32 -lglu32 -lglut32 #-mwindows
else
  LDFLAGS := -lGL -lGLU -lglut -lXmu -lX11 -lXi -L/usr/X11R6/lib 
endif 


# All flags
FLAGS =  -I.  $(CPPFLAGS) $(CXXFLAGS)

# All objects
OBJECTS = \
		./afr.o \
		./application/application.o ./application/configurator.o ./application/glcontroller.o ./application/sampler.o ./application/tilemanager.o \
		./exceptions/exception.o ./exceptions/exceptions.o \
		./materials/material.o ./materials/image_texture.o \
		./primitives/animated_triangle.o ./primitives/bbox.o ./primitives/cone.o ./primitives/cylinder.o ./primitives/polygon.o ./primitives/primitive.o ./primitives/sphere.o ./primitives/triangle.o \
		./raytracer/raytracer.o ./raytracer/kdtreenode.o ./raytracer/kdtree.o \
		./scene/animation.o ./scene/animation_manager.o ./scene/scene.o ./scene/scene_node.o \
		./types/color_rgb.o ./types/image.o ./types/matrix.o ./types/quaternion.o ./types/ray.o ./types/sample_buffer.o ./types/tile.o ./types/vector2.o ./types/vector3.o ./types/vector4.o \
		./utils/aff_parser.o ./utils/position_info.o ./utils/rotation_info.o	

# -------------------------------------------------------------------------
# Direktives for compiling
# -------------------------------------------------------------------------


# Install and uninstall
all: application.o exceptions.o materials.o primitives.o raytracer.o scene.o types.o utils.o ../../bin/afrFull

install: all

uninstall: clean

clean: 
	$(MAKE) clean -C application
	$(MAKE) clean -C exceptions
	$(MAKE) clean -C materials
	$(MAKE) clean -C primitives
	$(MAKE) clean -C raytracer
	$(MAKE) clean -C scene
	$(MAKE) clean -C types
	$(MAKE) clean -C utils
	rm -f ./*.o
	rm -f ./*.d

# Create binary file
../../bin/afrFull: $(OBJECTS)
	$(CXX) -o $@ $(OBJECTS) $(LDFLAGS)

# Create object files	
afr.o: ./afr.cpp
	$(CXX) -c -o $@ $(FLAGS) $<

application.o:
	$(MAKE) -C application

exceptions.o:
	$(MAKE) -C exceptions
	
materials.o:
	$(MAKE) -C materials
	
primitives.o:
	$(MAKE) -C primitives
	
raytracer.o:
	$(MAKE) -C raytracer
	
scene.o:
	$(MAKE) -C scene
	
types.o:
	$(MAKE) -C types
	
utils.o:
	$(MAKE) -C utils	

.PHONY: all install uninstall clean

# end of Makefile
