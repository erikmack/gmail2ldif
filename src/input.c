
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>
#include <iconv.h>

#include "input.h"

#define RAW_STDIN_SZ 16
char raw_stdin[ RAW_STDIN_SZ ];

struct token next_token() {
	// Change stream to wide-character
	if( !fwide(stdout,0) ) {
		if( fwide(stdout,1) <= 0 ) {
			perror( "Can't set to wide character stream, exiting failure\n");
			exit(1);
		} 
	}

	memset( raw_stdin, 0, RAW_STDIN_SZ );

	int status = 0;

	// Input can be encoded as ASCII or UTF-16LE, discern
	status = read(0, raw_stdin, 2);
	if( status == -1 ) wprintf( L"read: %s\n", strerror(errno) );

	char * input_encoding = NULL;
	if(raw_stdin[0]=='\377' && raw_stdin[1]=='\376') {
		input_encoding = "UTF-16LE";
	} else {
		input_encoding = "ASCII";
		rewind( stdin );
	}
}
