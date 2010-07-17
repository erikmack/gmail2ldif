#include <string.h>
#include <stdio.h>
#include <wchar.h>
#include <malloc.h>

#include "parse.h"
#include "input.h"

int fwprintf( FILE * stream, const wchar_t * format, ...);
int swprintf( wchar_t * wcs, size_t maxlen, const wchar_t * format, ...);

// TODO: handle OOM
#define APPEND_CHAR( wc ) { \
	size_t newlen = 2; \
	if( tok.string_val ) { \
		newlen = ( wcslen( tok.string_val ) + 2 ); \
		tok.string_val = realloc( tok.string_val, newlen * sizeof(wchar_t) ); \
	} else { \
		tok.string_val = malloc( newlen * sizeof(wchar_t) ); \
	} \
	*(tok.string_val + newlen - 2) = wc ; \
	*(tok.string_val + newlen - 1) = L'\0'; \
}

struct token next_token() {


	//while(has_more_wchars()) fwprintf(stderr, L"Found char %c\n", get_current_char() );

	struct token tok;
	memset( &tok, 0, sizeof tok );
	tok.type = UNKNOWN;

	if( !can_peek() ) {
		tok.type = ENDOFFILE;
		return tok;
	}

	int line_index = 0;
	//int field_index = 0;

	#define ERROR_LEN 256
	wchar_t error_msg[ ERROR_LEN+1 ];
	memset( error_msg, 0, (ERROR_LEN+1)*sizeof(wchar_t) );
	
	int in_quote = 0;
	//int in_string = 0;
	while(has_more_wchars()) {

		wchar_t c = get_current_char();
		if(in_quote) {
			if( c == L'"' ) {
				// Handle escaped quote, convert "" to \"
				if( peek_next_char() == L'"' ) {
					APPEND_CHAR( L'\\' )
					APPEND_CHAR( L'"' )
					has_more_wchars();
				} else {
					in_quote = 0;
				}

			// Escape new lines in quote for LDIF
			} else if( c == L'\n' ) {
				APPEND_CHAR( L'\\' )
				APPEND_CHAR( L'n' )
			} else if( c == L'\r' ) {
				APPEND_CHAR( L'\\' )
				APPEND_CHAR( L'r' )

			} else {
				APPEND_CHAR( c )
			}
		} else {
			if( c == L',' ) {
				//field_index++;
				tok.type = COMMA;
				break;
			} else if( c == L'\r' ) {
				if( can_peek() && peek_next_char()==L'\n' ) {
					line_index++;
					has_more_wchars();
					tok.type = NEWLINE;
					break;
				} else {
					tok.type = ERROR;
					tok.string_val = error_msg;
					swprintf( error_msg, ERROR_LEN, L"Invalid line ending encountered, line %d\n", line_index );
					break;
				}
			} else if( c == L'"' ) {
				tok.type = STRING;
				in_quote = 1;				
			} else {
				tok.type = STRING;
				APPEND_CHAR( c )
			}
		}

		// String is complete
		if( !in_quote && can_peek() &&
			( peek_next_char() == L','
				|| peek_next_char() == L'\r'
				|| peek_next_char() == L'\n' ) ) {
			break;
		}

	};

	if( tok.type == UNKNOWN ) {
		tok.type = ERROR;
		tok.string_val = error_msg;
		swprintf( error_msg, ERROR_LEN, L"Couldn't determine token type, line %d\n", line_index );
	}

	return tok;
}

void parse( line_end_func line, header_end_func header, string_parsed_func string )
{

	int status = input_initialize();
	if(status == -1) {
		fwprintf( stderr, L"initialize failed\n" );
	}
	struct token nt = next_token();
	input_destroy();

}

