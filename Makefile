CC = gcc
SRCDIR = src
OBJDIR = obj
BINDIR = bin
INCLUDEDIR = include
CFLAGS = -Wall -Wextra -I./$(SRCDIR) -I ./$(INCLUDEDIR)
SOURCES = $(wildcard $(SRCDIR)/*.c)
OBJECTS = $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
INCLUDES = $(wildcard $(INCLUDEDIR)/*.h)

.Phony: all clean
all: directories chell

chell: $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o $(BINDIR)/$@

$(OBJDIR)/%.o : $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

directories: $(OBJDIR) $(BINDIR) $(SRCDIR)

$(SRCDIR):
	mkdir -p $(SRCDIR)

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(BINDIR):
	mkdir -p $(BINDIR)

.Phony: clean
clean:
	rm -f $(OBJDIR)/*.o $(BINDIR)/*
