#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.


AC_PREREQ(2.61)
AC_INIT(chessd, alpha , BUG-REPORT-ADDRESS)
AM_INIT_AUTOMAKE(chessd,alpha)
AC_CONFIG_SRCDIR([src/TourneyManager.hh])
#AC_CONFIG_HEADER([config.h])
# Checks for programs.
AC_PROG_CXX
AC_PROG_CC

#CFLAGS=
#CXXFLAGS=

# Checks for libraries.
# FIXME: Replace `main' with a function in `-lboost_date_time':
AC_CHECK_LIB([boost_date_time], [main])
# FIXME: Replace `main' with a function in `-liksemel':
AC_CHECK_LIB([iksemel], [main])
# FIXME: Replace `main' with a function in `-lpthread':
AC_CHECK_LIB([pthread], [main])
# FIXME: Replace `main' with a function in `-lrt':
AC_CHECK_LIB([rt], [main])
# FIXME: Replace `main' with a function in `-lpqxx':
AC_CHECK_LIB([pqxx], [main])

#PKG_CHECK_MODULES(chessd,[libboost_date_time])
#PKG_CHECK_MODULES(chessd,[libboost_date_time-1.0 >= 1.34 libiksemel >= 1.2 ])
PKG_CHECK_MODULES(chessd,[iksemel >= 1.3])

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([stdint.h stdlib.h sys/time.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST
AC_C_INLINE
AC_TYPE_SIZE_T
AC_HEADER_TIME
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_C_VOLATILE

# Checks for library functions.
AC_TYPE_SIGNAL
AC_CHECK_FUNCS([gettimeofday pow sqrt strdup])

AC_CONFIG_FILES([Makefile src/Makefile])
AC_OUTPUT

