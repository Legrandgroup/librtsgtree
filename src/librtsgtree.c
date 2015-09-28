/*
 * \file librtsgtree.c
 *
 * TODO: Add unit tests on *binstr and *hexstr for dumping part of the uint128_t (not the whole 128 bits or 16 bytes)
 * TODO: Add tests on increment decrement
 * TODO: Split uint128_t to separate file than node_t
 * TODO: Implement node-specific functions
 * TODO: Use cpputest for unit testing
 */

#include <stdio.h>
#include <stdlib.h>

#include "utils.h"	// For assert()
#include "ipv6_node.h"

int main(void) {
	test_zero_uint18_t();
	test_uint128_t_to_binstr();
	test_uint128_t_to_hexstr();
	test_power2_to_uint128_t();
	test_uint128_t_right_shift();
	test_uint128_t_left_shift();
	test_uint128_t_mix_shift();
	test_uint128_t_inc();
	test_uint128_t_dec();
	test_uint16_t_to_uint128_t();
	test_uint8_t_to_uint128_t();
	test_uint128_t_sub();
	test_uint128_t_add();
	//test_uint128_t_mixed_add_sub_inc_dec();
	test_uint128_t_right_0bit_count();

	//test_node_id_to_ipv6();
	test_node_id_to_rank();
	test_get_root_node_id();
	test_get_left_child_node_id();
	test_get_right_child_node_id();

	printf("*** All unit tests passed successfully ***\n");

	return EXIT_SUCCESS;
}
