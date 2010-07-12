#include <string.h>
#include <stdio.h>
#include <wchar.h>

#include "parse.h"
#include "input.h"

int fwprintf( FILE * stream, const wchar_t * format, ...);

struct token next_token() {

	int status = input_initialize();
	if(status == -1) {
		fwprintf( stderr, L"initialize failed\n" );
	}

	//while(has_more_wchars()) fwprintf(stderr, L"Found char %c\n", get_current_char() );


	input_destroy();

	struct token tok;
	memset( &tok, 0, sizeof tok );
	return tok;
}

void parse( line_end_func line, header_end_func header, string_parsed_func string )
{

	struct token nt = next_token();

}

