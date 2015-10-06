/**
 * file node.c
 */

#include "node.h"
#include <assert.h>	// For assert()

inline node_id_t Rmax_to_max_node_id(const rank_t Rmax) {
	assert(Rmax != 128);

	return uint128_t_dec(power2_to_uint128_t(Rmax));	/* FIXME: Why not use a dedicated right bit-filling function like ipv6_prefix_to_uint128_t_mask() to be more efficient? */
}

inline void uint128_t_to_ipv6(const uint128_t input, struct in6_addr* output) {
	assert(sizeof(output->s6_addr) == sizeof(input.uint128_a8));

	memcpy((void *)(output->s6_addr), (void *)(input.uint128_a8), sizeof(input.uint128_a8));
}

inline void uint32_t_to_ipv4(const uint32_t input, struct in_addr* output) {
	assert(sizeof(output->s_addr) == sizeof(input));

	output->s_addr = input;
}

inline prefix_t get_tree_ip_addr_bit_sz(const self_ip_routing_tree_t* tree) {
	
	assert(tree);
	assert(tree->ip_type);
#ifdef IPV6_SUPPORT
	if (tree->ip_type == IPV6)
		return 128;
	else
#endif
#ifdef IPV4_SUPPORT
		if (tree->ip_type == IPV4)
			return 32;
		else
#endif
			assert(0);	/* Error */
}

prefix_t get_tree_prefix_len(const self_ip_routing_tree_t* tree) {
	prefix_t result;

	assert(tree);
	result = get_hosts_prefix_len(tree);
	assert(result > tree->Rmax);

	return result - tree->Rmax;
}

prefix_t get_hosts_prefix_len(const self_ip_routing_tree_t* tree) {
	prefix_t result;

	result = get_tree_ip_addr_bit_sz(tree);
	assert(result > tree->hostA);
	return result - tree->hostA;
}

#ifdef IPV6_SUPPORT
uint128_t ipv6_prefix_to_uint128_t_mask(prefix_t prefix) {
	uint8_t byte_pos;
	uint128_t result;

	assert(prefix<=128);

	for (byte_pos = 0; prefix >= 8; byte_pos++, prefix-=8) {
		result.uint128_a8[byte_pos] = 0xff;	/* Fill all full bytes */
	}
	if (prefix!=0) {	/* If there are remaining bits */
		assert(prefix<=8);
		assert(byte_pos<sizeof(result.uint128_a8));	/* Make sure we don't overflow... should never occur */
		result.uint128_a8[byte_pos] = 0xff << (8-prefix);
		byte_pos++;
	}
	for (; byte_pos<sizeof(result.uint128_a8); byte_pos++) {
		result.uint128_a8[byte_pos] = 0;	/* Fill the remaining bytes with 0 */
	}
	return result;
}
#endif	// IPV6_SUPPORT

node_id_t get_root_node_id(const self_ip_routing_tree_t* tree) {

	assert(tree);
	assert(tree->Rmax > 0);
	assert(tree->Rmax - 1 <= 128);
	return power2_to_uint128_t(tree->Rmax - 1);
}

rank_t node_id_to_rank(const self_ip_routing_tree_t* tree, node_id_t node) {
	rank_t b;

	assert(tree);
	/* Test if node is inside the addressable values (<2^Rmax)... if not, return 0 as an error */
	if (uint128_t_cmp((uint128_t)node, power2_to_uint128_t(tree->Rmax))>=0)
		return 0;
	/* Calculate the number of LSB bits set to 0 */
	b = uint128_t_right_0bit_count((uint128_t)node);
	if (tree->Rmax < b)	/* Error... invalid node ID for this tree */
		return 0;
	return tree->Rmax - b;
}

node_id_t get_parent_node_id(const self_ip_routing_tree_t* tree, const node_id_t child_node) {
	rank_t Rmax_Rp;
	rank_t child_rank;
	node_id_t result;
	prefix_t mask_prefix;
	
	uint128_t parent_node_mask;

	//Formula: to get our parent ID
	//Take our ID, set the last (Rmax-Rp+1) bits to 0, and set the previous bit to 1
	
	assert(tree);
	assert(tree->Rmax > 0);
	child_rank = node_id_to_rank(tree, child_node);
	
	if (tree->Rmax < child_rank)
		return (node_id_t)uint128_t_zero();	/* Child rank is higher than tree depth... return error */
	if (child_rank <= 1)
		return (node_id_t)uint128_t_zero();	/* Child is the root or is above root... return error */
	Rmax_Rp = tree->Rmax - child_rank;	/* Calculate the distance between our rank and the last ranks (leaves) */
	Rmax_Rp++;	/* Go up to parent, one more LSB bits to zero */
	assert(Rmax_Rp <= 128);
	mask_prefix = 128 - Rmax_Rp;
	result = (node_id_t)uint128_t_and((uint128_t)child_node,
									  ipv6_prefix_to_uint128_t_mask(mask_prefix));
	result = (node_id_t)uint128_t_or((uint128_t)result,
									 power2_to_uint128_t(Rmax_Rp));
	return result;
}

