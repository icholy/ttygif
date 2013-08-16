CC = gcc
CFLAGS = -O2
VERSION = 1.0.8
PREFIX = /usr
TARGET = ttyexec

all: $(TARGET)

ttyexec: ttyexec.o io.o util.o
	$(CC) $(CFLAGS) -o ttyexec ttyexec.o io.o util.o

clean:
	rm -f *.o $(TARGET) ttyrecord *~

install: ttygif
	install -pDm0755 ttyexec $(DESTDIR)$(PREFIX)/bin/ttyexec
