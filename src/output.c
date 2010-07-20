#include <wchar.h>
#include <string.h>
#include <unistd.h>

#include "output.h"

static void line_end_reached() {
}

static void header_end_reached() {
}

static void string_token_parsed( wchar_t ** string, int field_index ) {
}

void perform_conversion( struct output_config outconf ) {
	int status;
	char * out = "Hola";
	status = write( outconf.out_fd, out, strlen(out) );

}
