#
# Makefile for codelines.c
#

# CC = icc
CC = g++
DEBUG =
#DEBUG = -ggdb
DATE = `date +%Y%m%d%H%M`

all: cl

cl: codelines.o source_c.o
	$(CC) $(DEBUG) codelines.o source_c.o -o cl

codelines.o: codelines.c
	$(CC) $(DEBUG) -c codelines.c

source_c.o: source_c.c source_c.h
	$(CC) $(DEBUG) -c source_c.c

clean:
	rm *.o cl

install:
	cp -f cl ~/bin

backup:
	cp -f codelines.c /nfs/projnfs/backups/codelines/codelines.c.$(DATE)
	cp -f source_c.c /nfs/projnfs/backups/codelines/source_c.c.$(DATE)
	cp -f Makefile /nfs/projnfs/backups/codelines/Makefile.c.$(DATE)

