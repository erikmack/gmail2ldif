#ifndef PARSE_H_
#define PARSE_H_

#include <wchar.h>

typedef void (* line_end_func)();
typedef void (* header_end_func)();
typedef void (* string_parsed_func)( wchar_t ** string, int field_index );

void parse( line_end_func line, header_end_func header, string_parsed_func string );

struct token {
	
};

#endif // PARSE_H_
