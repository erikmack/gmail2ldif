#include <assert.h>

#include "output.h"

int main() {

	wchar_t * header = L"Two Words 14 - Two More";
	struct header_parse_result result = parse_header( header );

	assert( 
		result.is_global == 0
		&& result.domain_name_len == 9
		&& result.count_start == 10
		&& result.count_len == 2
		&& result.count_val == 14
		&& result.field_start == 15
		&& result.field_len == 8
	);

	return 0;
}
