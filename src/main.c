#include "parse.h"

static void line_end_reached() {
}

static void header_end_reached() {
}

static void string_token_parsed( wchar_t ** string, int field_index ) {
}

int main() {
	

	parse( &line_end_reached, &header_end_reached, 
		&string_token_parsed );


	
	

	
	return 0;
}
