# Makefile.in generated by unsermake
####################################

top_builddir = .
srcdir = .
ACLOCAL = ${SHELL} /home/david/devel/ktechlab/trunk/admin/missing --run aclocal-1.9
AMDEP_FALSE = #
AMDEP_TRUE = 
AMTAR = ${SHELL} /home/david/devel/ktechlab/trunk/admin/missing --run tar
AR = ar
ARTSCCONFIG = /usr/bin/artsc-config
AUTOCONF = $(SHELL) $(top_srcdir)/admin/cvs.sh configure || touch configure
AUTODIRS = 
AUTOHEADER = ${SHELL} /home/david/devel/ktechlab/trunk/admin/missing --run autoheader
AUTOMAKE = ${SHELL} /home/david/devel/ktechlab/trunk/admin/missing --run automake-1.9
AWK = gawk
CC = gcc
CCDEPMODE = depmode=gcc3
CFLAGS = -std=iso9899:1990 -W -Wall -Wchar-subscripts -Wshadow -Wpointer-arith -Wmissing-prototypes -Wwrite-strings -D_XOPEN_SOURCE=500 -D_BSD_SOURCE -O2   -Wformat-security -Wmissing-format-attribute
CONF_FILES =  $(top_srcdir)/./admin/configure.in.min $(top_srcdir)/configure.in.in
CPP = gcc -E
CPPFLAGS =  -DQT_THREAD_SUPPORT  -D_REENTRANT
CXX = g++
CXXCPP = g++ -E
CXXDEPMODE = depmode=gcc3
CXXFLAGS = -Wno-long-long -Wundef -ansi -D_XOPEN_SOURCE=500 -D_BSD_SOURCE -Wcast-align -Wconversion -Wchar-subscripts -Wall -W -Wpointer-arith -O2 -Wformat-security -Wmissing-format-attribute -Wno-non-virtual-dtor -fno-exceptions -fno-check-new -fno-common
CYGPATH_W = echo
DCOPIDL = /usr/bin/dcopidl
DCOPIDL2CPP = /usr/bin/dcopidl2cpp
DCOPIDLNG = /usr/bin/dcopidlng
DCOP_DEPENDENCIES = $(DCOPIDL) $(DCOPIDLNG)
DEFS = -DHAVE_CONFIG_H
DEPDIR = .deps
ECHO = echo
ECHO_C = 
ECHO_N = -n
ECHO_T = 
EGREP = grep -E
ENABLE_PERMISSIVE_FLAG = -fpermissive
EXEEXT = 
F77 = 
FFLAGS = 
FRAMEWORK_COREAUDIO = 
GMSGFMT = /usr/bin/msgfmt
HAVE_GCC_VISIBILITY = 0
INSTALL_DATA = ${INSTALL} -m 644
INSTALL_PROGRAM = ${INSTALL} $(INSTALL_STRIP_FLAG)
INSTALL_SCRIPT = ${INSTALL}
INSTALL_STRIP_PROGRAM = ${SHELL} $(install_sh) -c -s
KCFG_DEPENDENCIES = $(KCONFIG_COMPILER)
KCONFIG_COMPILER = /usr/bin/kconfig_compiler
KDECONFIG = /usr/bin/kde-config
KDE_CHECK_PLUGIN = $(KDE_PLUGIN) -rpath $(libdir)
KDE_EXTRA_RPATH = 
KDE_INCLUDES = 
KDE_LDFLAGS = -L/usr/lib
KDE_MT_LDFLAGS = 
KDE_MT_LIBS = -lpthread
KDE_NO_UNDEFINED = -Wl,--no-undefined -Wl,--allow-shlib-undefined
KDE_PLUGIN = -avoid-version -module -no-undefined $(KDE_NO_UNDEFINED) $(KDE_RPATH) $(KDE_MT_LDFLAGS)
KDE_RPATH = -R $(libdir) -R $(kde_libraries) -R $(qt_libraries) -R $(x_libraries)
KDE_USE_CLOSURE_FALSE = 
KDE_USE_CLOSURE_TRUE = #
KDE_USE_FINAL_FALSE = 
KDE_USE_FINAL_TRUE = #
KDE_USE_FPIE = -fpie
KDE_USE_NMCHECK_FALSE = 
KDE_USE_NMCHECK_TRUE = #
KDE_USE_PIE = -pie
KDE_XSL_STYLESHEET = /usr/share/apps/ksgmltools2/customization/kde-chunk.xsl
LDFLAGS = 
LDFLAGS_AS_NEEDED = 
LDFLAGS_NEW_DTAGS = 
LIBCOMPAT = 
LIBCRYPT = -lcrypt
LIBDL = -ldl
LIBJPEG = -ljpeg
LIBOBJS = 
LIBPNG = -lpng -lz -lm
LIBPTHREAD = -lpthread
LIBRESOLV = -lresolv
LIBS = 
LIBSM = -lSM -lICE
LIBSOCKET = 
LIBTOOL = $(SHELL) $(top_builddir)/libtool --silent
LIBUCB = 
LIBUTIL = -lutil
LIBZ = -lz
LIB_GPSIM = -lreadline -lgpsim -lgpsim_eXdbm -lgpsimcli
LIB_KAB = -lkab
LIB_KABC = -lkabc
LIB_KDECORE = -lkdecore
LIB_KDED = 
LIB_KDEPIM = -lkdepim
LIB_KDEPRINT = -lkdeprint
LIB_KDEUI = -lkdeui
LIB_KDNSSD = -lkdnssd
LIB_KFILE = -lkio
LIB_KFM = 
LIB_KHTML = -lkhtml
LIB_KIMPROXY = -lkimproxy
LIB_KIO = -lkio
LIB_KJS = -lkjs
LIB_KNEWSTUFF = -lknewstuff
LIB_KPARTS = -lkparts
LIB_KSPELL = -lkspell
LIB_KSYCOCA = -lkio
LIB_KUNITTEST = -lkunittest
LIB_KUTILS = -lkutils
LIB_POLL = 
LIB_QPE = 
LIB_QT = -lqt-mt  $(LIBZ) $(LIBPNG) -lXext $(LIB_X11) $(LIBSM) -lpthread
LIB_SMB = -lsmb
LIB_X11 = -lX11 $(LIBSOCKET)
LIB_XEXT = -lXext
LIB_XRENDER = 
LN_S = ln -s
LTLIBOBJS = 
MAKEINFO = ${SHELL} /home/david/devel/ktechlab/trunk/admin/missing --run makeinfo
MAKEKDEWIDGETS = 
MCOPIDL = /usr/bin/mcopidl
MEINPROC = /usr/bin/meinproc
MOC = /usr/share/qt3/bin/moc
MSGFMT = /usr/bin/msgfmt
NOOPT_CFLAGS = -O0
NOOPT_CXXFLAGS = -O0
OBJEXT = o
PACKAGE = ktechlab
PACKAGE_BUGREPORT = 
PACKAGE_NAME = 
PACKAGE_STRING = 
PACKAGE_TARNAME = 
PACKAGE_VERSION = 
PATH_SEPARATOR = :
PERL = /usr/bin/perl
QTE_NORTTI = 
QT_INCLUDES = -I/usr/share/qt3/include
QT_LDFLAGS = -L/usr/share/qt3/lib
RANLIB = ranlib
SET_MAKE = 
SHELL = /bin/sh
STRIP = strip
TOPSUBDIRS =  doc examples icons microbe po src
UIC = /usr/share/qt3/bin/uic -L $(kde_widgetdir) -nounload
UIC_TR = tr2i18n
USER_INCLUDES = 
USER_LDFLAGS = 
USE_EXCEPTIONS = -fexceptions
USE_RTTI = 
USE_THREADS = 
VERSION = 0.3
WOVERLOADED_VIRTUAL = 
XGETTEXT = /usr/bin/xgettext
XMLLINT = /usr/bin/xmllint
X_EXTRA_LIBS = 
X_INCLUDES = -I.
X_LDFLAGS = -L/usr/X11R6/lib
X_PRE_LIBS = 
X_RPATH = -R $(x_libraries)
ac_ct_AR = ar
ac_ct_CC = gcc
ac_ct_CXX = g++
ac_ct_F77 = 
ac_ct_RANLIB = ranlib
ac_ct_STRIP = strip
all_includes = -I/usr/share/qt3/include -I. 
all_libraries = -L/usr/lib -L/usr/share/qt3/lib -L/usr/X11R6/lib   
am__fastdepCC_FALSE = #
am__fastdepCC_TRUE = 
am__fastdepCXX_FALSE = #
am__fastdepCXX_TRUE = 
am__include = include
am__leading_dot = .
am__quote = 
am__tar = ${AMTAR} chof - "$$tardir"
am__untar = ${AMTAR} xf -
bindir = ${exec_prefix}/bin
build = x86_64-unknown-linux-gnu
build_alias = 
build_cpu = x86_64
build_os = linux-gnu
build_vendor = unknown
datadir = ${prefix}/share
depcomp = $(SHELL) $(top_srcdir)/admin/depcomp

