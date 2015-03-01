/*

	source_c.c

	Process c language source lines. 

	API:
	int process_c_source(FILE *, int, int)
		FILE *: Source file info
		int:    vflag, light verbosity 
		int:    Vflag, high verbosity


	Dean Blevins
	Feb 2015

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

#include "source_c.h"

#define TRUE		1
#define FALSE		(!TRUE)
#define MAXLINELENGTH	1024

/* Process a C language file */
int process_c_source(FILE *fp, int vflag, int Vflag)
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
					/** Very verbose **/
					if (Vflag) {
						strncpy(pcodeline, codeline, strlen(codeline)-1); // chomp NL
						pcodeline[strlen(codeline)-1] = '\0'; // Terminate the string
						printf("%d : %d : %s\n", rawlines, codelines, pcodeline ); 
					}

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
