include config/utility.am
include config/project.am

AUTOMAKE_OPTIONS = foreign
ACLOCAL_AMFLAGS = -I m4

SUBDIRS = rutil
SUBDIRS += resip
if BUILD_REPRO
SUBDIRS += repro
endif

if BUILD_TFM
all: build-netxx

build-netxx:
	cd $(srcdir)/contrib/Netxx-0.3.2/ && \
	perl configure.pl --disable-examples --disable-shared && \
	make

SUBDIRS += tfm
endif


EXTRA_DIST += $(srcdir)/reSIProcate_10_0.sln
EXTRA_DIST += $(srcdir)/reSIProcate_8_0.sln
EXTRA_DIST += $(srcdir)/reSIProcate_9_0.sln
EXTRA_DIST += sipxresiprocate.spec
EXTRA_DIST += RELEASE-PROCESS.txt
EXTRA_DIST += build/configure-android.sh
EXTRA_DIST += $(srcdir)/contrib/cajun/include/cajun/*
