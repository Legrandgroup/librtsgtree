#ifdef USE_CPPUTEST
#include <CppUTest/TestHarness.h>
#include <CppUTest/CommandLineTestRunner.h>
#else
#include <stdio.h>
#define TEST_GROUP(x)
#define TEST(group, func) void func()
#define FAIL(message, ...) do { fprintf(stderr, "%s():%d: ", __func__, __LINE__); fprintf(stderr, message, ## __VA_ARGS__); exit(1); } while(0);
#endif
