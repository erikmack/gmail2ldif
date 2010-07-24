#include <wchar.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>
#include <wctype.h>
#include <iconv.h>
#include <errno.h>

#include "output.h"
#include "parse.h"

int fwprintf( FILE * stream, const wchar_t * format, ...);
int vswprintf( wchar_t * dest, size_t maxlen, const wchar_t * format, ...);
int swprintf( wchar_t * dest, size_t maxlen, const wchar_t * format, ...);

wchar_t ** strings;
size_t strings_count;
int output_fd;

iconv_t output_iconv_cd;



enum notable_source_fields {
	
	UNUSED = -1,

	// Global domain
	NAME = 0,		// core commonName/cn
	GIVEN_NAME,	// core - givenName/gn
	FAMILY_NAME,	// core - surname/sn
	NAME_PREFIX,	// cosine - personalTitle
	NAME_SUFFIX,	// core - generationQualifier
	INITIALS,	// core - initials
	OCCUPATION,		// core - title

	// Common to all non-global domains
	TYPE,

	// email, phone, website, event domains
	VALUE,

	// address domain
	FORMATTED,

	// organization domain
	
	

	NOTABLE_FIELD_COUNT
};



/* A domain is a container scope for field values */
struct domain {
	int is_global;
	wchar_t * type;
	int index_of_type; // Email 1 is 0, Email 2 is 1, etc

	int notable_field_map[ NOTABLE_FIELD_COUNT ];
	int * unaccounted_fields;
	wchar_t ** unaccounted_field_names;
};

struct domain * domains = NULL;
size_t domain_count = 0;





static int out_printf( const wchar_t * format, ...) {

	/* Begin create single wide buffer */
	va_list argp;
	va_start(argp, format);

	size_t maxlen = 16;
	size_t buf_sz;
	wchar_t * wbuf; 
	int wbuf_chars_written;
	while(1) {

		buf_sz = maxlen * sizeof(wchar_t);
		wbuf = malloc( buf_sz );
		memset( wbuf, 0, buf_sz );
		
		wbuf_chars_written = 
			vswprintf( wbuf, maxlen, format, argp );

		int need_more = 
			wbuf_chars_written == maxlen
			|| (wbuf_chars_written == -1 && errno==ESPIPE );

		if( need_more ) {
			// Didn't have enough room, try again
			free( wbuf );
			maxlen *= 2;
		} else if( wbuf_chars_written == -1 ) {
			fwprintf( stderr, L"vswprintf: %d\n", errno );
		} else break;
	}
	va_end(argp);
	/* End create single wide buffer */


	// Output write buffer ... unlikely to be larger than
	// wbuf (wchar_t -> utf-8), so make it that big
	char mbbuf[ buf_sz ];
	memset( mbbuf, 0, buf_sz );


	char * inbuf;
	char * outbuf;
	inbuf = (char *)wbuf;
	outbuf = (char *)mbbuf;
	size_t inbytesleft = wbuf_chars_written * sizeof(wchar_t);
	size_t outbytesleft = buf_sz;
	iconv( output_iconv_cd, &inbuf, &inbytesleft, &outbuf, &outbytesleft );


	free( wbuf );
	int status = write( output_fd, mbbuf, outbuf-mbbuf );
	if( status == -1 ) fwprintf( stderr, L"output_fd write: %s\n", strerror(errno) );

	return status; 
}

static void line_end_reached() {
}

/*
 * Determines whether a header is global (top-level, not a 
 * list item), or for list items, determine what are the
 * relevant pieces of the header string.
 */
struct header_parse_result parse_header( wchar_t * header ) {
	struct header_parse_result result;
	memset( &result, 0, sizeof result );
	result.is_global = 1;
	
	enum state {
		IN_DOMAIN,
		IN_SPACE_1,
		IN_COUNT,
		IN_SPACE_2,
		IN_HYPHEN,
		IN_SPACE_3,
		IN_FIELD
	};

	enum state current = IN_DOMAIN;

	wchar_t * onechar = header;
	for(; *onechar; onechar++ ) {
		//fwprintf( stderr, L"%c\n", *onechar );

		if( current == IN_DOMAIN ) {
			if( *onechar==L' ' ) {
				current = IN_SPACE_1;
				result.domain_name_len = (onechar - header);
			}
		} else if( current == IN_SPACE_1 ) {
			if( iswdigit( *onechar) ) {
				current = IN_COUNT;
				result.count_start = (onechar - header);
			} else if( iswalpha( *onechar ) ) {
				current = IN_DOMAIN;
			}
		} else if( current == IN_COUNT ) {
			if( !iswdigit( *onechar) ) {
				current = IN_SPACE_2;
				result.count_len = (onechar - header) - result.count_start;
			}
		} else if( current == IN_SPACE_2 ) {
			if( *onechar==L'-' ) {
				current = IN_HYPHEN;
			}
		} else if( current == IN_HYPHEN ) {
			if( *onechar == L' ' ) {
				current = IN_SPACE_3;
			}
		} else if( current == IN_SPACE_3 ) {
			if( *onechar != L' ' ) {
				current = IN_FIELD;
				result.field_start = (onechar - header);
			}
		}
	}

	if( current == IN_FIELD ) {
		result.is_global = 0;
		result.field_len = (onechar - header) - result.field_start;
		result.count_val = wcstol( header+result.count_start, NULL, 0);
	} else {
		result.field_len = wcslen( header );
	}

	return result;
}

