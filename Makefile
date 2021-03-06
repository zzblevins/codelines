#
# Makefile for codelines.c
#

# CC = icc
CC = g++
DEBUG =
#DEBUG = -ggdb
DATE = `date +%Y%m%d%H%M`

APP = cl

all: $(APP)

cl: codelines.o source_c.o source_py.o source_txt.o
	$(CC) $(DEBUG) codelines.o source_c.o source_py.o source_txt.o -o cl

codelines.o: codelines.c
	$(CC) $(DEBUG) -c codelines.c

source_c.o: source_c.c source_c.h
	$(CC) $(DEBUG) -c source_c.c

source_py.o: source_py.c source_py.h
	$(CC) $(DEBUG) -c source_py.c

source_txt.o: source_txt.c source_txt.h
	$(CC) $(DEBUG) -c source_txt.c
clean:
	rm -f *.o cl

install:
	/usr/bin/install --verbose --strip --target-directory ~/bin $(APP)

backup:
	cp -f codelines.c /nfs/projnfs/backups/codelines/codelines.c.$(DATE)
	cp -f source_c.c /nfs/projnfs/backups/codelines/source_c.c.$(DATE)
	cp -f source_py.c /nfs/projnfs/backups/codelines/source_py.c.$(DATE)
	cp -f source_txt.c /nfs/projnfs/backups/codelines/source_txt.c.$(DATE)
	cp -f Makefile /nfs/projnfs/backups/codelines/Makefile.c.$(DATE)

