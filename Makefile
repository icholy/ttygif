CC = gcc
CFLAGS = -O2
VERSION = 1.0.8

TARGET = ttygif

DIST =	ttygif.c ttyrec.h util.c util.h io.c io.h README Makefile 

all: $(TARGET)

ttygif: ttygif.o io.o util.o
	$(CC) $(CFLAGS) -o ttygif ttygif.o io.o util.o
	
clean:
	rm -f *.o $(TARGET) ttyrecord *~

dist:
	rm -rf ttyrec-$(VERSION)
	rm -f ttyrec-$(VERSION).tar.gz

	mkdir ttyrec-$(VERSION)
	cp $(DIST) ttyrec-$(VERSION)
	tar zcf ttyrec-$(VERSION).tar.gz  ttyrec-$(VERSION)
	rm -rf ttyrec-$(VERSION)
