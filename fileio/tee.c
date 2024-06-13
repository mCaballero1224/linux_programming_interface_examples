/*
* tee.c
* Reads stdin until end of file writing a copy of the input to
* stdout and to the file named in its command line argument.
* Overwrites any existing file with the given name.
*
* Commandline options:
* -a: causes tee to append text to the end of a file if it already exists
*/

#include <errno.h> /* system error numbers */
#include <stdio.h> /* standard buffered input/output */
#include <stdlib.h> /* standard library definitions */
#include <string.h> /* string operations */
#include <unistd.h> /* standard symbolic constants and types */
#include <ctype.h> /* character types */
#include <fcntl.h> /* file control options */
#include <sys/stat.h> /* data returned by the stat() function */

#define BUFFSIZE 32768

int
main(int argc, char **argv) 
{
	int outputFd, openFlags;
	mode_t filePerms;
	ssize_t numRead; // integer to track number of bytes read from stdin durin the while loop
	char buf[BUFFSIZE]; // buffer used to read bytes from stdin
	
	openFlags = O_CREAT | O_WRONLY | O_TRUNC | O_APPEND;
	filePerms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP |
		    S_IROTH | S_IWOTH; /* rw-rw-rw- */

	outputFd = open(argv[1], openFlags, filePerms);
	
	/* cast to void to silence compiler warnings */
	(void)argc;
	(void)argv;

	while ((numRead = read(STDIN_FILENO, buf, BUFFSIZE)) > 0) {
		if (write(STDOUT_FILENO, buf, numRead) != numRead) {
			fprintf(stderr, "Unable to write: %s\n",
				strerror(errno));
			exit(EXIT_FAILURE);
		}

		if (write(outputFd, buf, numRead) != numRead) {
			fprintf(stderr, "Unable to write: %s\n",
				strerror(errno));
			exit(EXIT_FAILURE);
		}
	}

	if (numRead < 0) {
		fprintf(stderr, "Unable to read: %s\n",
			strerror(errno));
		exit(EXIT_FAILURE);
	}
	
}
