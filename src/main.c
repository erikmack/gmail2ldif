#include "test.h"
#include "parse.h"

static void line_end_reached() {
}

static void header_end_reached() {
}

static void string_token_parsed( wchar_t ** string, int field_index ) {
}

int main() {
	

	run_tests();
	/*
	int status = input_initialize();
	if(status == -1) {
		fwprintf( stderr, L"initialize failed\n" );
	}
	parse( &line_end_reached, &header_end_reached, 
		&string_token_parsed );
	input_destroy();
	*/



	
	

	
	return 0;
}
