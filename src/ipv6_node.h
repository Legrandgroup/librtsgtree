/**
 * file ip_node.h
 */

#ifndef __IPV6_NODE_H__
#define __IPV6_NODE_H__

#include "uint128.h"	// For uint128_t

#include <string.h>	// For memcpy()

typedef uint8_t rank_t;
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
 * \brief Specification of a self creating IP routing tree
**/
typedef struct {
	ip_protocol_t  ip_type; /*!< Type of IP addresses represented by the tree (IPv4, IPv6) */
	rank_t         Rmax; /*!< Maximum depth of the tree */
	uint128_t      prefix;	/*!< Prefix for the tree's subnet */
	prefix_t       hostA; /*!< Host address size in bits */
} self_ip_routing_tree_t;

/**
 * \brief Specification of an IP address + a prefix length (netmask)
 */
typedef struct {
	ip_protocol_t  ip_type; /*!< Type of IP addresses represented by the tree (IPv4, IPv6) */
	union {
		struct in_addr  __ipv4_in_addr;	/*!< The in_addr if ip_type == IPv4 */
		struct in6_addr __ipv6_in6_addr; /*!< The in_addr if ip_type == IPv6 */
	} __in_addr;
	prefix_t       prefix; /*!< The prefix length (CIDR for IPv4) or netmask */
} if_ip_addr_t;

/**
 * \brief Convert a uint128_t to an IPv6 address represented as a struct in6_addr
 *
 * \param[in] input The node ID to convert
 * \param[out] output A struct in6_addr for which s6_addr will be filled-in based on \p input (note: other fields are unchanged)
**/
inline void uint128_t_to_ipv6(const uint128_t input, struct in6_addr* output);

/**
 * \brief Get the number of most significant bits that form the network part of a tree (common prefix bitmask between all hosts of the tree)
 *
 * This function works on IPv4 and IPv6 trees
 *
 * \param tree The tree to work on
 *
 * \result The prefix length (between 128 or 32 (tree depth is 0) and 0
 */
prefix_t get_tree_prefix_len(const self_ip_routing_tree_t* tree);

/**
 * \brief Get the number of most significant bits that form the network part of each interconnecting subnet (common prefix bitmask between hosts of each interconnecting subnet)
 *
 * This function works on IPv4 and IPv6 trees
 *
 * \param tree The tree to work on
 *
 * \result The prefix length (between 128 or 32 (tree depth is 0) and 0
 */
prefix_t get_hosts_prefix_len(const self_ip_routing_tree_t* tree);

/**
 * \brief Get the node ID of the root node in a routed self-generated tree
 *
 * \param tree The tree for which we want to get the root node ID
 *
 * \return The node ID of the root
 */
node_id_t get_root_node_id(const self_ip_routing_tree_t* tree);

/**
 * \brief Get the rank associated with a given node ID in a routed self-generated tree
 *
 * \param tree The tree inside which we perform the calculation
 * \param node The node ID for which we want to calculate the rank
 *
 * \return The rank (in the tree) of the requested \p node (or 0 if \p node is invalid for the provided \p tree)
 */
inline rank_t node_id_to_rank(const self_ip_routing_tree_t* tree, node_id_t node);

/**
 * \brief Get the left child node ID based on a parent node ID
 *
 * \param tree The tree inside which we perform the calculation
 * \param parent_node The node ID for which we want to calculate the left child
 *
 * \return The node ID for the left child or 0 if there was an error
**/
node_id_t get_left_child_node_id(const self_ip_routing_tree_t* tree, const node_id_t parent_node);

/**
 * \brief Get the right child node ID based on a parent node ID
 *
 * \param tree The tree inside which we perform the calculation
 * \param parent_node The node ID for which we want to calculate the right child
 *
 * \return The node ID for the right child or 0 if there was an error
**/
node_id_t get_right_child_node_id(const self_ip_routing_tree_t* tree, const node_id_t parent_node);

/**
 * \brief Get the right child node ID based on a parent node ID
 *
 * \param tree The tree inside which we perform the calculation
 * \param node The node ID for which we want to calculate the right child
 *
 * \return The node ID for the right child or 0 if there was an error
**/
if_ip_addr_t get_top_interface_config(const self_ip_routing_tree_t* tree, const node_id_t node);

#endif	// __IPV6_NODE_H__
