CC = gcc
CFLAGS = -O2
VERSION = 1.0.8

UNAME := $(shell uname)
ifeq ($(UNAME),Darwin)
	TARGET = osx
else
	TARGET = ttygif
endif

all: $(TARGET)

ttygif: ttygif.o io.o 
	$(CC) $(CFLAGS) -o ttygif ttygif.o io.o 

osx: ttygif_osx.o io.o
	$(CC) $(CFLAGS) -o ttygif ttygif_osx.o io.o
	
clean:
	rm -f *.o $(TARGET) ttyrecord *~