node_id_t get_left_child_node_id(const self_ip_routing_tree_t* tree, const node_id_t parent_node) {
	rank_t Rmax_1, R;
	rank_t parent_rank;

	//Formula: LC(p) = p - 2^(Rmax - 1 - Rp)
	assert(tree);
	assert(tree->Rmax > 0);
	assert(tree->Rmax - 1 <= 128);
	Rmax_1 = tree->Rmax - 1;
	parent_rank = node_id_to_rank(tree, parent_node);
	if (parent_rank == 0)
		return (node_id_t)uint128_t_zero();
	if (parent_rank>Rmax_1)
		return (node_id_t)uint128_t_zero();
	R = Rmax_1 - parent_rank;
	return (node_id_t)uint128_t_sub(parent_node, (node_id_t)power2_to_uint128_t(R));
}

node_id_t get_right_child_node_id(const self_ip_routing_tree_t* tree, const node_id_t parent_node) {
	rank_t Rmax_1, R;
	rank_t parent_rank;

	//Formula: LC(p) = p + 2^(Rmax - 1 - Rp)
	assert(tree);
	assert(tree->Rmax > 0);
	assert(tree->Rmax - 1 <= 128);
	Rmax_1 = tree->Rmax - 1;
	parent_rank = node_id_to_rank(tree, parent_node);
	if (parent_rank == 0)
		return (node_id_t)uint128_t_zero();
	if (parent_rank>Rmax_1)
		return (node_id_t)uint128_t_zero();
	R = Rmax_1 - parent_rank;
	return (node_id_t)uint128_t_add(parent_node, (node_id_t)power2_to_uint128_t(R));
}

#ifdef IPV6_SUPPORT
uint128_t get_top_interface_ipv6_addr(const self_ip_routing_tree_t* tree, const node_id_t node) {
	uint128_t result;

	assert(tree);
	result = uint128_t_left_shift_n((uint128_t)node, tree->hostA);
	return uint128_t_or(result,
	                    uint128_t_and(tree->prefix, ipv6_prefix_to_uint128_t_mask(get_hosts_prefix_len(tree))));
}
#endif	// IPV6_SUPPORT

if_ip_addr_t get_top_interface_config(const self_ip_routing_tree_t* tree, const node_id_t node) {
	if_ip_addr_t result;

	assert(tree);
	assert(tree->ip_type);
	assert_valid_node_id_for_tree(node, *tree);	/* Make sure node contains a valid value for this tree */

	result.ip_type = tree->ip_type;
#ifdef IPV6_SUPPORT
	if (tree->ip_type == IPV6) {
		uint128_t_to_ipv6(get_top_interface_ipv6_addr(tree, (uint128_t)node), &(result.in_addr.__ipv6_in6_addr));
		result.prefix = get_hosts_prefix_len(tree);
	}
	else
#endif
#ifdef IPV4_SUPPORT
		if (tree->ip_type == IPV4) {
			assert(0);	/* Not implemented yet */
			//TODO: implement function get_top_interface_ipv4_addr() to support IPv4 trees
			//uint32_t_to_ipv4(get_top_interface_ipv4_addr(tree, (uint128_t)node), &(result.__in_addr.__ipv4_in_addr));
			result.prefix = get_hosts_prefix_len(tree);
		}
		else
#endif
			assert(0);	/* Force fail */

	return result;
}