exec_prefix = ${prefix}
glib_cflags = -I/usr/include/glib-2.0 -I/usr/lib/glib-2.0/include  
host = x86_64-unknown-linux-gnu
host_alias = 
host_cpu = x86_64
host_os = linux-gnu
host_vendor = unknown
include_ARTS_FALSE = #
include_ARTS_TRUE = 
include_x11_FALSE = #
include_x11_TRUE = 
includedir = ${prefix}/include
infodir = ${prefix}/info
install_sh = /home/david/devel/ktechlab/trunk/admin/install-sh
kde_appsdir = ${datadir}/applnk
kde_bindir = ${exec_prefix}/bin
kde_confdir = ${datadir}/config
kde_datadir = ${datadir}/apps
kde_htmldir = ${prefix}/share/doc/kde/HTML
kde_icondir = ${datadir}/icons
kde_includes = /usr/include
kde_kcfgdir = ${datadir}/config.kcfg
kde_libraries = /usr/lib
kde_libs_htmldir = /usr/share/doc/kde/HTML
kde_libs_prefix = /usr
kde_locale = ${datadir}/locale
kde_mimedir = ${datadir}/mimelnk
kde_moduledir = ${libdir}/kde3
kde_qtver = 3
kde_servicesdir = ${datadir}/services
kde_servicetypesdir = ${datadir}/servicetypes
kde_sounddir = ${datadir}/sounds
kde_styledir = ${libdir}/kde3/plugins/styles
kde_templatesdir = ${datadir}/templates
kde_wallpaperdir = ${datadir}/wallpapers
kde_widgetdir = /usr/lib/kde3/plugins/designer
kdeinitdir = $(kde_moduledir)
libdir = ${exec_prefix}/lib
libexecdir = ${exec_prefix}/libexec
localstatedir = ${prefix}/var
mandir = ${prefix}/man
mkdir_p = mkdir -p --
mkinstalldirs = $(SHELL) $(top_srcdir)/admin/mkinstalldirs
oldincludedir = /usr/include
prefix = /usr
program_transform_name = s,x,x,
qt_includes = /usr/share/qt3/include
qt_libraries = /usr/share/qt3/lib
sbindir = ${exec_prefix}/sbin
sharedstatedir = ${prefix}/com
sysconfdir = ${prefix}/etc
target = x86_64-unknown-linux-gnu
target_alias = 
target_cpu = x86_64
target_os = linux-gnu
target_vendor = unknown
top_srcdir = .
unsermake_enable_pch_FALSE = 
unsermake_enable_pch_TRUE = #
x_includes = .
x_libraries = /usr/X11R6/lib
xdg_appsdir = ${datadir}/applications/kde
xdg_directorydir = ${datadir}/desktop-directories
xdg_menudir = ${sysconfdir}/xdg/menus
transform = s,x,x,
INSTALL = /usr/bin/install -c -p
.FORWARDS: all install install-data install-exec clean check force-reedit

