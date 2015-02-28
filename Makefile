#
# Makefile for codelines.c
#

# CC = icc
CC = g++
DEBUG = -ggdb
DATE = `date +%Y%m%d%H%M`

cl: codelines.c
	$(CC) codelines.c -o cl

debug: codelines.c
	$(CC) $(DEBUG) codelines.c -o cl.debug

install:
	cp -f cl ~/bin

backup:
	cp -f codelines.c /nfs/projnfs/backups/codelines/codelines.c.$(DATE)
	cp -f Makefile /nfs/projnfs/backups/codelines/Makefile.$(DATE)

clean:
	rm *.o
