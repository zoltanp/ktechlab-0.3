INCLUDES = -I$(top_srcdir)/src -I$(top_srcdir)/src/electronics \
	-I$(top_srcdir)/src/electronics/components -I$(top_srcdir)/src/electronics/simulation \
	-I$(top_srcdir)/src/flowparts -I$(top_srcdir)/src/gui -I$(top_srcdir)/src/languages \
	-I$(top_srcdir)/src/mechanics -I$(top_srcdir)/src/micro $(glib_cflags) $(all_includes)

METASOURCES = AUTO

SUBDIRS = 

noinst_LTLIBRARIES = libtechmath.la

libtechmath_la_SOURCES = matrix.cpp qmatrix.cpp qvector.cpp

libtechmath_la_LIBADD =


noinst_HEADERS = matrix.h qmatrix.h qvector.h
libtechmath_la_LDFLAGS = -no-undefined