all:
	@echo 'WARNING: use unsermake instead of make or use a wrapper script, e.g. makeobj!!!'
	unsermake all

install:
	@echo 'WARNING: use unsermake instead of make or use a wrapper script, e.g. makeobj!!!'
	unsermake install

install-data:
	@echo 'WARNING: use unsermake instead of make or use a wrapper script, e.g. makeobj!!!'
	unsermake install-data

install-exec:
	@echo 'WARNING: use unsermake instead of make or use a wrapper script, e.g. makeobj!!!'
	unsermake install-exec

clean:
	@echo 'WARNING: use unsermake instead of make or use a wrapper script, e.g. makeobj!!!'
	unsermake clean

check:
	@echo 'WARNING: use unsermake instead of make or use a wrapper script, e.g. makeobj!!!'
	unsermake check

force-reedit:
	@echo 'WARNING: use unsermake instead of make or use a wrapper script, e.g. makeobj!!!'
	unsermake force-reedit

$(top_builddir)/Makefile: $(top_builddir)/config.status $(top_srcdir)/Makefile.in
	cd $(top_builddir) && $(SHELL) ./config.status Makefile

$(top_builddir)/config.h: $(top_builddir)/stamp-h1
	@if test ! -f $(top_builddir)/config.h; then \
	   rm -f $(top_builddir)/stamp-h1; \
	   cd $(top_builddir)/ && $(MAKE) stamp-h1; \
	else :; fi

$(top_builddir)/stamp-h1: $(top_srcdir)/config.h.in $(top_builddir)/config.status
	@rm -f $(top_builddir)/stamp-h1
	cd $(top_builddir) && $(SHELL) ./config.status config.h

$(top_srcdir)/config.h.in: $(top_srcdir)/configure.in
	cd $(top_srcdir) && $(AUTOHEADER)
	rm -f $(top_builddir)/stamp-h1
	touch $(top_srcdir)/config.h.in

all: $(top_builddir)/Makefile
distclean: distclean-hdr-top
distclean-hdr-top: 
	-rm -f $(top_builddir)/config.h $(top_builddir)/stamp-h1

final: 
	@echo no programs available to recreate as final. Call in subdirs

.PHONY: all distclean distclean-hdr-top final