#include <assert.h>
#include <string.h>
#include <unistd.h>

#include "parse.h"

/* Begin parsing test infrastructure */

enum callback_type {
	LINE_END,
	HEADER_END,
	STRING_PARSED
};

struct parser_callback {
	enum callback_type type;
	wchar_t * string;
	int field_index;
	int result;
};

#define MAX_CALLBACKS 128

struct parser_callback_list {
	off_t index;
	size_t callback_count;
	struct parser_callback entries[MAX_CALLBACKS];
};
	
struct parser_callback_list * g_list = NULL;

static void test_line_end_reached() {
	int result;
	struct parser_callback this = g_list->entries[ g_list->index ];
	result = ( this.type == LINE_END );
	g_list->entries[ g_list->index ].result = result;
	g_list->index++;
}

static void test_header_end_reached() {
	int result;
	struct parser_callback this = g_list->entries[ g_list->index ];
	result = ( this.type == HEADER_END );
	g_list->entries[ g_list->index ].result = result;
	g_list->index++;
}

static void test_string_token_parsed( wchar_t ** strings, size_t strings_count, int field_index ) {
	int result;
	struct parser_callback this = g_list->entries[ g_list->index ];
	result = ( this.type == STRING_PARSED );
	result = result && ( !wcscmp( this.string, strings[0] ) );
	result = result && ( this.field_index == field_index );
	g_list->entries[ g_list->index ].result = result;
	g_list->index++;
}

/* End parsing test infrastructure */



int main() {

	struct parser_callback_list list;
	g_list = &list;
	memset( &list, 0, sizeof list );

	list.entries[ list.callback_count ].type = STRING_PARSED; 
	list.entries[ list.callback_count ].string = L"One"; 
	list.entries[ list.callback_count ].field_index = 0; 
	list.callback_count++;
	
	list.entries[ list.callback_count ].type = STRING_PARSED; 
	list.entries[ list.callback_count ].string = L"Two"; 
	list.entries[ list.callback_count ].field_index = 1; 
	list.callback_count++;
	
	list.entries[ list.callback_count ].type = STRING_PARSED; 
	list.entries[ list.callback_count ].string = L"Three \\\"tokens\\\""; 
	list.entries[ list.callback_count ].field_index = 2; 
	list.callback_count++;
	
	list.entries[ list.callback_count ].type = HEADER_END; 
	list.callback_count++;

	list.entries[ list.callback_count ].type = STRING_PARSED; 
	list.entries[ list.callback_count ].string = L"Multiline\\r\\ntoken\\r\\nhere"; 
	list.entries[ list.callback_count ].field_index = 2; 
	list.callback_count++;
	
	list.entries[ list.callback_count ].type = LINE_END; 
	list.callback_count++;

	list.entries[ list.callback_count ].type = LINE_END; 
	list.callback_count++;

	parse( &test_line_end_reached, &test_header_end_reached, 
		&test_string_token_parsed );
	
	int success = 1;
	int i;
	for(i = 0; i<list.callback_count; i++ ) {
		success = success && list.entries[ i ].result;
	}

	assert(success);

	return 0;
}
