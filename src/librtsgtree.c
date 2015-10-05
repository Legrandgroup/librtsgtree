/*
 * \file librtsgtree.c
 *
 * TODO: Use cpputest for unit testing
 */

#include <stdio.h>
#include <stdlib.h>

#include <assert.h>	// For assert()
#include "node.h"

int main(void) {
	test_zero_max_uint18_t();
	test_uint128_t_to_binstr();
	test_uint128_t_to_hexstr();
	test_power2_to_uint128_t();
	test_uint128_t_right_shift();
	test_uint128_t_left_shift();
	test_uint128_t_mix_shift();
	test_uint128_t_right_shift_n();
	test_uint128_t_left_shift_n();
	test_uint128_t_inc();
	test_uint128_t_dec();
	test_uint16_t_to_uint128_t();
	test_uint8_t_to_uint128_t();
	test_uint128_t_sub();
	test_uint128_t_add();
	test_uint128_t_mixed_add_sub_inc_dec();
	test_uint128_t_or();
	test_uint128_t_and();
	test_uint128_t_right_0bit_count();
	test_uint128_t_cmp();

#ifdef IPV6_SUPPORT
	test_uint128_t_to_ipv6();
#endif
	test_get_tree_prefix_len();
	test_get_hosts_prefix_len();
	test_Rmax_to_max_node_id();
#ifdef IPV6_SUPPORT
	test_ipv6_prefix_to_uint128_t_mask();
#endif
	test_node_id_to_rank();
	test_get_root_node_id();
	test_get_parent_node_id();
	test_get_left_child_node_id();
	test_get_right_child_node_id();
	test_get_top_interface_config();
	test_get_left_interface_config();
	test_get_right_interface_config();
	//test_get_top_interface_route();
	test_get_left_right_interface_route();

	printf("\n*** All unit tests passed successfully ***\n");

	return EXIT_SUCCESS;
}
