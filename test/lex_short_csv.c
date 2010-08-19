#include <assert.h>

#include "parse.h"

int main() {

	input_initialize();

	int success = 1;
	struct token tok;

	tok = next_token();
	success = success && tok.type==STRING_SET;
	success = success && !wcscmp( tok.strings[0] , L"One" );

	tok = next_token();
	success = success && tok.type==COMMA;

	tok = next_token();
	success = success && tok.type==STRING_SET;
	success = success && !wcscmp( tok.strings[0] , L"Two" );

	tok = next_token();
	success = success && tok.type==COMMA;

	tok = next_token();
	success = success && tok.type==STRING_SET;
	success = success && !wcscmp( tok.strings[0] , L"Three \\\"tokens\\\"" );

	tok = next_token();
	success = success && tok.type==NEWLINE;

	tok = next_token();
	success = success && tok.type==COMMA;

	tok = next_token();
	success = success && tok.type==COMMA;

	tok = next_token();
	success = success && tok.type==STRING_SET;
	success = success && !wcscmp( tok.strings[0] , L"Multiline\\r\\ntoken\\r\\nhere");

	tok = next_token();
	success = success && tok.type==NEWLINE;

	tok = next_token();
	success = success && tok.type==COMMA;

	tok = next_token();
	success = success && tok.type==COMMA;

	tok = next_token();
	success = success && tok.type==NEWLINE;

	tok = next_token();
	success = success && tok.type==ENDOFFILE;

	assert( success );

	input_destroy();

}