if_ip_addr_t get_left_interface_config(const self_ip_routing_tree_t* tree, const node_id_t node) {
	if_ip_addr_t result;

	assert(tree);
	assert(tree->ip_type);
	assert_valid_node_id_for_tree(node, *tree);	/* Make sure node contains a valid value for this tree */

	result.ip_type = tree->ip_type;
#ifdef IPV6_SUPPORT
	if (tree->ip_type == IPV6) {
		uint128_t_to_ipv6(uint128_t_zero(), &(result.in_addr.__ipv6_in6_addr));
		result.prefix = 128;
		result.ip_type = NONE;	/* Left interface has no IP address in IPv6 trees (only link-local fe80::/64 addresses are used on interfaces with children) */
	}
	else
#endif
#ifdef IPV4_SUPPORT
		if (tree->ip_type == IPV4) {
			assert(0);	/* Not implemented yet */
			//TODO: implement function get_left_interface_ipv4_addr() to support IPv4 trees
			//uint32_t_to_ipv4(get_left_interface_ipv4_addr(tree, (uint128_t)node), &(result.__in_addr.__ipv4_in_addr));
			result.prefix = get_hosts_prefix_len(tree);
		}
		else
#endif
			assert(0);	/* Force fail */

	return result;
}

if_ip_addr_t get_right_interface_config(const self_ip_routing_tree_t* tree, const node_id_t node) {
	if_ip_addr_t result;

	assert(tree);
	assert(tree->ip_type);
	assert_valid_node_id_for_tree(node, *tree);	/* Make sure node contains a valid value for this tree */

	result.ip_type = tree->ip_type;
#ifdef IPV6_SUPPORT
	if (tree->ip_type == IPV6) {
		uint128_t_to_ipv6(uint128_t_zero(), &(result.in_addr.__ipv6_in6_addr));
		result.prefix = 128;
		result.ip_type = NONE;	/* Right interface has no IP address in IPv6 trees (only link-local fe80::/64 addresses are used on interfaces with children) */
	}
	else
#endif
#ifdef IPV4_SUPPORT
		if (tree->ip_type == IPV4) {
			assert(0);	/* Not implemented yet */
			//TODO: implement function get_right_interface_ipv4_addr() to support IPv4 trees
			//uint32_t_to_ipv4(get_right_interface_ipv4_addr(tree, (uint128_t)node), &(result.__in_addr.__ipv4_in_addr));
			result.prefix = get_hosts_prefix_len(tree);
		}
		else
#endif
			assert(0);	/* Force fail */

	return result;
}

ip_route_t get_top_interface_route(const self_ip_routing_tree_t* tree, const node_id_t node) {
	ip_route_t result;
	rank_t     node_rank;

	assert(tree);
	assert(tree->ip_type);
	assert_valid_node_id_for_tree(node, *tree);	/* Make sure node contains a valid value for this tree */

	node_rank = node_id_to_rank(tree, node);

	result.ip_type = tree->ip_type;
#ifdef IPV6_SUPPORT
#warning IPv6 support is not implemented
	if (tree->ip_type == IPV6) {
		assert(0);	/* Not implemented yet */
		//TODO: support IPv6 trees for routes
	}
	else
#endif
#ifdef IPV4_SUPPORT
#warning IPv4 support is not implemented
		if (tree->ip_type == IPV4) {
			assert(0);	/* Not implemented yet */
			//TODO: support IPv4 trees for routes
		}
		else
#endif
			assert(0);	/* Force fail */

	return result;
}

/**
 * \def get_child_func_t
 *
 * Pointer to either get_left_child_node_id() or get_right_child_node_id()
 */
typedef ip_route_t (*get_child_func_t)(const self_ip_routing_tree_t* tree, const node_id_t node);

/**
 * \brief Private function returning a ip_route_t value meaning "no valid route"
 *
 * \return "no valid route" value, with ip_type = NONE
 */
inline ip_route_t no_interface_route() {
	ip_route_t result;

	memset(&result, 0, sizeof(result));	/* Fill IP address with 0 */
	result.ip_type = NONE;
	return result;
}

/**
 * \brief Private function that allows to implement get_left_interface_route() and get_right_interface_route() with a unique source code
 *
 * get_left_interface_route() and get_right_interface_route() are mostly identical, except for the child node ID used in the calculation.
 * get_left_interface_route() and get_right_interface_route() are thus implemented as two wrappers around this function
 * We take the same parameters as these functions, plus an extra \p child_node
 *
 * \param tree The tree inside which we perform the calculation
 * \param node The node ID of which we want to calculate the route
 * \param node_rank The rank for \p node in \p tree
 * \param child_node The node ID of the child (left or right) for which we cant to calculate the route
 */
