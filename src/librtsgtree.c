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
#include <stdint.h>
#include <arpa/inet.h>	// For inet_ntop() and htons()

#include "utils.h"	// For assert()
#include "ipv6_node.h"

int main(void) {
	node_id_t this_node;
	char result[129];

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
	test_uint128_t_mixed_add_sub_inc_dec();

	char repr[INET6_ADDRSTRLEN+1];
	struct in6_addr dst_in_addr;
	//uint16_t_to_binstr(this_node.w0, 16, result);
	uint128_t_to_binstr(this_node, 128, result);
	node_id_to_ipv6(this_node, &dst_in_addr);
	inet_ntop(AF_INET6, &dst_in_addr, repr, INET6_ADDRSTRLEN);
	puts(repr);
	puts(result);
	printf("Tests passed\n");
	return EXIT_SUCCESS;
}
