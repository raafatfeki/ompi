dnl -*- shell-script -*-
dnl
dnl Copyright (c) 2004-2005 The Trustees of Indiana University and Indiana
dnl                         University Research and Technology
dnl                         Corporation.  All rights reserved.
dnl Copyright (c) 2004-2005 The University of Tennessee and The University
dnl                         of Tennessee Research Foundation.  All rights
dnl                         reserved.
dnl Copyright (c) 2004-2018 High Performance Computing Center Stuttgart,
dnl                         University of Stuttgart.  All rights reserved.
dnl Copyright (c) 2004-2006 The Regents of the University of California.
dnl                         All rights reserved.
dnl Copyright (c) 2022      University of Houston. All rights reserved.
dnl $COPYRIGHT$
dnl
dnl Additional copyrights may follow
dnl
dnl $HEADER$
dnl

# OMPI_CHECK_GDS(prefix, [action-if-found], [action-if-not-found])
# --------------------------------------------------------
# check if GDS support can be found.  sets prefix_{CPPFLAGS,
# LDFLAGS, LIBS} as needed and runs action-if-found if there is
# support, otherwise executes action-if-not-found

AC_DEFUN([OMPI_CHECK_GDS],[
    OPAL_VAR_SCOPE_PUSH([gds_CPPFLAGS_save gds_LDFLAGS_save gds_LIBS_save])

    AC_REQUIRE([OPAL_CHECK_CUDA])

    gds_CPPFLAGS_save="$CPPFLAGS"
    gds_LDFLAGS_save="$LDFLAGS"
    gds_LIBS_save="$LIBS"

    # Check to see if user wants GDS support
    AC_ARG_WITH([gds],
                [AS_HELP_STRING([--with-gds(=DIR)],
                [Build gds support, optionally adding DIR/include])])
    AC_MSG_CHECKING([if --with-gds is set])
    AC_ARG_WITH([gds-libdir],
                [AS_HELP_STRING([--with-gds-libdir=DIR],
                                [Search for CUDA libraries in DIR])])

    ompi_check_gds_happy="yes"
    AS_IF([test "$with_gds" = "no"],
          [ompi_check_gds_happy="no"
           AC_MSG_RESULT([no (--with-gds=$with_gds)])],
          [AC_MSG_RESULT([yes (--with-gds=$with_gds)])
           AS_IF([test "$opal_check_cuda_happy" = "no"],
                 [AC_MSG_WARN([Cuda support is disabled.])
                  AC_MSG_WARN([Please set --with-cuda to enable gds support.]) 
                  AC_MSG_ERROR([Cannot continue.])],
                 [AS_IF([test "$with_gds" = "yes" || test "x$with_gds" = "x"],
                        [with_gds="$opal_cuda_incdir/../" 
                        AC_MSG_WARN([GDS support is looking into $with_gds.])]
                        )]
                )]
        )

    OAC_CHECK_PACKAGE([gds],
                      [$1],
                      [cufile.h],
                      [cufile],
                      [cuFileDriverOpen],
                      [ompi_check_gds_happy="yes"],
                      [ompi_check_gds_happy="no"])

    AS_IF([test "$ompi_check_gds_happy" = "yes"],
          [$2],
          [AS_IF([test ! -z "$with_gds" && test "$with_gds" != "no"],
                 [AC_MSG_ERROR([GDS support requested but not found.  Aborting])])
           $3])

    OPAL_VAR_SCOPE_POP
])
