
SUBDIRS = data

TESTS = \
	parse_args_01 \
	parse_args_02 \
	parse_args_03 \
	input_short_ascii.sh \
	input_short_utf16.sh \
	input_short_header_ascii.sh \
	input_short_header_utf16.sh

check_PROGRAMS = \
	parse_args_01 \
	parse_args_02 \
	parse_args_03 \
	input_short \
	input_short_header

CLEANFILES = \
	input_short_ascii.sh \
	input_short_utf16.sh \
	input_short_header_ascii.sh \
	input_short_header_utf16.sh

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

input_short_ascii.sh: 
	echo "./input_short <$(top_srcdir)/test/data/short-ascii.txt" > $@
	chmod +x $@

input_short_utf16.sh: 
	echo "./input_short <$(top_srcdir)/test/data/short-utf16le.txt" > $@
	chmod +x $@


input_short_header_ascii.sh: 
	echo "./input_short_header <$(top_srcdir)/test/data/header-ascii.csv" > $@
	chmod +x $@

input_short_header_utf16.sh: 
	echo "./input_short_header <$(top_srcdir)/test/data/header-utf16.csv" > $@
	chmod +x $@



