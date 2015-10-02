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

prefix_t get_tree_prefix_len(const self_ip_routing_tree_t* tree) {
	prefix_t result;

	assert(tree);
	result = get_hosts_prefix_len(tree);
	assert(result > tree->Rmax);

	return result - tree->Rmax;
}

prefix_t get_hosts_prefix_len(const self_ip_routing_tree_t* tree) {
	prefix_t result;

	assert(tree);
	assert(tree->ip_type);
#ifdef IPV6_SUPPORT
	if (tree->ip_type == IPV6)
		result = 128;
	else
#endif
#ifdef IPV4_SUPPORT
		if (tree->ip_type == IPV4)
			result = 32;
		else
#endif
			assert(0);	/* Error */

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
	if (tree->ip_type == IPV6) {
		assert(0);	/* Not implemented yet */
		//TODO: support IPv6 trees for routes
	}
	else
#endif
#ifdef IPV4_SUPPORT
		if (tree->ip_type == IPV4) {
			assert(0);	/* Not implemented yet */
			//TODO: support IPv4 trees for routes
		}
		else
#endif
			assert(0);	/* Force fail */

	return result;
}

ip_route_t get_left_interface_route(const self_ip_routing_tree_t* tree, const node_id_t node) {
	ip_route_t result;
	rank_t     node_rank;

	assert(tree);
	assert(tree->ip_type);
	assert_valid_node_id_for_tree(node, *tree);	/* Make sure node contains a valid value for this tree */

	node_rank = node_id_to_rank(tree, node);

	result.ip_type = tree->ip_type;
#ifdef IPV6_SUPPORT
	if (tree->ip_type == IPV6) {
		if (node_rank == tree->Rmax) {	/* In IPv6 leaves have no route to left of right */
			result.ip_type = NONE;
			uint128_t_to_ipv6(uint128_t_zero(), &(result.in_addr_range.__ipv6_in6_addr));
			uint128_t_to_ipv6(uint128_t_zero(), &(result.in_addr_next_hop.__ipv6_in6_addr));
			result.range_prefix = 128;
		}
		else if (node_rank+1 == tree->Rmax) {	/* We are on the penultimate rank */
			uint128_t_to_ipv6(get_top_interface_ipv6_addr(tree, get_left_child_node_id(tree, node)), &(result.in_addr_range.__ipv6_in6_addr));
			//TODO: which IPv6 address should we use? The link-local one together with interface scope? Store it in in_addr_next_hop or let the caller do it...
			result.range_prefix = 128;
		}
		else {	/* We are on any other rank */
			uint128_t_to_ipv6(get_top_interface_ipv6_addr(tree, get_left_child_node_id(tree, node)), &(result.in_addr_range.__ipv6_in6_addr));
			assert(0);	/* TODO: this needs to be continued */
		}
	}
	else
#endif
#ifdef IPV4_SUPPORT
		if (tree->ip_type == IPV4) {
			assert(0);	/* Not implemented yet */
			//TODO: support IPv4 trees for routes
		}
		else
#endif
			assert(0);	/* Force fail */

	return result;
}

ip_route_t get_right_interface_route(const self_ip_routing_tree_t* tree, const node_id_t node) {
	assert(0);
}
