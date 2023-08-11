CC := g++ # This is the main compiler
# CC := clang --analyze # and comment out the linker last line for sanity
SRCDIR := src
BUILDDIR := build
BINDIR := bin
TARGET := bin/srtRenderer
 
SRCEXT := cpp
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
CFLAGS := -g -std=c++17 -O3 -Wall -fopenmp `libpng-config --cflags` `sdl2-config --libs --cflags` -lm
LIB := -L lib -fopenmp `libpng-config --ldflags` -lassimp -lSDL2
INC := -I include  



$(TARGET): $(OBJECTS)
	@echo "Linking $(OBJECTS)"
	@mkdir -p $(BINDIR)
	@$(CC) $^ -o $(TARGET) $(LIB)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR) $(BUILDDIR)/models $(BUILDDIR) $(BUILDDIR)/gi $(BUILDDIR)/utils $(BUILDDIR)/raytracer
	@$(CC) $(CFLAGS) $(INC) -c -o $@ $< && echo "[OK] $@"

clean:
	@echo " Cleaning..."; 
	@echo " $(RM) -r $(BUILDDIR) $(BINDIR)"; $(RM) -r $(BUILDDIR) $(BINDIR)

.PHONY: clean

all: $(TARGET)