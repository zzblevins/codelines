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
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

#define TRUE		1
#define FALSE		(!TRUE)
#define MAXFILENAME	20
#define MAXLINELENGTH	1024

int process_c_source(FILE *x, int v);

/* Process a C language file */
int process_c_source(FILE *fp, int vflag)
{

	char c; 
	char *cl; 
	char codeline[MAXLINELENGTH];
	char pcodeline[MAXLINELENGTH];

	int codelines =		0;
	int j =			0;
	int SawCode =		FALSE;
	int InCComment =	FALSE;
	int InCppComment =	FALSE;
	int TestSlash =		FALSE;
	int TestStar =		FALSE;
	int TotalLines =	0;
	int rawlines =		0;
	int maxline =		MAXLINELENGTH; 


	/* Process each file */

	rawlines = 0;

	// Read each line
	while ( ( cl = fgets(codeline, maxline, fp) ) != NULL) {

		rawlines = rawlines +1;

		// Traverse the codeline string
		for (j=0; j < strlen(codeline); j++) {

			c = codeline[j];
		
			switch (c) {
				
				case ' ': case '\t': case '\r': 
					TestSlash = FALSE;
					break;

				case '/':
					if (TestSlash) { 
						InCppComment = TRUE;
						TestSlash = FALSE; // Reset Cpp comment marker
					} else if (TestStar) {	
						TestStar = FALSE;
						TestSlash = FALSE;
						SawCode = FALSE;
						InCppComment = FALSE;
						InCComment = FALSE;
					} else
						TestSlash = TRUE;
					break;

				case '*':
					if (TestSlash) {
						InCComment = TRUE;
						if (SawCode) // saw code earlier in the line?
							codelines++;
						SawCode = FALSE;
						InCppComment = FALSE;
						TestSlash = FALSE;
						TestStar = FALSE;
					} else
						TestStar = TRUE;
					break;

				case '\n':

					if (SawCode) {
						codelines++;
						InCppComment = FALSE;
						SawCode = FALSE;
					}
					if (InCppComment) {
						InCppComment = FALSE;
						SawCode = FALSE;
						TestSlash = FALSE;
					}
					/** VERBOSE **/
					if (vflag) {
						strncpy(pcodeline, codeline, strlen(codeline)-1); // chomp NL
						pcodeline[strlen(codeline)-1] = '\0'; // Terminate the string
						printf("%d : %d : %s\n", rawlines, codelines, pcodeline ); 
					}
					/** VERBOSE **/

					TestStar = FALSE;
					break;

				default:
					// Read a code char, reset comment marker
					TestSlash = FALSE;
					TestStar = FALSE;
					if (InCComment) break;
					if (InCppComment) break;
					SawCode = TRUE;
					break;
			}

		}
	}

	return(codelines);

}

main(int argc, char *argv[])
{

	FILE *fp;

	char filename[MAXFILENAME];

	int o;
	int vflag =		0;
	int tflag =		0;
	int codelines =		0;
	int index =		0;
	int TotalLines =	0;

	while ((o = getopt (argc, argv, "vth")) != -1) {
		switch (o) {
			case 'v':
				vflag = 1;
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

		// Is it a c language file?
		if ( strstr(argv[index], ".c") || strstr(argv[index], ".h") ) {
			if (vflag) {
				printf("c source: %s\n", argv[index]);
			}

			codelines = process_c_source(fp, vflag);
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
