#include <string.h>
#include <stdio.h>
#include <wchar.h>
#include <malloc.h>

#include "parse.h"
#include "input.h"

int fwprintf( FILE * stream, const wchar_t * format, ...);
int swprintf( wchar_t * wcs, size_t maxlen, const wchar_t * format, ...);

// TODO: handle OOM
void append_char( wchar_t wc, struct token * tok ) {

	if( !tok->strings ) {
		tok->strings = malloc(sizeof( wchar_t ** ));
		tok->strings[0] = NULL;
	}

	// value may have been incremented before function call
	size_t new_count = tok->strings_count;

	wchar_t * last_real_element = tok->strings[ new_count-1 ];
	size_t newlen;

	if( last_real_element ) {
		// if element pre-exists
		newlen = ( wcslen( last_real_element ) + 1 );
	} else {
		// if last_real_element is null terminator, append new string

		// grow list, add null terminator
		tok->strings = realloc( tok->strings, (new_count+1)*sizeof(wchar_t *));
		tok->strings[ new_count ] = NULL;

		// reserve (empty) place for string
		tok->strings[ new_count-1 ] = malloc(0);
		newlen = 1;
	}

	// grow string, append char
	tok->strings[ new_count-1 ] = realloc( tok->strings[ new_count-1 ], (newlen+1) * sizeof(wchar_t) );
	last_real_element = tok->strings[ new_count-1 ];
	last_real_element[ newlen - 1 ] = wc;
	last_real_element[ newlen ] = L'\0';
}

struct token next_token() {


	//while(has_more_wchars()) fwprintf(stderr, L"Found char %c\n", get_current_char() );

	struct token tok;
	memset( &tok, 0, sizeof tok );
	tok.type = UNKNOWN;
	tok.strings_count = 0;

	if( !can_peek() ) {
		tok.type = ENDOFFILE;
		return tok;
	}

	int line_index = 0;
	//int field_index = 0;

	#define ERROR_LEN 256
	wchar_t error_msg[ ERROR_LEN+1 ];
	memset( error_msg, 0, (ERROR_LEN+1)*sizeof(wchar_t) );

	enum separator_state {
		NOT_IN_SEPARATOR,
		IN_SPACE_1,
		IN_COLON_1,
		IN_COLON_2,
		IN_COLON_3,
		IN_SPACE_2, //needed?
	};
		
	enum separator_state sep_state = NOT_IN_SEPARATOR;
	
	int in_quote = 0;
	//int in_string = 0;
	while(1) {
		if(has_more_wchars()) {
			if( !tok.strings_count ) tok.strings_count++;

			wchar_t c = get_current_char();
			if(in_quote) {
				if( c == L'"' ) {
					// Handle escaped quote, convert "" to \"
					if( peek_next_char() == L'"' ) {
						append_char( L'\\' , &tok );
						append_char( L'"' , &tok );
						has_more_wchars();
					} else {
						in_quote = 0;
					}

				// Escape new lines in quote for LDIF
				} else if( c == L'\n' ) {
					append_char( L'\\' , &tok );
					append_char( L'n' , &tok );
				} else if( c == L'\r' ) {
					append_char( L'\\' , &tok );
					append_char( L'r' , &tok );

				} else {
					append_char( c , &tok );
				}
			} else {
				
				// if we finished a separator, bump strings_count and reset
				if( sep_state == IN_SPACE_2 ) {
					tok.strings_count++;
					sep_state = NOT_IN_SEPARATOR;
				}

				enum separator_state old_state = sep_state;
				if( sep_state == NOT_IN_SEPARATOR && c==L' ' ) sep_state = IN_SPACE_1;
				else if( sep_state == IN_SPACE_1 && c==L':' ) sep_state = IN_COLON_1;
				else if( sep_state == IN_COLON_1 && c==L':' ) sep_state = IN_COLON_2;
				else if( sep_state == IN_COLON_2 && c==L':' ) sep_state = IN_COLON_3;
				else if( sep_state == IN_COLON_3 && c==L' ' ) sep_state = IN_SPACE_2;
				else sep_state = NOT_IN_SEPARATOR;

				if( sep_state == NOT_IN_SEPARATOR ) {

					if( old_state != NOT_IN_SEPARATOR ) {
						// We were wrong about the separator, append characters to remediate
						if( old_state >= IN_SPACE_1 ) append_char( L' ', &tok );
						if( old_state >= IN_COLON_1 ) append_char( L':', &tok );
						if( old_state >= IN_COLON_2 ) append_char( L':', &tok );
						if( old_state >= IN_COLON_3 ) append_char( L':', &tok );
					}

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
							tok.strings[0] = error_msg;
							swprintf( error_msg, ERROR_LEN, L"Invalid line ending encountered, line %d\n", line_index );
							break;
						}
					} else if( c == L'"' ) {
						tok.type = STRING_SET;
						in_quote = 1;				

					
					} else {
						tok.type = STRING_SET;
						append_char( c , &tok );
					}
				}
			}

			// String is complete
			if( !in_quote && can_peek() &&
				( peek_next_char() == L','
					|| peek_next_char() == L'\r'
					|| peek_next_char() == L'\n' ) ) {
				break;
			}
		} else {
			tok.type = ENDOFFILE;
			break;
		}
	};

	if( tok.type == UNKNOWN ) {
		tok.type = ERROR;
		tok.strings[0] = error_msg;
		swprintf( error_msg, ERROR_LEN, L"Couldn't determine token type, line %d\n", line_index );
	}

	return tok;
}

void parse( line_end_func line, header_end_func header, string_parsed_func string )
{
	int header_complete = 0;
	int field_index = 0;
	struct token tok;
	while( (tok=next_token()).type != ENDOFFILE ) {
		if( tok.type == ERROR ) {
			// TODO: handle
		} else if( tok.type == STRING_SET ) {
			string( tok.strings, tok.strings_count, field_index );
			//free( tok.strings );
			tok.strings = NULL;
			tok.strings_count = 0;
		} else if( tok.type == COMMA ) {
			field_index++;
		} else if( tok.type == NEWLINE ) {
			field_index = 0;
			if( header_complete ) {
				line();
			} else {
				header_complete = 1;
				header();
			}
		}
	}

}

