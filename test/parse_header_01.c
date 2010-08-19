#include <assert.h>

#include "output.h"

int main() {

	wchar_t * header = L"Phone 2 - Type";
	struct header_parse_result result = parse_header( header );

	assert( 
		result.is_global == 0
		&& result.domain_name_len == 5
		&& result.count_start == 6
		&& result.count_len == 1
		&& result.count_val == 2
		&& result.field_start == 10
		&& result.field_len == 4
	);

	return 0;
}
