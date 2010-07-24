#include "test.h"
#include "parse.h"
#include "output.h"

int main() {
	

	run_tests();

	/*
	struct output_config outconf;
	outconf.out_fd = 1; //stdout
	perform_conversion( outconf );


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
