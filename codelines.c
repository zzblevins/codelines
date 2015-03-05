/*

	codelines.c

	Find the number of C or C++ codelines, filters out the comments.

	Syntax:
	% $command [-vth] file1, file2, ...

	Dean Blevins
	March 2013
	1.0: Mon Oct 13 21:00:29 CDT 2014, C code only source type

*/

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

#include "source_c.h"
#include "source_py.h"
#include "source_txt.h"

#define MAXFILENAME	20

main(int argc, char *argv[])
{

	FILE *fp;

	struct stat stbuf;

	char filename[MAXFILENAME];
	char *ext;

	int o;
	int tflag =		0;
	int codelines =		0;
	int index =		0;
	int TotalLines =	0;
	int prefix =		0;

	int vflag =		0;
	int Vflag =		0;

	while ((o = getopt (argc, argv, "vVth")) != -1) {
		switch (o) {
			case 'v':
				vflag = 1;
				break;
			case 'V':
				Vflag = 1;
				break;
			case 't':
				tflag = 1;
				break;
			case 'h':
				printf("Syntax: %s [-v] [-t] file(s) \n", argv[0]);
				printf(" -h : syntax\n");
				printf(" -t : total of all files\n");
				printf(" -v : verbose output for debug\n");
				exit(0);
				break;
			case '?':
				if (isprint(optopt))
					fprintf(stderr, "Unknown option '-%c'.\n", optopt);
				break;
			default:
				break;
		}
	}

	if (optind == argc) {
		fprintf(stderr, "Error: No files to process...\n");
		return (-1);
	}

	/* Initialize count for all files */

	TotalLines = 0;

	/* Process each file */
	for (index = optind; index < argc; index++) {
	
		if ( ( fp = fopen(argv[index], "r") ) == NULL) {
			printf("Error: Can't read %s\n", argv[index]);
			return (-1);
		}

		// Check to see if this is a directory, skip if so
		stat(argv[index], &stbuf);
		if (stbuf.st_mode & S_IFDIR) {
			if (vflag || Vflag) {
				printf("Skipping directory: %s\n", argv[index]);
                        }
			continue;
		}

		// Look at the file extension (last .)

		ext = strrchr(argv[index], '.');
		if (ext == NULL) {

			// No '.', assume it's a text file

			if (vflag || Vflag) {
				printf("Text(?) source: %s\n", argv[index]);
			}

			codelines = process_txt_source(fp, vflag, Vflag);

		} else if ((strcmp(ext, ".c") == 0) || (strcmp(ext, ".h") == 0)) {
				
			// C language file
			
			if (vflag || Vflag) {
				printf("c source: %s\n", argv[index]);
			}

			codelines = process_c_source(fp, vflag, Vflag);

		} else if (strcmp(ext, ".py") == 0) {
				
			// Python file
			
			if (vflag || Vflag) {
				printf("Python source: %s\n", argv[index]);
			}

			codelines = process_py_source(fp, vflag, Vflag);

		} else {

			// Treat as a text file - default
			if (vflag || Vflag) {
				printf("Text(?) source: %s\n", argv[index]);
			}

			codelines = process_txt_source(fp, vflag, Vflag);
		}

		printf ("%d %s\n", codelines, argv[index]);

		TotalLines += codelines;
		codelines = 0;

		fclose(fp);
	}

	if (tflag)
		printf ("Total: %d\n", TotalLines);

	return (0);
}