ip_route_t get_bottom_interface_route(const self_ip_routing_tree_t* tree, const node_id_t node, const rank_t node_rank, const node_id_t child_node) {
	ip_route_t result;
	//uint128_t result_u128; // Temporary for debug
	//char result_txt[33];	// Temporary for debug

	assert(tree);
	assert(tree->ip_type);
	assert_valid_node_id_for_tree(node, *tree);	/* Make sure node contains a valid value for this tree */

	result.ip_type = tree->ip_type;
#ifdef IPV6_SUPPORT
	if (tree->ip_type == IPV6) {
		assert(node_rank != tree->Rmax);	/* node_rank == tree->Rmax case must be handled by caller */
		if (node_rank+1 == tree->Rmax) {	/* We are on the penultimate rank */
			uint128_t_to_ipv6(
			                  get_top_interface_ipv6_addr(tree, child_node),	/* get_child_func_t being either get_left_child_node_id() or get_right_child_node_id() */
			                  &(result.in_addr.__ipv6_in6_addr)
			                 );
			result.prefix = 128;
			return result;
		}
		else {	/* We are on any other rank */
			assert(node_rank < tree->Rmax);

			//printf("Will compute AND mask between:\n");
			//uint128_t_to_hexstr(get_top_interface_ipv6_addr(tree, child_node), 16, result_txt);
			//printf("%s (child IPv6)\nand\n", result_txt);
			//uint128_t_to_hexstr(ipv6_prefix_to_uint128_t_mask(get_tree_prefix_len(tree) + node_rank), 16, result_txt);
			//printf("%s (mask for %d+%d prefix)\n=\n", result_txt, get_tree_prefix_len(tree), node_rank);
			//uint128_t_to_hexstr(uint128_t_and(
			//                                  get_top_interface_ipv6_addr(tree, child_node),	/* get_child_func_t being either get_left_child_node_id() or get_right_child_node_id() */
			//                                  ipv6_prefix_to_uint128_t_mask(get_tree_prefix_len(tree) + node_rank)
			//                                 ), 16, result_txt);
			//printf("%s (resulting route with prefix %d)\n", result_txt, get_tree_prefix_len(tree) + node_rank);

			uint128_t_to_ipv6(
			                  uint128_t_and(
			                                get_top_interface_ipv6_addr(tree, child_node),	/* get_child_func_t being either get_left_child_node_id() or get_right_child_node_id() */
			                                ipv6_prefix_to_uint128_t_mask(get_tree_prefix_len(tree) + node_rank)
			                               ),
			                  &(result.in_addr.__ipv6_in6_addr)
			                 );
			result.prefix = get_tree_prefix_len(tree) + node_rank;
			return result;
		}
	}
	else
#endif
#ifdef IPV4_SUPPORT
#warning IPv4 support is not implemented
	if (tree->ip_type == IPV4) {
		assert(0);	/* Not implemented yet */
		//TODO: support IPv4 trees for routes
	}
#endif

	assert(0);	/* We should not reach here */
}

ip_route_t get_left_interface_route(const self_ip_routing_tree_t* tree, const node_id_t node) {

	rank_t node_rank;

	assert(tree);
	assert(tree->ip_type);
	assert_valid_node_id_for_tree(node, *tree);	/* Make sure node contains a valid value for this tree */

	node_rank = node_id_to_rank(tree, node);

#ifdef IPV6_SUPPORT
	if (tree->ip_type == IPV6) {
		if (node_rank == tree->Rmax) {	/* In IPv6 leaves have no route to left of right */
			return no_interface_route();
		}
		else {
			return get_bottom_interface_route(tree, node, node_rank, get_left_child_node_id(tree, node));
		}
	}
#endif
#ifdef IPV4_SUPPORT
#warning IPv4 support is not implemented
	if (tree->ip_type == IPV4) {
		assert(0);	/* Not implemented yet */
		return no_interface_route();
		//TODO: support IPv4 trees for routes
	}
#endif
}

ip_route_t get_right_interface_route(const self_ip_routing_tree_t* tree, const node_id_t node) {

	rank_t node_rank;

	assert(tree);
	assert(tree->ip_type);
	assert_valid_node_id_for_tree(node, *tree);	/* Make sure node contains a valid value for this tree */

	node_rank = node_id_to_rank(tree, node);

#ifdef IPV6_SUPPORT
	if (tree->ip_type == IPV6) {
		if (node_rank == tree->Rmax) {	/* In IPv6 leaves have no route to left of right */
			return no_interface_route();
		}
		else {
			return get_bottom_interface_route(tree, node, node_rank, get_right_child_node_id(tree, node));
		}
	}
#endif
#ifdef IPV4_SUPPORT
#warning IPv4 support is not implemented
	if (tree->ip_type == IPV4) {
		assert(0);	/* Not implemented yet */
		return no_interface_route();
		//TODO: support IPv4 trees for routes
	}
#endif
}
