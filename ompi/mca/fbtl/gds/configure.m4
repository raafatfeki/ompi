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

# MCA_fbtl_gds_CONFIG(action-if-can-compile,
#                        [action-if-cant-compile])
# ------------------------------------------------
AC_DEFUN([MCA_ompi_fbtl_gds_CONFIG],[
    # AC_REQUIRE([MCA_ompi_fbtl_posix_CONFIG])
    AC_CONFIG_FILES([ompi/mca/fbtl/gds/Makefile])

    OMPI_CHECK_GDS([fbtl_gds],
                   [fbtl_gds_happy="yes"],
                   [fbtl_gds_happy="no"])

    AS_IF([test "$fbtl_gds_happy" = "yes"],
          [$1],
          [$2])

    # substitute in the things needed to build gds
    AC_SUBST([fbtl_gds_CPPFLAGS])
    AC_SUBST([fbtl_gds_LDFLAGS])
    AC_SUBST([fbtl_gds_LIBS])

])dnl
