#include "parse.h"
#include "input.h"


void parse(
	void (line_end_func)(),
	void (header_end_func)(),
	void (string_parsed_func)( wchar_t ** string, int field_index ) )
{
	struct token nt = next_token();

}
