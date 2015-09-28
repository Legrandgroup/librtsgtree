/**
 * \file utils.h
 * \brief Generic utilities library
 *
 * Commonly used routines shared by all conductor code
 */

#ifndef __UTILS_H__
#define __UTILS_H__

#include <syslog.h>

/**
 * \def plog(priority, format, ...)
 * \brief Logs a message
 * \param priority The log level as defined in the first parameter of syslog(3)
 * \param format A format string, followed by the variable argument list
 *
 * This is a wrapper around syslog(3) call that actually does the same as syslog(3) for now
 */
#define plog syslog

/* Copied from libc assert(), but writes to SYSLOG rather than stderr that is closed for us running as a daemon */
#include <features.h>

/**
 * \def __ASSERT_VOID_CAST
 * \brief Prefix to cast to void a function depending on the compiler type
 *
 * Declare noop functions using __ASSERT_VOID_CAST (0)
 */
#if defined __cplusplus && __GNUC_PREREQ (2,95)
# define __ASSERT_VOID_CAST static_cast<void>
#else
# define __ASSERT_VOID_CAST (void)
#endif

/* void assert (int expression);

   If NDEBUG is defined, do nothing.
   If not, and EXPRESSION is zero, print an error message and abort.  */

/**
 * \def assert(expr)
 * \brief Makes sure the expression is true or terminates the current program
 * \param expr The expression to compare to boolean true (must be !=0 or the program will exit)
 *
 * This macro is used as a safety guard to check the validity of variables or environment conditions before starting to execute code
 * It should be called on all conditions that are not supposed to occur and that are not met in normal execution.
 * If this condition expression is not satisfied, a log will be sent to syslog and the program will terminate immediately (so please use with caution)
 * Usually, this macro should be used on all pointers before dereferencing them and on all conditions that are not explicitely tested by the code (because they should never occur)
 */

/**
 * \def dplog(priority, format, ...)
 * \brief Logs a message if DEBUG preprocessor flag is set (and NDEBUG is not set)
 * \param priority The log level as defined in the first parameter of syslog(3)
 * \param format A format string, followed by the variable argument list
 *
 * This is a wrapper around plog
 */
#ifdef	NDEBUG

# define assert(expr)		(__ASSERT_VOID_CAST (0))
# define dplog				(__ASSERT_VOID_CAST (0))

#else /* Not NDEBUG.  */

#ifdef DEBUG
# define dplog plog
#else
# define dplog				(__ASSERT_VOID_CAST (0))
#endif

/* Lionel: we might one day switch to a multi-level debug macro like the following:
#define dprintf(...) if(globals.debugfile) fprintf(globals.debugfile, __VA_ARGS__)
#define dvprintf(num, ...) if(globals.debugfile && globals.verbose >= num) fprintf(globals.debugfile, __VA_ARGS__)
*/

__BEGIN_DECLS

/* This prints an "Assertion failed" message and aborts.  */
void __assert_syslog_fail (__const char *__assertion, __const char *__file,
                           unsigned int __line, __const char *__function)
     __THROW __attribute__ ((__noreturn__));

__END_DECLS

# define assert(expr) \
  ((expr)? __ASSERT_VOID_CAST (0) : __assert_syslog_fail (__STRING(expr), __FILE__, __LINE__, __ASSERT_FUNCTION))

/**
 * \def __ASSERT_FUNCTION
 * \brief AZ directive returning the name of the current function (depends on the compiler)
 * This macro points to the directive returning the name of the current function (could be __FUNCTION__ for GCC or __func__ etc...)
 */

/* Version 2.4 and later of GCC define a magical variable `__PRETTY_FUNCTION__'
   which contains the name of the function currently being defined.
   This is broken in G++ before version 2.6.
   C9x has a similar variable called __func__, but prefer the GCC one since
   it demangles C++ function names.  */
# if defined __cplusplus ? __GNUC_PREREQ (2, 6) : __GNUC_PREREQ (2, 4)
#   define __ASSERT_FUNCTION	__PRETTY_FUNCTION__
# else
#  if defined __STDC_VERSION__ && __STDC_VERSION__ >= 199901L
#   define __ASSERT_FUNCTION	__func__
#  else
#   define __ASSERT_FUNCTION	((__const char *) 0)
#  endif
# endif

#endif /* NDEBUG.  */

#endif	// __UTILS_H__
