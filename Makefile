#Binary Building tool

#declare Phony, so in case a file has the same name
#the phony will still be run
.PHONY = all clean

#declare all variables
CC = gcc
#object dir
ODIR=obj
#header dir
IDIR=.
##IDIR=../includes/
#eclipse Debug dir
DDIR=Debug

#declare Sources and Binaries
SRCS := $(wildcard *.c)

BINS := $(SRCS:%.c=%)

_OBJF := $(SRCS:%.c=%.o)
OBJF  := $(patsubst %,$(ODIR)/%,$(_OBJF))

_HEADERS := $(SRCS:%.c=%.h)
HEADERS := $(patsubst %,$(IDIR)/%,$(_HEADERS))


TARGET := hangman

#declare all Compilation and Linkerflags:
CFLAGS= -I$(IDIR) -g -std=c99 -Wall -Wextra -pedantic -Wno-unused-parameter
##

#libraries
LFLAGS= -lm

#define the "all" target
all: clean SHOW_README $(OBJF) $(TARGET) RUN
##

#target <- objectfiles
$(TARGET): $(OBJF)
	@$(CC) $(CFLAGS) -o $@ $^ $(LFLAGS)


$(ODIR)/%.o: %.c | obj
	@$(CC) $(CFLAGS) -c -o $@ $^ 

RUN: 
	@./$(TARGET)

obj:
	@mkdir -p $@

# --leak-check=full: "each individual leak will be shown in detail"
# --track-origins=yes: Favor useful output over speed. This tracks the origins of uninitialized values
# --verbose: Can tell you about unusual behavior of your program. Repeat for more verbosity.
memleak_check: $(SRCS)
	$(CC) $(CFLAGS) -Dmemleak=1 -o $(TARGET) $^ $(LFLAGS)
	valgrind --leak-check=full  --tool=memcheck ./$(TARGET)

#clean target
clean:
	@echo Cleaning Up
	@rm -rvf $(ODIR)/*.o *.exe $(BINS) $(ODIR) .*swo .*swp

#clean debug folder
cldebug:
	@echo Removing Debug Folder
	@rm -rvf $(DDIR)
	
#README
SHOW_README:
	@echo "*** START README ***"
	@cat README.md
	@echo "***  END README  ***"