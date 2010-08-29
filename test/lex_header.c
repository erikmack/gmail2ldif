#include <assert.h>

#include "input.h"
#include "parse.h"

int main() {

	input_initialize();

	int success = 1;
	struct token tok;

	tok = next_token();
	success = success && tok.type==STRING_SET;
	success = success && !wcscmp( tok.strings[0] , L"Name" );

	tok = next_token();
	success = success && tok.type==COMMA;

	tok = next_token();
	success = success && tok.type==STRING_SET;
	success = success && !wcscmp( tok.strings[0] , L"Given Name" );

	tok = next_token();
	success = success && tok.type==COMMA;

	tok = next_token();
	success = success && tok.type==STRING_SET;
	success = success && !wcscmp( tok.strings[0] , L"Additional Name" );

	tok = next_token();
	success = success && tok.type==COMMA;

	tok = next_token();
	success = success && tok.type==STRING_SET;
	success = success && !wcscmp( tok.strings[0] , L"Family Name" );

	tok = next_token();
	success = success && tok.type==COMMA;

	tok = next_token();
	success = success && tok.type==STRING_SET;
	success = success && !wcscmp( tok.strings[0] , L"Yomi Name" );

	tok = next_token();
	success = success && tok.type==COMMA;

	assert( success );

	input_destroy();

	return 0;

}
