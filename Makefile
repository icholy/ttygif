CC = gcc
VERSION = '"1.4.0"'
CFLAGS = -O2 -Wall -DVERSION=$(VERSION)
PREFIX ?= /usr/local

UNAME := $(shell uname)
ifeq ($(UNAME), Darwin)
	CFLAGS += -DOS_DARWIN
endif
ifeq ($(UNAME), Linux)
	CFLAGS += -DOS_LINUX
endif

all: ttygif

ttygif: ttygif.o io.o string_builder.o utils.o
	$(CC) $(CFLAGS) -o ttygif ttygif.o io.o string_builder.o utils.o

install: ttygif
	install -d $(PREFIX)/bin
	install -m755 ttygif $(PREFIX)/bin/ttygif

clean:
	rm -f *.o ttygif ttyrecord *~
