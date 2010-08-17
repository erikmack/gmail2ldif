
#include <stdio.h>
#include <ctype.h>
#include <wchar.h>
#include <getopt.h>

#include "args.h"

int fwprintf( FILE * stream, const wchar_t * format, ...);

int parse_args( struct output_config * config, int argc, char ** argv ) {

	// suppress built-in error message, we'll use our own
	opterr = 0;

	// resets parser, since unit tests call this several times
	optind = 0;

	static struct option long_options[] =
	{
		{"help",    no_argument, 0, 'h'},
		{"tests-only",    no_argument, 0, 't'},
		{"suffix",    required_argument, 0, 's'},
		{0, 0, 0, 0}
	};
	int option_index = 0;

	int c;
	while( (c=getopt_long(argc, argv, "s:th", long_options, &option_index)) != -1 )
		switch(c) {
		case 's':	// suffix
			config->dn_suffix = optarg;
			break;
		case 'h':	// help/usage
			config->show_help = 1;
			break;
		case 't':	// run tests
			config->run_tests = 1;
			break;
		case '?':	// invalid option argument
			if( optopt == 's' )
				fwprintf( stderr, L"option -%c requires a dn suffix (ex. \"ou=Contacts,dc=example,dc=org\")\n", optopt );
			else if (isprint (optopt))
				fwprintf (stderr, L"Unknown option -%c.\n", optopt);
			else fwprintf (stderr, L"Unknown option character \\x%x.\n", optopt);

			return 0;
		default:
			return 0;
		}

	return 1;
}

