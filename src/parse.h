#ifndef PARSE_H_
#define PARSE_H_

#include <wchar.h>

void parse(
	void (line_end_func)(),
	void (header_end_func)(),
	void (string_parsed_func)( wchar_t ** string, int field_index )
);

#endif // PARSE_H_
