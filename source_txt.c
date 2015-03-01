/*

	source_py.c

	Process plan text lines. Only removes blank lines.

	API:
	int process_py_source(FILE *, int, int)
		FILE *:	Source file info
		int:	vflag, light verbosity 
		int:	Vflag, high verbosity

	Dean Blevins
	Feb 2015

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

#include "source_txt.h"

#define TRUE		1
#define FALSE		(!TRUE)
#define MAXLINELENGTH	1024

/* Process a text file */
int process_txt_source(FILE *fp, int vflag, int Vflag)
{

	char codeline[MAXLINELENGTH];
	char pcodeline[MAXLINELENGTH];

	int codelines =		0;
	int j =			0;
	int TotalLines =	0;
	int rawlines =		0;
	int maxline =		MAXLINELENGTH; 


	// Read each line
	while ( ( fgets(codeline, maxline, fp) ) != NULL) {

		rawlines++; 

		// Get past initial white space
		j = 0;
		while (codeline[j] == ' ' || codeline[j] == '\t' || codeline[j] == '\r') {
			j++;
		}

		if (codeline[j] != '\n') {
			codelines++;
		}

		/** Very verbose **/
		if (Vflag) {
			strncpy(pcodeline, codeline, strlen(codeline)-1); // chomp NL
			pcodeline[strlen(codeline)-1] = '\0'; // Terminate the string
			printf("%d : %d : %s\n", rawlines, codelines, pcodeline ); 
		}

	}

	return(codelines);

}
