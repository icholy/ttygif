CC = gcc
CFLAGS = -O2
VERSION = 1.0.8

TARGET = ttygif

all: $(TARGET)

ttygif: ttygif.o io.o 
	$(CC) $(CFLAGS) -o ttygif ttygif.o io.o 
	
clean:
	rm -f *.o $(TARGET) ttyrecord *~
