/*
    Copyright (C) 2010 Free Software Foundation

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <wchar.h>
#include <ctype.h>
#include <getopt.h>

#include "input.h"
#include "test.h"
#include "parse.h"
#include "main.h"
#include "../config.h"

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

static void show_usage() {

	static const char * usage = 
		"\n"
		"Convert a contact list exported from GMail (in Google-format .csv) to\n"
		"LDIF, ready for import to an LDAP server.\n"
		"\n"
		"Usage:\n"
		"\n"
		"  " PACKAGE_NAME " --suffix [distinguished name suffix]\n"
		"\n"
		"  " PACKAGE_NAME " --help\n"
		"\n"
		"  " PACKAGE_NAME " --tests-only\n"
		"\n"
		"\n"
		"Example:\n"
		"\n"
		"  " PACKAGE_NAME " --suffix ou=Contacts,dc=example,dc=org <contacts.csv >addscript.ldif\n"
		"\n"
		"\n"
		"See man " PACKAGE_NAME " for more information.\n"
		"\n"
		PACKAGE_NAME " version " PACKAGE_VERSION "\n"
		"Copyright (C) 2010 Free Software Foundation\n"
		"\n"
		"This program comes with ABSOLUTELY NO WARRANTY.\n"
		"This is free software, and you are welcome to redistribute it\n"
		"under certain conditions; visit http://www.gnu.org/licenses/gpl.htm\n"
		"for details.\n"
	;
	
	fprintf( stderr, "%s\n", usage );
}

int main( int argc, char ** argv ) {
	
	struct output_config config;
	memset( &config, 0, sizeof config );

	int parse_success = parse_args( &config, argc, argv );
	if( !parse_success ) return 1;

	if( config.run_tests ) {
		if( config.show_help || config.dn_suffix ) goto error;
		run_tests();
	} else if( config.show_help ) {
		show_usage();
	} else if( config.dn_suffix ) {
		if( config.show_help || config.run_tests ) goto error;
		input_initialize();
		config.out_fd = 1; // stdout
		perform_conversion( config );
		input_destroy();
	} else goto error;
	
	return 0;

	error:
		show_usage();
		return 1;
}
