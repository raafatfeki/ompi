#
# Copyright (c) 2004-2005 The Trustees of Indiana University and Indiana
#                         University Research and Technology
#                         Corporation.  All rights reserved.
# Copyright (c) 2004-2005 The University of Tennessee and The University
#                         of Tennessee Research Foundation.  All rights
#                         reserved.
# Copyright (c) 2004-2005 High Performance Computing Center Stuttgart,
#                         University of Stuttgart.  All rights reserved.
# Copyright (c) 2004-2005 The Regents of the University of California.
#                         All rights reserved.
# Copyright (c) 2008-2020 University of Houston. All rights reserved.
# $COPYRIGHT$
#
# Additional copyrights may follow
#
# $HEADER$
#

# Make the output library in this directory, and name it either
# mca_<type>_<name>.la (for DSO builds) or libmca_<type>_<name>.la
# (for static builds).

if MCA_BUILD_ompi_fbtl_gds_DSO
component_noinst =
component_install = mca_fbtl_gds.la
else
component_noinst = libmca_fbtl_gds.la
component_install =
endif

AM_CPPFLAGS = $(fbtl_gds_CPPFLAGS)

mcacomponentdir = $(ompilibdir)
mcacomponent_LTLIBRARIES = $(component_install)
mca_fbtl_gds_la_SOURCES = $(sources)
mca_fbtl_gds_la_LDFLAGS = -module -avoid-version $(fbtl_gds_LDFLAGS)
mca_fbtl_gds_la_LIBADD = $(top_builddir)/ompi/lib@OMPI_LIBMPI_NAME@.la \
                         $(OMPI_TOP_BUILDDIR)/ompi/mca/fbtl/posix/libmca_fbtl_posix.la  \
                         $(fbtl_gds_LIBS)

noinst_LTLIBRARIES = $(component_noinst)
libmca_fbtl_gds_la_SOURCES = $(sources)
libmca_fbtl_gds_la_LDFLAGS = -module -avoid-version $(fbtl_gds_LDFLAGS)
libmca_fbtl_gds_la_LIBADD = $(fbtl_gds_LIBS)

# Source files

sources = \
        fbtl_gds.h \
        fbtl_gds.c \
        fbtl_gds_component.c \
        fbtl_gds_preadv.c \
        fbtl_gds_ipreadv.c \
        fbtl_gds_pwritev.c \
        fbtl_gds_ipwritev.c
