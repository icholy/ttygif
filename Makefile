CC = gcc
CFLAGS = -O2
VERSION = 1.0.8

TARGET = ttyrec ttyplay ttytime

DIST =	ttyrec.c ttyplay.c ttyrec.h io.c io.h ttytime.c\
	README Makefile ttyrec.1 ttyplay.1 ttytime.1

all: $(TARGET)

ttyrec: ttyrec.o io.o
	$(CC) $(CFLAGS) -o ttyrec ttyrec.o io.o

ttyplay: ttyplay.o io.o
	$(CC) $(CFLAGS) -o ttyplay ttyplay.o io.o

ttytime: ttytime.o io.o
	$(CC) $(CFLAGS) -o ttytime ttytime.o io.o

clean:
	rm -f *.o $(TARGET) ttyrecord *~

dist:
	rm -rf ttyrec-$(VERSION)
	rm -f ttyrec-$(VERSION).tar.gz

	mkdir ttyrec-$(VERSION)
	cp $(DIST) ttyrec-$(VERSION)
	tar zcf ttyrec-$(VERSION).tar.gz  ttyrec-$(VERSION)
	rm -rf ttyrec-$(VERSION)
