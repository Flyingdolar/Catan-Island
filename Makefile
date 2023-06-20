CC = gcc
CFLAGS =  -std=c11
LDLIBS = -lm -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer
LDFLAGS = -L./library -I./include
DEFINES = -g

SRCDIR = code
OBJDIR := $(shell [ -d obj ] || mkdir obj && echo "obj")
SRC=$(notdir $(wildcard $(SRCDIR)/*.c))

.PHONY: all clean

all: Catan

run:
	./Catan 2> /dev/null

configure: sdl sdl_image sdl_ttf sdl_mixer

sdl:
	apt-get install libsdl2-dev

sdl_image:
	apt-get install libsdl2-image-dev

sdl_ttf:
	apt-get install libsdl2-ttf-dev

sdl_mixer:
	apt-get install libsdl2-mixer-dev

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
