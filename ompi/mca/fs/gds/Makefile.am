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
# Copyright (c) 2008-2018 University of Houston. All rights reserved.
# Copyright (c) 2017      IBM Corporation.  All rights reserved.
# $COPYRIGHT$
#
# Additional copyrights may follow
#
# $HEADER$
#

# Make the output library in this directory, and name it either
# mca_<type>_<name>.la (for DSO builds) or libmca_<type>_<name>.la
# (for static builds).

if MCA_BUILD_ompi_fs_gds_DSO
component_noinst =
component_install = mca_fs_gds.la
else
component_noinst = libmca_fs_gds.la
component_install =
endif

AM_CPPFLAGS = $(fs_gds_CPPFLAGS)

mcacomponentdir = $(ompilibdir)
mcacomponent_LTLIBRARIES = $(component_install)
mca_fs_gds_la_SOURCES = $(sources)
mca_fs_gds_la_LDFLAGS = -module -avoid-version $(fs_gds_LDFLAGS)
mca_fs_gds_la_LIBADD = $(top_builddir)/ompi/lib@OMPI_LIBMPI_NAME@.la $(fs_gds_LIBS)

noinst_LTLIBRARIES = $(component_noinst)
libmca_fs_gds_la_SOURCES = $(sources)
libmca_fs_gds_la_LDFLAGS = -module -avoid-version $(fs_gds_LDFLAGS)
libmca_fs_gds_la_LIBADD = $(fs_gds_LIBS)

# Source files

sources = \
        fs_gds.h \
        fs_gds.c \
        fs_gds_component.c \
        fs_gds_file_open.c \
        fs_gds_file_close.c
