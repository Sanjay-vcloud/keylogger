CC := gcc
CFLAGS := -Wall -Wextra 

SRCDIR := src
OBJDIR := obj
INCDIR := include

OBJS := $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o,$(wildcard $(SRCDIR)/*.c))

trace : $(OBJS)
	$(CC) $(CFLAGS) -o trace $(OBJS) -lcurl

$(OBJDIR)/%.o : $(SRCDIR)/%.c
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

print:
	@echo $(OBJS)
all: trace
	@echo "Build complete. Run 'make run' to execute the program."

.PHONY: all clean print

install:
	@echo "Installing dependencies..."
	sudo apt install libcurl4-openssl-dev
	sudo apt install make
	@echo "Dependencies installed."
run: 
	sudo ./trace
clean:
	rm -f $(OBJS) trace
	rm -f *.out
	rm -f *.o