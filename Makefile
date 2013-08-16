CC = gcc
CFLAGS = -O2
VERSION = 1.0.8
PREFIX = /usr
TARGET = ttygif

all: $(TARGET)

ttygif: ttygif.o io.o util.o
	$(CC) $(CFLAGS) -o ttygif ttygif.o io.o util.o
	
clean:
	rm -f *.o $(TARGET) ttyrecord *~

install: ttygif
	install -pDm0755 ttygif $(DESTDIR)$(PREFIX)/bin/ttygif
