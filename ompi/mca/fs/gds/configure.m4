# -*- shell-script -*-
#
# Copyright (c) 2004-2005 The Trustees of Indiana University and Indiana
#                         University Research and Technology
#                         Corporation.  All rights reserved.
# Copyright (c) 2004-2005 The University of Tennessee and The University
#                         of Tennessee Research Foundation.  All rights
#                         reserved.
# Copyright (c) 2004-2005 High Performance Computing Center Stuttgart,
#                         University of Stuttgart.  All rights reserved.
# Copyright (c) 2004-2012 The Regents of the University of California.
#                         All rights reserved.
# Copyright (c) 2010-2014 Cisco Systems, Inc.  All rights reserved.
# Copyright (c) 2008-2020 University of Houston. All rights reserved.
# $COPYRIGHT$
#
# Additional copyrights may follow
#
# $HEADER$
#

# MCA_fs_gds_CONFIG(action-if-can-compile, 
#                        [action-if-cant-compile])
# ------------------------------------------------
AC_DEFUN([MCA_ompi_fs_gds_CONFIG],[
    AC_CONFIG_FILES([ompi/mca/fs/gds/Makefile])

    OMPI_CHECK_GDS([fs_gds],
                   [fs_gds_happy="yes"],
                   [fs_gds_happy="no"])

    AS_IF([test "$fs_gds_happy" = "yes"],
          [fs_gds_WRAPPER_EXTRA_LDFLAGS="$fs_gds_LDFLAGS"
           fs_gds_WRAPPER_EXTRA_LIBS="$fs_gds_LIBS"
           $1],
          [$2])

    OPAL_SUMMARY_ADD([[OMPIO File Systems]],[[NVIDIA GDS]],[$1],[$fs_gds_happy])

    # substitute in the things needed to build gds
    AC_SUBST([fs_gds_CPPFLAGS])
    AC_SUBST([fs_gds_LDFLAGS])
    AC_SUBST([fs_gds_LIBS])
])dnl
