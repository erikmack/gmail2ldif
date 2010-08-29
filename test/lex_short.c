#include <assert.h>

#include "parse.h"

int main() {

	input_initialize();

	int success = 1;
	struct token tok;

	tok = next_token();
	success = success && tok.type==STRING_SET;
	success = success && !wcscmp( tok.strings[0] , L"Hello" );
	assert( success );

	input_destroy();

	return 0;

}
