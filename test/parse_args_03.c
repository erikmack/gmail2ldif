#include <assert.h>
#include <string.h>

#include "args.h"

int main() {

	struct output_config config;
	memset( &config, 0, sizeof config );

	char * argv[] = { 
		"dummy_program_name",
		"--help",
		"--suffix=ou=Contacts,dc=example,dc=org", 
	};

	int success = parse_args( &config, sizeof argv / sizeof argv[0], argv );
	success = success && config.dn_suffix;
	success = success && !strcmp("ou=Contacts,dc=example,dc=org",config.dn_suffix);
	success = success && config.show_help;
	success = success && !config.run_tests;

	assert(success);
	return 0;
}
