INCLUDES = -I$(top_srcdir)/src \
	-I$(top_srcdir)/src/electronics -I$(top_srcdir)/src/electronics/components \
	-I$(top_srcdir)/src/electronics/simulation -I$(top_srcdir)/src/flowparts -I$(top_srcdir)/src/gui \
	-I$(top_srcdir)/src/languages -I$(top_srcdir)/src/mechanics -I$(top_srcdir)/src/micro -Igui \
	$(all_includes)
METASOURCES = AUTO
noinst_LTLIBRARIES = liblanguages.la
liblanguages_la_SOURCES = language.cpp languagemanager.cpp microbe.cpp \
		externallanguage.cpp gpasm.cpp gpdasm.cpp processchain.cpp flowcode.cpp asmparser.cpp \
		sdcc.cpp gplink.cpp gplib.cpp sourceline.cpp picprogrammer.cpp
noinst_HEADERS = externallanguage.h gpasm.h gpdasm.h language.h \
		languagemanager.h microbe.h processchain.h flowcode.h asmparser.h sdcc.h gplink.h gplib.h \
		sourceline.h picprogrammer.h
liblanguages_la_LIBADD = $(top_builddir)/src/gui/libgui.la
