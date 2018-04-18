# Remove directory
RM := rm -rf

# Language to use
LANG = g++

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

