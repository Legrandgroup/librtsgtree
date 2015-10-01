/**
 * file ip_node.c
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
	if (tree->ip_type == IPV6)
		result = 128;
	else if (tree->ip_type == IPV4)
		result = 32;
	else
		assert(0);	/* Error */

	assert(result > tree->hostA);

	return result - tree->hostA;
}

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
		return (node_id_t)uint128t_zero();
	if (parent_rank>Rmax_1)
		return (node_id_t)uint128t_zero();
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
		return (node_id_t)uint128t_zero();
	if (parent_rank>Rmax_1)
		return (node_id_t)uint128t_zero();
	R = Rmax_1 - parent_rank;
	return (node_id_t)uint128_t_add(parent_node, (node_id_t)power2_to_uint128_t(R));
}

uint128_t get_top_interface_ipv6_addr(const self_ip_routing_tree_t* tree, const node_id_t node) {
	uint128_t result;

	assert(tree);
	result = uint128_t_left_shift_n((uint128_t)node, tree->hostA);
	return uint128_t_or(result,
	                    uint128_t_and(tree->prefix, ipv6_prefix_to_uint128_t_mask(get_hosts_prefix_len(tree))));
}

if_ip_addr_t get_top_interface_config(const self_ip_routing_tree_t* tree, const node_id_t node) {
	if_ip_addr_t result;

	assert(tree);
	assert_valid_node_id_for_tree(node, *tree);	/* Make sure node contains a valid value for this tree */

	result.ip_type = tree->ip_type;
	if (tree->ip_type == IPV6) {
		uint128_t_to_ipv6(get_top_interface_ipv6_addr(tree, (uint128_t)node), &(result.__in_addr.__ipv6_in6_addr));
		result.prefix = get_hosts_prefix_len(tree);
	}
	else if (tree->ip_type == IPV4) {
		assert(0);	/* Not implemented yet */
		//uint32_t_to_ipv4(get_top_interface_ipv4_addr(tree, (uint128_t)node), &(result.__in_addr.__ipv4_in_addr));
		result.prefix = get_hosts_prefix_len(tree);
	}
	else
		assert(0);	/* Force fail */
	return result;
}
