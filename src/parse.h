#ifndef PARSE_H_
#define PARSE_H_

#include <wchar.h>

typedef void (* line_end_func)();
typedef void (* header_end_func)();
typedef void (* string_parsed_func)( wchar_t ** string, int field_index );

void parse( line_end_func line, header_end_func header, string_parsed_func string );

typedef enum {
	UNKNOWN = 0,
	STRING,
	COMMA,
	NEWLINE,
	ERROR,
	ENDOFFILE
} token_type;

struct token {
	token_type type;
	wchar_t * string_val;
};

struct token next_token();

#endif // PARSE_H_
