#include <assert.h>

#include "input.h"

int main() {

	input_initialize();

	wchar_t * string = L"Name,Given Name,Additional Name,Family Name,Yomi";
	int success = 1;
	wchar_t * onechar;
	for( onechar=string; onechar-string < wcslen( string ); onechar++ ) {
		success = success && has_more_wchars() && get_current_char()==*onechar;
		if( !success ) break;
	}

	input_destroy();

	assert(success);

	return 0;

}
