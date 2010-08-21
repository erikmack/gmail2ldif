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

#ifndef OUTPUT_H_
#define OUTPUT_H_

#include <stdlib.h>

struct output_config {
	int out_fd;
	char * dn_suffix;
	int show_help;
};

void perform_conversion( struct output_config outconf );

struct header_parse_result {
	int is_global;
	size_t domain_name_len;
	long count_start;
	size_t count_len;
	long count_val;
	long field_start;
	size_t field_len;
};
struct header_parse_result parse_header( wchar_t * header );


#endif // OUTPUT_H_
