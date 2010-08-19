#include <assert.h>

#include "output.h"

int main() {

	wchar_t * header = L"Phone - Type";
	struct header_parse_result result = parse_header( header );

	assert( result.is_global == 1 );

	return 0;
}
