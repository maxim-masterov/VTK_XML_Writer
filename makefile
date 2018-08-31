# Remove directory
RM := rm -rf

# Compilation type
type = gcc
# Language to use
LANG = g++

ifeq ($(type),$(filter $(type), gcc mpi_gcc))
LANG = g++
else ifeq ($(type),clang)
LANG = clang
else ifeq ($(type),$(filter $(type), intel mpi_intel))
LANG=intel
else
$(info )
$(info *******************************************************************************)
$(info *****************  Unknown compilation type: $(type))
$(info *****************  Project will be compiled with the default compiler: $(LANG))
$(info *******************************************************************************)
$(info )
endif

# Flags
FLAGS = -std=c++0x -O3 -Wall -c -fmessage-length=0 -DNDEBUG

INCL = -I../FancyBear/FancyBear/

SYMBOLS = -D__GXX_EXPERIMENTAL_CXX0X__ -D__cplusplus=201103L

# Library name
LIBNAME = libxmlwriter.a

# List of source files
SRCS = \
	src/XMLWriter.cpp

# Directory for object files
OBJDIR = ./obj

# Object files
objects = $(patsubst %.cpp,obj/%.o,$(SRCS))

# All Target
all: $(objects) makelib


$(objects): | obj

# Create necessary directories for object files
# and print some information
obj:
	@mkdir -p $(OBJDIR)
	@mkdir -p $(OBJDIR)/src
	@echo ' '
	@echo 'Welcome to XMLWriter!' 
	@echo ' '
	@echo 'Building options:'
	@echo -e '   Compiler:\t$(LANG)'
	@echo -e '   Flags:\t$(FLAGS)'
	@echo -e '   Symbols:\t$(SYMBOLS)'
	@echo -e '   Output name:\t$(LIBNAME)'
	@echo ' '

# Build object files and print out their names
$(OBJDIR)/%.o: %.cpp
	@echo 'Building file: $<'
	@$(LANG) $(FLAGS) $(INCL) $(SYMBOLS) -c $< -o $@

# Create lib file
makelib: $(objects)
	ar cr $(LIBNAME) $(objects) $(LIBS)

# Other targets
clean:
	-$(RM) $(OBJDIR) $(LIBNAME)

.PHONY: all clean
.SECONDARY:

