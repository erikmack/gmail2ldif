/*
    Copyright (C) 2010 Free Software Foundation

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef PARSE_H_
#define PARSE_H_

#include <wchar.h>

typedef void (* line_end_func)();
typedef void (* header_end_func)();
typedef void (* string_parsed_func)( wchar_t ** strings, size_t strings_count, int field_index );

void parse( line_end_func line, header_end_func header, string_parsed_func string );

typedef enum {
	UNKNOWN = 0,

	// A NULL-terminated wchar_t **
	// since a csv 'cell' in the input
	// can have multiple strings embedded
	// Example: Enter two e-mails with type
	// 'work', and the csv will contain
	// a single 'work' e-mail with this value:
	//    "one@work.com ::: two@work.com"
	// so we split on the " ::: " token into
	// a set of strings
	STRING_SET,

	COMMA,
	NEWLINE,
	ERROR,
	ENDOFFILE
} token_type;

struct token {
	token_type type;
	wchar_t ** strings;
	size_t strings_count;
};

struct token next_token();

#endif // PARSE_H_
