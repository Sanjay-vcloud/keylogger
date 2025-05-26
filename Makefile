CC := gcc
CFLAGS := -Wall -Wextra

SRCDIR := src
OBJDIR := obj
INCDIR := include

OBJS := $(OBJDIR)/main.o $(OBJDIR)/logger.o $(OBJDIR)/daemon.o

trace: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

$(OBJDIR)/main.o : $(SRCDIR)/main.c 
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/logger.o : $(SRCDIR)/logger.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/daemon.o : $(SRCDIR)/daemon.c
	$(CC) $(CFLAGS) -c $< -o $@

print:
	@echo Source files: $(SRCDIR)/*.c
	@echo "Object files: $(OBJS)"
	@echo "Executable: trace"
	@echo pwd: $(shell pwd)
	@echo "Include directory: $(INCDIR)"
	@echo "Object directory: $(OBJDIR)"
	@echo "Source directory: $(SRCDIR)"
	@echo "Compiler: $(CC)"
	@echo "Compiler flags: $(CFLAGS)"
all: trace
	@echo "Build complete. Run 'make run' to execute the program."

.PHONY: all clean print

run: 
	sudo ./trace
clean:
	rm -f $(OBJS) trace
	rm -f *.out
	rm -f *.o