/*
 * \file test_librtsgtree.c
 *
 * TODO: Use cpputest for unit testing
 */

#include <stdio.h>
#include <stdlib.h>

#include <assert.h>	// For assert()
#include "node.h"

int unit_tests_uint128(int argc, char* argv[]);	// Declaration of uint128 unit test procedure (see uint128_arithm_tests.c)
int unit_tests_node(int argc, char* argv[]);	// Declaration of node unit test procedure (see node_tests.c)

int main(int argc, char* argv[]) {
	int result = 0;
	int rc;
	
	printf("*** Testing uint128 ***\n");
	rc = unit_tests_uint128(argc, argv);
	result = rc;
	printf("*** Testing node ***\n");
	rc = unit_tests_node(argc, argv);
	if (result == 0)
		result == rc;
	
	if (result == 0)
		printf("\n*** All unit tests passed successfully ***\n");
	
	return result;
}
