
SRCPATHS = \
	src/*.c \

SRC = $(wildcard $(SRCPATHS))
OBJECTS = $(SRC:src/%.c=build/%.o)

ECHO = echo

CC = gcc
LD = gcc

INCLUDEPATHS = -Isrc/
ASANFLAGS = -fsanitize=undefined -fsanitize=address
CFLAGS = -std=gnu2x -g -fwrapv -fno-strict-aliasing -march=native
WARNINGS = -Wall -Wimplicit-fallthrough -Wno-deprecated-declarations -Wno-enum-compare -Wno-unused -Wno-format -Wno-enum-conversion -Wincompatible-pointer-types -Wno-discarded-qualifiers -Wno-strict-aliasing
ALLFLAGS = $(CFLAGS) $(WARNINGS)
OPT = -Ofast -flto 

ifneq ($(OS),Windows_NT)
	CFLAGS += -rdynamic
	ECHO = /usr/bin/echo
	# JANK FIX FOR SANDWICH'S DUMB ECHO ON HIS LINUX MACHINE
endif

FILE_NUM = 0
build/%.o: src/%.c
	$(eval FILE_NUM=$(shell echo $$(($(FILE_NUM)+1))))
	$(shell $(ECHO) 1>&2 -e "\e[0m[\e[32m$(FILE_NUM)/$(words $(SRC))\e[0m]\t Compiling \e[1m$<\e[0m")
	
	@$(CC) -c -o $@ $< -MD $(INCLUDEPATHS) $(ALLFLAGS) $(OPT)

c4: $(OBJECTS)
	@$(LD) $(OBJECTS) -o dnmt $(ALLFLAGS) -lm

.PHONY: clean
clean:
	@rm -rf build/
	@mkdir build/
	@mkdir -p $(dir $(OBJECTS))

-include $(OBJECTS:.o=.d)