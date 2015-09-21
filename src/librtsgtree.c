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
#include <netinet/in.h>	// For struct in6_addr
#include <arpa/inet.h>	// For inet_ntop() and htons()
#include <string.h>	// For memcpy()

#include "utils.h"	// For assert()
#include "uint128.h"

typedef uint16_t rank_t;
typedef uint8_t prefix_t;

typedef uint128_t node_id_t;

/**
 * \brief IP protocol type (IPV4/IPV6)
**/
typedef enum {
	  IPV4 = 0,
	  IPV6,
} ip_protocol_t;

/**
 * \brief Convert a node ID to an IPv6 address represented as a struct in6_addr
 *
 * \param[in] input The node ID to convert
 * \param[out] output A struct in6_addr for which s6_addr will be filled-in based on \p input (note: other fields are unchanged)
**/
inline void node_id_to_ipv6(const node_id_t input, struct in6_addr* output) {
	assert(sizeof(output->s6_addr) == sizeof(input.uint128_a8));

	memcpy((void *)(output->s6_addr), (void *)(input.uint128_a8), sizeof(output->s6_addr));
}

/**
 * \brief Specification of a self creating IP routing tree
**/
typedef struct {
  ip_protocol_t  ip_type; /*!< Type of IP addresses represented by the tree (IPv4, IPv6) */
  rank_t         Rmax; /*!< Maximum depth of the tree */
  prefix_t       treeP; /*!< IP prefix for the range allocated to the tree */
  prefix_t       hostA; /*!< Host address size in bits */
} self_ip_routing_tree_t;

/**
 * \brief Get the left child node ID based on a parent node ID
 *
 * \return The node ID or 0 if there was an error
**/
node_id_t get_left_child_node_id(self_ip_routing_tree_t tree, node_id_t parent_node) {
	node_id_t zero;
	zero_uint128_t(zero);	//FIXME
	return zero;
}

/**
 * \brief
**/
node_id_t get_parent_node_id(self_ip_routing_tree_t tree, node_id_t child_node) {
	node_id_t zero;
	zero_uint128_t(zero);	//FIXME
	return zero;
}

/**
 *
**/
node_id_t get_node_id_from_parent_hostname(self_ip_routing_tree_t tree, char* parent_hostname) {
	node_id_t zero;
	zero_uint128_t(zero);	//FIXME
	return zero;
}

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
