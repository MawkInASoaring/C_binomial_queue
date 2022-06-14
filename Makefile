

TARGETS = utest_pqext
INCDIR1 = . 
INCDIR2 = /usr/include/
OBJDIR = obj
CPPINCFILES1 = $(wildcard $(INCDIR1)/*.h)
CC=gcc
#LIBS= -lgsl -lgslcblas -lm
LDFLAGS= /usr/lib/x86_64-linux-gnu/
all: $(TARGETS)

# Target object files
SRCDIR = .
SRC = $(wildcard $(SRCDIR)/*.c)
OBJ := $(addprefix $(OBJDIR)/,$(notdir $(SRC:.c=.o)))

#options
GPPOPT = -O0 -Wall

#options
DEBUG = -g
LISTFILE_CPP = -Wa,-aghl=$(OBJDIR)/$(basename $<).lst -save-temps=$(OBJDIR)

# Create directory for object and temp files
MKOBJDIR := $(shell mkdir -p $(OBJDIR))

#Auto Build rules : frocefully recompile all object...
$(TARGETS): %: $(OBJDIR)/%.o $(OBJDIR)/%.adep $(OBJ)
	@set -e;
#	echo "linking";
	$(CC) -L$(LDFLAGS) $< $(shell cat $(OBJDIR)/*.adep | awk '!/$*.o/ || seen { print } /$*.o/ && !seen { seen = 1 }') $(LIBS) -o $@

# Note:

MAKEDEPEND = $(CC) -MM $< | grep -oh "\w*\.h"  | sed 's,\([ ]*\)\.h,\1.c ,g'  | grep -oh "\w*\.c"  \
			| xargs -I% find . -type f -maxdepth 1 -name "%" |  sed 's|^./||' | sed 's,\([ ]*\)\.c,\1.o ,g' | sed 's/^/obj\//' > $@ 
				
#
	
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@set -e; \
	echo "compile obj" $@	
	$(CC) $(DEBUG) $(LISTFILE_CPP) $(GPPOPT) -I$(INCDIR1) -I$(INCDIR1) -c $< -o $@
				
$(OBJDIR)/%.adep: $(SRCDIR)/%.c
	@set -e; \
	rm -f $@; \
	echo "hello dep"
	@$(MAKEDEPEND)
	cat $(@)

$(OBJS): %: $(SRCDIR)/%.c
	@set -e; \
	echo "compile objs" $@	
	$(CC) $(DEBUG) $(LISTFILE_CPP) $(GPPOPT) -I$(INCDIR1) -c $< -o $@

.PHONY: all	clean

clean:
	rm -f $(SRCDIR)/*.d* $(SRCDIR)/*.o
	rm -f $(TARGETS)
	rm -rf $(OBJDIR)
