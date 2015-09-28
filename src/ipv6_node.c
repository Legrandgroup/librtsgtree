/**
 * file ipv6_node.c
 */

#include "ipv6_node.h"

inline void node_id_to_ipv6(const node_id_t input, struct in6_addr* output) {
	assert(sizeof(output->s6_addr) == sizeof(input.uint128_a8));

	memcpy((void *)(output->s6_addr), (void *)(input.uint128_a8), sizeof(output->s6_addr));
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
	/* Test if node is inside the addressable values (<2^Rmax) */
	//if (!uint128_t_is_lt((uint128_t)node, power2_to_uint128_t(tree->Rmax)))
	//	return 0;
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
	assert(parent_rank<=Rmax_1);
	R = Rmax_1 - parent_rank;
	return uint128_t_sub(parent_node, (node_id_t)power2_to_uint128_t(R));
}

node_id_t get_parent_node_id(self_ip_routing_tree_t tree, node_id_t child_node) {
	node_id_t zero;
	zero_uint128_t(zero);	//FIXME
	return zero;
}

node_id_t get_node_id_from_parent_hostname(self_ip_routing_tree_t tree, char* parent_hostname) {
	node_id_t zero;
	zero_uint128_t(zero);	//FIXME
	return zero;
}
