#ifndef OUTPUT_H_
#define OUTPUT_H_

#include <stdlib.h>

struct output_config {
	int out_fd;
	char * dn_suffix;
};

void perform_conversion( struct output_config outconf );

struct header_parse_result {
	int is_global;
	size_t domain_name_len;
	off_t count_start;
	size_t count_len;
	long count_val;
	off_t field_start;
	size_t field_len;
};
struct header_parse_result parse_header( wchar_t * header );


#endif // OUTPUT_H_
