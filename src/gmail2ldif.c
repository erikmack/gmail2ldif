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

#include "input.h"
#include "parse.h"
#include "args.h"
#include "../config.h"

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

	if( config.show_help ) {
		show_usage();
	} else if( config.dn_suffix ) {
		if( config.show_help ) goto error;
		input_initialize();
		perform_conversion( config );
		input_destroy();
	} else goto error;
	
	return 0;

	error:
		show_usage();
		return 1;
}
