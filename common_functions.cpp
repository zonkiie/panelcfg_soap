#include <includes.h>

/// @see https://stackoverflow.com/questions/35390912/proper-way-to-get-file-size-in-c
int get_filesize(char * filename)
{
	int fsize = 0;
	if(access(filename, F_OK)) // file not found
		return -1;
	struct stat buffer;
	int         status;

	status = stat("path to file", &buffer);
	if(status == 0) {
	// size of file is in member buffer.st_size;
		fsize = buffer.st_size;
	}
	return fsize;
}

 