static struct domain * get_domain_for_header(wchar_t * header_value, 
	struct header_parse_result parse ) {

	// List domains in format "Phone 2 - Type", all others global
	
	struct domain * ret = NULL;
	int make_new = 0;
	int index_of_type = 0;

	if( domain_count ) {
		
		struct domain * one_domain;
		for( one_domain=domains; one_domain-domains<domain_count; one_domain++ ) {

			int are_both_global = 
				one_domain->is_global && parse.is_global;

			int is_list_type_match =
				!one_domain->is_global
				&& !wcsncmp( one_domain->type, header_value, parse.domain_name_len );

			if( is_list_type_match ) index_of_type++;

			int is_match =
				are_both_global
				|| ( is_list_type_match 
					&& one_domain->index_of_type == parse.count_val-1 );

			if( is_match ) return one_domain;
		}

		domains = realloc( domains, (domain_count+1) * sizeof( struct domain ) );
		ret = domains + domain_count;
		make_new = 1;

	} else {
		domains = ret = malloc( sizeof(struct domain) );
		make_new = 1;
	}

	if( make_new ) {
		domain_count++;
		memset( ret, 0, sizeof(struct domain) );
		ret->is_global = parse.is_global;	
		if( !ret->is_global ) {
			size_t len = parse.domain_name_len;
			ret->type = malloc( (len+1) * sizeof(wchar_t) );
			ret->type[ len ] = L'\0';
			wcsncpy( ret->type, header_value, len );
		}

		int i;
		for( i=0; i<NOTABLE_FIELD_COUNT; i++ ) {
			ret->notable_field_map[ i ] = UNUSED;			
		}

		ret->unaccounted_fields = NULL;
		ret->unaccounted_field_names = NULL;

		ret->index_of_type = index_of_type;
	}

	return ret;
}

static void header_end_reached() {

	int i;
	wchar_t ** one_string = strings;
	for( i=0; i<strings_count; i++, one_string++ ) {
		

		struct header_parse_result result = parse_header( *one_string );

		struct domain * domain = get_domain_for_header( *one_string, result ); 

		int is_notable = 0;

#define MAP( expect, enumval ) if( !wcscmp( expect, (*one_string)+result.field_start ) ) { \
				domain->notable_field_map[ enumval ] = i;\
				is_notable = 1; }

		if( domain->is_global ) {

			MAP( L"Name", NAME )
			else MAP( L"Given Name", GIVEN_NAME )
			else MAP( L"Family Name", FAMILY_NAME )
			else MAP( L"Name Prefix", NAME_PREFIX )
			else MAP( L"Name Suffix", NAME_SUFFIX )
			else MAP( L"Initials", INITIALS )
			else MAP( L"Occupation", OCCUPATION )

		} else if ( !wcscmp( L"Address", domain->type ) ) {
		} else if ( !wcscmp( L"Organization", domain->type ) ) {
		} else if ( !wcscmp( L"IM", domain->type ) ) {
		} else if ( !wcscmp( L"E-mail", domain->type ) 
					|| !wcscmp( L"Phone", domain->type ) 
					|| !wcscmp( L"Website", domain->type ) 
					|| !wcscmp( L"Event", domain->type ) ) {
		}

		if( !is_notable ) {
			if( !domain->unaccounted_fields ) {
				domain->unaccounted_fields = malloc( sizeof(int) );
				domain->unaccounted_fields[0] = UNUSED;

				domain->unaccounted_field_names = malloc( sizeof(wchar_t *) );
				domain->unaccounted_field_names[0] = NULL;
			}

			int existing_count = 0;
			for(; *(domain->unaccounted_fields + existing_count) != UNUSED; 
				existing_count++ ) /* deliberate empty block */;

			domain->unaccounted_fields = 
				realloc( domain->unaccounted_fields,
					(existing_count+2) * sizeof(int *) );
			domain->unaccounted_fields[ existing_count ] = i;
			domain->unaccounted_fields[ existing_count + 1] = UNUSED; // terminator


			domain->unaccounted_field_names =
				realloc( domain->unaccounted_field_names,
					(existing_count+2) * sizeof(wchar_t *) );
			size_t sz = (result.field_len + 1) * sizeof(wchar_t);
			wchar_t * new = malloc( sz );
			memset( new, 0, sz );
			if( result.is_global ) {
				wcsncpy( new, *one_string, result.field_len+1 );
			} else {
				wcsncpy( new, *one_string + result.field_len, result.field_len+1 );
			}
			domain->unaccounted_field_names[ existing_count ] = new;

			domain->unaccounted_field_names[ existing_count + 1 ] = NULL;
		}
	}

}

static void string_token_parsed( wchar_t * string, int field_index ) {
	if( field_index+1 > strings_count ) {
		if( strings_count ) {
			strings = realloc( strings, (field_index+1) * sizeof( wchar_t * ) );
		} else {
			strings = malloc( sizeof( wchar_t * ) );
		}
		
		// Fill in any gaps if necessary with NULL
		int i;
		for( i=strings_count; i<field_index; i++ ) {
			strings[i] = NULL;
		}

		strings_count = field_index + 1;
	}

	//fwprintf( stderr, L"Added at index %d, string %ls\n", field_index, string );

	strings[ field_index ] = string;
}

void perform_conversion( struct output_config outconf ) {

	strings = NULL;
	strings_count = 0;
	output_fd = outconf.out_fd;


	output_iconv_cd = iconv_open( "UTF-8", "WCHAR_T" );
	if( output_iconv_cd == (iconv_t)-1 ) fwprintf( stderr, L"iconv_open: %s\n", strerror(errno) );

	parse( &line_end_reached, &header_end_reached, 
		&string_token_parsed );

	int status;
	wchar_t * out = L"Hola";
	status = out_printf( L"%ls", out );
	if( status == -1 ) fwprintf( stderr, L"out_printf: %s\n", strerror(errno) );



	status = iconv_close( output_iconv_cd );
	if( status == -1 ) fwprintf( stderr, L"iconv_close: %s\n", strerror(errno) );

}
