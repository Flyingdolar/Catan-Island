CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -fsanitize=address
LDLIBS = -lm -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer
LDFLAGS = -L./library -I./include
DEFINES = -DDEBUG -g

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
