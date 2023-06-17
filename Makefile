CC = gcc
CFLAGS = -Wall -Wextra -std=c11
LDLIBS = -lm -lSDL2
LDFLAGS = -L./library -I./include
DEFINES = -DDEBUG

SRCDIR = code
OBJDIR := $(shell [ -d obj ] || mkdir obj && echo "obj")
SRC=$(notdir $(wildcard $(SRCDIR)/*.c))

.PHONY: all clean

all: Catan

Catan: $(patsubst %.c, $(OBJDIR)/%.o, $(SRC))
	$(CC) $(CFLAGS) $(filter %.o, $^) -o $@ $(LDLIBS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(OBJDIR)/%.d
	$(CC) $(CFLAGS) $(DEFINES) -c -o $@ $<

$(OBJDIR)/%.d: $(SRCDIR)/%.c
	$(CC) -MT $(@:.d=.o) -MM -MP -o $@ $(CFLAGS) $<

.PRECIOUS: $(OBJDIR)/%.d
-include $(patsubst %.c, $(OBJDIR)/%.d, $(SRC))

clean:
	rm -rf Catan $(OBJDIR)
