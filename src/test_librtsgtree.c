/*
 * \file test_librtsgtree.c
 *
 * TODO: Use cpputest for unit testing
 */

#include <stdio.h>
#include <stdlib.h>

#include <assert.h>	// For assert()
#include "node.h"

void unit_tests_uint128();	// Declaration of uint128 unit test procedure (see uint128_arithm_tests.c)
void unit_tests_node();	// Declaration of node unit test procedure (see node_tests.c)

int main(void) {
	printf("*** Testing uint128 ***\n");
	unit_tests_uint128();
	printf("*** Testing node ***\n");
	unit_tests_node();
	printf("\n*** All unit tests passed successfully ***\n");
	return EXIT_SUCCESS;
}
