#include <assert.h>

#include "input.h"
#include "parse.h"

int main() {

	input_initialize();

	int success = 1;
	struct token tok;

	tok = next_token();
	success = success && tok.type==STRING_SET;
	success = success && tok.strings_count==3;
	success = success && !wcscmp( tok.strings[0] , L"One" );
	success = success && !wcscmp( tok.strings[1] , L"Two :: Three" );
	success = success && !wcscmp( tok.strings[2] , L"Four" );
	assert( success );

	input_destroy();

	return 0;

}
