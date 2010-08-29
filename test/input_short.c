#include <assert.h>

#include "input.h"

int main() {

	input_initialize();

	assert( has_more_wchars()
		&& get_current_char() == L'H'
		&& has_more_wchars()
		&& get_current_char() == L'e'
		&& has_more_wchars()
		&& get_current_char() == L'l'
		&& has_more_wchars()
		&& get_current_char() == L'l'
		&& has_more_wchars()
		&& get_current_char() == L'o'
		&& !has_more_wchars()
		);

	input_destroy();

	return 0;

}
