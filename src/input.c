
#include <assert.h>
#include <wchar.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iconv.h>
#include <unistd.h>

#include "input.h"


// Some required forward declarations
int fwprintf( FILE * stream, const wchar_t * format, ...);
int fwide( FILE * stream, int mode );


// Raw character data is read into a buffer.
#define RAW_STDIN_SZ 16
char raw[ RAW_STDIN_SZ ];
char * raw_convert_from;
char * raw_read_to;


#define WCHAR_BUF_COUNT 16
wchar_t wide[ WCHAR_BUF_COUNT ];
wchar_t * converted_to_here = wide;





int is_lexer_initialized = 0;
iconv_t cd = (iconv_t)-1;	// conversion descriptor for iconv

int input_fd = 0;	// normally stdin, but unit tests will set it otherwise

wchar_t current_wchar;
int can_peek_var = 0;

wchar_t * peek_wchar_ptr;

int has_more_wchars() {

	// Maybe invalid (start state) but will reset soon
	current_wchar = *peek_wchar_ptr;
	peek_wchar_ptr++;
	can_peek_var = 0;

	if( peek_wchar_ptr < converted_to_here ) {
		can_peek_var = 1;	
	// At end of converted characters, must convert some more
	} else if( peek_wchar_ptr == converted_to_here ) {
		
		// raw_read_to at end of buffer, need to read again
		if( raw_read_to == raw + RAW_STDIN_SZ ) {

			if( raw_convert_from == raw_read_to ) {
				raw_read_to = raw;
			} else {
				// Multibyte input was truncated, move fragment to buffer start
				size_t fragment_sz = raw_read_to - raw_convert_from;
				memmove( raw, raw_convert_from, fragment_sz );
				raw_read_to = raw + fragment_sz;
			}

			raw_convert_from = raw;
			int status = read( input_fd, raw_read_to, RAW_STDIN_SZ-(raw_read_to-raw));
			if( status == -1 ) {
				fwprintf( stderr, L"read: %s\n", strerror(errno) );
				return 0;
			} else {
				raw_read_to += status;
			}
		}
		
		peek_wchar_ptr = wide ;
		size_t insz = (raw + RAW_STDIN_SZ) - raw_convert_from;
		size_t outsz = WCHAR_BUF_COUNT * sizeof(wchar_t);

		char * wide_target_char_ptr = (char *)wide;


		size_t status = iconv( cd,
			&raw_convert_from, &insz,
			&wide_target_char_ptr, &outsz);
		
		if(status == (size_t)-1) {
			if( errno == EILSEQ ) {
				fwprintf( stderr, L"iconv: encountered illegal input sequence.\n" );
			} else if( errno == EINVAL ) {
				//TODO: Handle this case, where partial character read
				fwprintf( stderr, L"iconv: read partial multibyte character, case not yet implemented.\n" );
			} else if( errno == E2BIG ) {
				fwprintf( stderr, L"iconv: asked to convert too many characters, shouldn't occur, algorithm incorrect.\n" );
			}
		} else {
			//current_wchar = *wide;
			converted_to_here = (wchar_t *)wide_target_char_ptr;
		}
		
	}

	can_peek_var = converted_to_here > wide;
	// iconv will ignore EOF and fill with null chars, stop at null
	can_peek_var = can_peek_var &&  *peek_wchar_ptr;
	
	return can_peek_var;
}

int input_initialize() {
	if( is_lexer_initialized ) return 0;

	// Change stream to wide-character
	if( !fwide(stdout,0) ) {
		if( fwide(stdout,1) <= 0 ) {
			perror( "Can't set to wide character stream, exiting failure\n");
			return -1;
		} 
	}

	memset( raw, 0, RAW_STDIN_SZ );
	
	// Set raw ptr at end to force read
	raw_read_to = raw_convert_from = raw+ RAW_STDIN_SZ;

	// Sanity requirement, prevents multiple conversions for one output wchar
	assert( WCHAR_BUF_COUNT > 1 );

	memset( wide, 0, WCHAR_BUF_COUNT * sizeof(wchar_t) );

	int status = 0;

	// Input can be encoded as ASCII or UTF-16LE, discern
	status = read( input_fd, raw, 2);
	if( status == -1 ) {
		fwprintf( stderr, L"read: %s\n", strerror(errno) );
	} else if (status < 2) {
		fwprintf( stderr, L"read: couldn't even read two bytes to detect encoding, invalid file\n" );
		return -1;
	}

	peek_wchar_ptr = wide + WCHAR_BUF_COUNT - 1;
	
	converted_to_here = wide + WCHAR_BUF_COUNT;

	char * input_encoding = NULL;
	if(raw[0]=='\377' && raw[1]=='\376') {
		input_encoding = "UTF-16LE";
	} else {
		input_encoding = "ASCII";

		// We read two characters to sniff the encoding,
		// but we'd still like to read them normally.
		// rewind() and lseek() can rewind our files under
		// normal conditions, but the unit tests use pipes
		// which are unseekable.  Instead, let's just move
		// the two characters where we'd like them so that
		// reading can resume normally.
		*(wide+WCHAR_BUF_COUNT-2) = btowc( *raw );
		*(wide+WCHAR_BUF_COUNT-1) = btowc( *(raw+1) );
		peek_wchar_ptr -= 1;
		*(raw + RAW_STDIN_SZ - 2) = *raw;
		*(raw + RAW_STDIN_SZ - 1) = *(raw+1);
	}

	cd = iconv_open( "WCHAR_T", input_encoding );
	if( cd == (iconv_t)-1 ) fwprintf( stderr, L"iconv_open: %s\n", strerror(errno) );

	is_lexer_initialized = 1;

	// Force initial read, unless ASCII
	if( strcmp("ASCII",input_encoding) ) has_more_wchars();

	return 0;
}

void input_destroy() {
	int status = iconv_close( cd );
	if( status == -1 ) fwprintf( stderr, L"iconv_close: %s\n", strerror(errno) );

	status = close( input_fd );
	if( status == -1 ) fwprintf( stderr, L"close: %s\n", strerror(errno) );

	input_fd = 0;
	is_lexer_initialized = 0;
}

void set_input( int fd ) {
	input_fd = fd;
}

int can_peek()
{
	return can_peek_var;
}

wchar_t get_current_char()
{
	return current_wchar;
}

/*
 * Result is undefined if !can_peek()
 */
wchar_t peek_next_char()
{
	return *peek_wchar_ptr;
}




