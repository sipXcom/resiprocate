

AUTOMAKE_OPTIONS = foreign
ACLOCAL_AMFLAGS = -I m4

SUBDIRS = rutil
SUBDIRS += resip
if BUILD_REPRO
SUBDIRS += repro
endif
if BUILD_TFM
SUBDIRS += tfm
endif


EXTRA_DIST = $(srcdir)/reSIProcate_10_0.sln
EXTRA_DIST += $(srcdir)/reSIProcate_8_0.sln
EXTRA_DIST += $(srcdir)/reSIProcate_9_0.sln
EXTRA_DIST += $(srcdir)/.tarball-revision
EXTRA_DIST += sipxresiprocate.spec
EXTRA_DIST += RELEASE-PROCESS.txt
EXTRA_DIST += build/configure-android.sh
EXTRA_DIST += $(srcdir)/contrib/cajun/include/cajun/*
