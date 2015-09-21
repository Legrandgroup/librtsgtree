/**
 * \file utils.c
 */

#include "utils.h"

/**
 * \brief Send a syslog message notifying that an assertion failed
 * \param __assertion The assertion code itself
 * \param __file The file where this assertion code is located
 * \param __line The line where this assertion code is located
 * \param __function The function that called this assertion
 *
 * This is a home-made assertion function that will send an error message in the syslog (whereas the standard assert() sends to stderr)
 * This function is called by the assert() macro defined in utils.h
 */
inline void __assert_syslog_fail(__const char *__assertion, __const char *__file, unsigned int __line, __const char *__function) {

	plog(LOG_CRIT, "%s:%u: %s%sAssertion `%s' failed.\n", __file, __line, __function ? __function : "", __function ? ": " : "", __assertion);
	abort();
}
