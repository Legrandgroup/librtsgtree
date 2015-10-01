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
	  NONE = 0,
	  IPV4,
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
	ip_protocol_t  ip_type; /*!< Type of IP addresses described inside this entry (IPv4, IPv6) */
	union {
		struct in_addr  __ipv4_in_addr;	/*!< The in_addr if ip_type == IPv4 */
		struct in6_addr __ipv6_in6_addr; /*!< The in_addr if ip_type == IPv6 */
	} in_addr;	/*!< The IPv4 or IPv6 address stored inside this entry */
	prefix_t       prefix; /*!< The prefix length (CIDR for IPv4) or netmask */
} if_ip_addr_t;

/**
 * \brief Specification of an IP route (IP range + next hop)
 */
typedef struct {
	ip_protocol_t  ip_type; /*!< Type of IP addresses described inside this entry (IPv4, IPv6) */
	union {
		struct in_addr  __ipv4_in_addr;	/*!< The in_addr if ip_type == IPv4 */
		struct in6_addr __ipv6_in6_addr; /*!< The in_addr if ip_type == IPv6 */
	} in_addr_range;	/*!< The IP address of the network range concerned by this route */
	prefix_t       range_prefix; /*!< The prefix length (CIDR for IPv4) or netmask of the range concerned by this route */
	union {
		struct in_addr  __ipv4_in_addr;	/*!< The in_addr if ip_type == IPv4 */
		struct in6_addr __ipv6_in6_addr; /*!< The in_addr if ip_type == IPv6 */
	} in_addr_next_hop;	/*!< The next hop (router) for this route */
} ip_route_t;

/**
 * \brief Compute the maximum node ID given a Rmax
 *
 * \param Rmax The value for Rmax
 *
 * \result The maximum node ID (B = 2^(Rmax - 1))
 */
inline uint128_t Rmax_to_max_node_id(const rank_t Rmax);

/**
 * \brief Convert a uint128_t to an IPv6 address represented as a struct in6_addr
 *
 * \param[in] input The 128-bit value representing the IPv6 address to convert
 * \param[out] output A struct in6_addr for which s6_addr will be filled-in based on \p input (note: other fields are unchanged)
**/
inline void uint128_t_to_ipv6(const uint128_t input, struct in6_addr* output);

/**
 * \brief Convert a uint32_t to an IPv4 address represented as a struct in_addr
 *
 * \param[in] input The 32-bit value representing the IPv4 address to convert
 * \param[out] output A struct in_addr for which s_addr will be filled-in based on \p input (note: other fields are unchanged)
**/
inline void uint32_t_to_ipv4(const uint32_t input, struct in_addr* output);

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
 * \brief Create a 128-bit IPv6 netmask from a prefix length
 *
 * \prarm prefix The prefix (value should be >=0 and <=128)
 *
 * \return The 128-bit netmask as a uint128_t
 */
uint128_t ipv6_prefix_to_uint128_t_mask(prefix_t prefix);

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
 * \brief Get the IP configuration for the top network interface of a node
 *
 * Note: tree.ip_type, tree.Rmax, tree.hostA and tree.prefix must be provisioned correctly in argument \p tree to get a correct result
 *
 * \param tree The tree inside which we perform the calculation
 * \param node The node ID of which we want to calculate the network interface characteristics
 *
 * \return The IP addressing of the top network interface for \p node
**/
if_ip_addr_t get_top_interface_config(const self_ip_routing_tree_t* tree, const node_id_t node);

/**
 * \brief Get the IP configuration for the bottom left network interface of a node
 *
 * Note: tree.ip_type, tree.Rmax, tree.hostA and tree.prefix must be provisioned correctly in argument \p tree to get a correct result
 * Warning: You have to check that the returned ip_type is not NONE.
 * If it is NONE, it means there is no expected IP configuration for this interface
 *
 * \param tree The tree inside which we perform the calculation
 * \param node The node ID of which we want to calculate the network interface characteristics
 *
 * \return The IP addressing of the left network interface for \p node
**/
if_ip_addr_t get_left_interface_config(const self_ip_routing_tree_t* tree, const node_id_t node);

/**
 * \brief Get the IP configuration for the bottom right network interface of a node
 *
 * Note: tree.ip_type, tree.Rmax, tree.hostA and tree.prefix must be provisioned correctly in argument \p tree to get a correct result
 * Warning: You have to check that the returned ip_type is not NONE.
 * If it is NONE, it means there is no expected IP configuration for this interface
 *
 * \param tree The tree inside which we perform the calculation
 * \param node The node ID of which we want to calculate the network interface characteristics
 *
 * \return The IP addressing of the right network interface for \p node
**/
if_ip_addr_t get_right_interface_config(const self_ip_routing_tree_t* tree, const node_id_t node);

/**
 * \brief Get the route going via the top network interface of a node
 *
 * Note: tree.ip_type, tree.Rmax, tree.hostA and tree.prefix must be provisioned correctly in argument \p tree to get a correct result
 * Warning: You have to check that the returned ip_type is not NONE.
 * If it is NONE, it means there is no expected route for this interface
 *
 * \param tree The tree inside which we perform the calculation
 * \param node The node ID of which we want to calculate the route
 *
 * \return The route for the top network interface of \p node
**/
ip_route_t get_top_interface_route(const self_ip_routing_tree_t* tree, const node_id_t node);

/**
 * \brief Get the route going via the bottom left network interface of a node
 *
 * Note: tree.ip_type, tree.Rmax, tree.hostA and tree.prefix must be provisioned correctly in argument \p tree to get a correct result
 * Warning: You have to check that the returned ip_type is not NONE.
 * If it is NONE, it means there is no expected route for this interface
 *
 * \param tree The tree inside which we perform the calculation
 * \param node The node ID of which we want to calculate the route
 *
 * \return The route for the left network interface of \p node
**/
ip_route_t get_left_interface_route(const self_ip_routing_tree_t* tree, const node_id_t node);

/**
 * \brief Get the route going via the bottom left network interface of a node
 *
 * Note: tree.ip_type, tree.Rmax, tree.hostA and tree.prefix must be provisioned correctly in argument \p tree to get a correct result
 * Warning: You have to check that the returned ip_type is not NONE.
 * If it is NONE, it means there is no expected route for this interface
 *
 * \param tree The tree inside which we perform the calculation
 * \param node The node ID of which we want to calculate the route
 *
 * \return The route for the left network interface of \p node
**/
ip_route_t get_right_interface_route(const self_ip_routing_tree_t* tree, const node_id_t node);

/**
 * \def assert_valid_node_id_for_tree(n, t)
 *
 * \brief Makes sure the node id \p n is valid for the tree \t (does not exceed the maximum node ID, and is not 0)
 *
 * \param n A node_id_t variable to test
 * \param t A self_ip_routing_tree_t variable containing the tree description
 */
#define assert_valid_node_id_for_tree(n, t) assert(!(uint128_t_cmp(n, Rmax_to_max_node_id((t).Rmax)) > 0 || uint128_t_cmp(n, uint128_t_zero()) == 0))

#endif	// __IPV6_NODE_H__
