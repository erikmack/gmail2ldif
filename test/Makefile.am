
SUBDIRS = data

TESTS = \
	parse_args_01 \
	parse_args_02 \
	parse_args_03 \
	input_short_ascii.sh \
	input_short_utf16.sh \
	input_short_header_ascii.sh \
	input_short_header_utf16.sh \
	lex_short_ascii.sh \
	lex_short_utf16.sh \
	lex_short_ascii_separators.sh \
	lex_header_ascii.sh \
	lex_header_utf16.sh \
	lex_short_csv.sh \
	parse_short_string.sh \
	parse_short_csv.sh \
	parse_header_01 \
	parse_header_02 \
	parse_header_03 \
	output_ascii_one.sh \
	output_utf16_one.sh

check_PROGRAMS = \
	parse_args_01 \
	parse_args_02 \
	parse_args_03 \
	input_short \
	input_short_header \
	lex_short \
	lex_short_separators \
	lex_header \
	lex_short_csv \
	parse_short_string \
	parse_short_csv \
	parse_header_01 \
	parse_header_02 \
	parse_header_03


CLEANFILES = \
	input_short_ascii.sh \
	input_short_utf16.sh \
	input_short_header_ascii.sh \
	input_short_header_utf16.sh \
	lex_short_ascii.sh \
	lex_short_utf16.sh \
	lex_short_ascii_separators.sh \
	lex_header_ascii.sh \
	lex_header_utf16.sh \
	lex_short_csv.sh \
	parse_short_string.sh \
	parse_short_csv.sh \
	output_ascii_one.sh \
	output_utf16_one.sh \
	tmp_ascii.ldif \
	tmp_utf16.ldif


parse_args_01_LDADD = ../src/libcore.a
parse_args_01_CPPFLAGS = -I$(top_srcdir)/src
parse_args_02_LDADD = ../src/libcore.a
parse_args_02_CPPFLAGS = -I$(top_srcdir)/src
parse_args_03_LDADD = ../src/libcore.a
parse_args_03_CPPFLAGS = -I$(top_srcdir)/src
input_short_LDADD = ../src/libcore.a
input_short_CPPFLAGS = -I$(top_srcdir)/src
input_short_header_LDADD = ../src/libcore.a
input_short_header_CPPFLAGS = -I$(top_srcdir)/src
lex_short_LDADD = ../src/libcore.a
lex_short_CPPFLAGS = -I$(top_srcdir)/src
lex_short_separators_LDADD = ../src/libcore.a
lex_short_separators_CPPFLAGS = -I$(top_srcdir)/src
lex_header_LDADD = ../src/libcore.a
lex_header_CPPFLAGS = -I$(top_srcdir)/src
lex_short_csv_LDADD = ../src/libcore.a
lex_short_csv_CPPFLAGS = -I$(top_srcdir)/src
parse_short_string_LDADD = ../src/libcore.a
parse_short_string_CPPFLAGS = -I$(top_srcdir)/src
parse_short_csv_LDADD = ../src/libcore.a
parse_short_csv_CPPFLAGS = -I$(top_srcdir)/src
parse_header_01_LDADD = ../src/libcore.a
parse_header_01_CPPFLAGS = -I$(top_srcdir)/src
parse_header_02_LDADD = ../src/libcore.a
parse_header_02_CPPFLAGS = -I$(top_srcdir)/src
parse_header_03_LDADD = ../src/libcore.a
parse_header_03_CPPFLAGS = -I$(top_srcdir)/src

input_short_ascii.sh: 
	echo "./input_short$(EXEEXT) <$(top_srcdir)/test/data/short-ascii.txt" > $@
	chmod +x $@

input_short_utf16.sh: 
	echo "./input_short$(EXEEXT) <$(top_srcdir)/test/data/short-utf16le.txt" > $@
	chmod +x $@


input_short_header_ascii.sh: 
	echo "./input_short_header$(EXEEXT) <$(top_srcdir)/test/data/header-ascii.csv" > $@
	chmod +x $@

input_short_header_utf16.sh: 
	echo "./input_short_header$(EXEEXT) <$(top_srcdir)/test/data/header-utf16.csv" > $@
	chmod +x $@


lex_short_ascii.sh: 
	echo "./lex_short$(EXEEXT) <$(top_srcdir)/test/data/short-ascii.txt" > $@
	chmod +x $@

lex_short_utf16.sh: 
	echo "./lex_short$(EXEEXT) <$(top_srcdir)/test/data/short-utf16le.txt" > $@
	chmod +x $@

lex_short_ascii_separators.sh: 
	echo "./lex_short_separators$(EXEEXT) <$(top_srcdir)/test/data/short-ascii-separators.txt" > $@
	chmod +x $@


lex_header_ascii.sh: 
	echo "./lex_header$(EXEEXT) <$(top_srcdir)/test/data/header-ascii.csv" > $@
	chmod +x $@

lex_header_utf16.sh: 
	echo "./lex_header$(EXEEXT) <$(top_srcdir)/test/data/header-utf16.csv" > $@
	chmod +x $@

lex_short_csv.sh: 
	echo "./lex_short_csv$(EXEEXT) <$(top_srcdir)/test/data/short-ascii-csv.csv" > $@
	chmod +x $@


parse_short_string.sh: 
	echo "./parse_short_string$(EXEEXT) <$(top_srcdir)/test/data/short-ascii-two.txt" > $@
	chmod +x $@

parse_short_csv.sh: 
	echo "./parse_short_csv$(EXEEXT) <$(top_srcdir)/test/data/short-ascii-csv.csv" > $@
	chmod +x $@


output_ascii_one.sh:
	echo "../src/gmail2ldif$(EXEEXT) --suffix 'ou=Contacts,dc=example,dc=org' <$(top_srcdir)/test/data/complete-ascii.csv >tmp_ascii.ldif" > $@
	echo "diff $(top_srcdir)/test/data/complete-ascii-expect.ldif tmp_ascii.ldif >/dev/null || exit 1" >> $@
	echo "rm -f tmp_ascii.ldif" >> $@
	chmod +x $@

output_utf16_one.sh:
	echo "../src/gmail2ldif$(EXEEXT) --suffix 'ou=Contacts,dc=example,dc=org' <$(top_srcdir)/test/data/complete-utf16.csv >tmp_utf16.ldif || exit 1" > $@
	echo "diff $(top_srcdir)/test/data/complete-utf16-expect.ldif tmp_utf16.ldif >/dev/null || exit 1" >> $@
	echo "rm -f tmp_utf16.ldif" >> $@
	chmod +x $@
