/**
 * file ipv6_node.h
 */

#ifndef __IPV6_NODE_H__
#define __IPV6_NODE_H__

#include "uint128.h"	// For uint128_t

#include "utils.h"	// For assert()
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
  prefix_t       treeP; /*!< IP prefix for the range allocated to the tree */
  prefix_t       hostA; /*!< Host address size in bits */
} self_ip_routing_tree_t;

/**
 * \brief Convert a node ID to an IPv6 address represented as a struct in6_addr
 *
 * \param[in] input The node ID to convert
 * \param[out] output A struct in6_addr for which s6_addr will be filled-in based on \p input (note: other fields are unchanged)
**/
inline void node_id_to_ipv6(const node_id_t input, struct in6_addr* output);

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
rank_t node_id_to_rank(const self_ip_routing_tree_t* tree, node_id_t node);

/**
 * \brief Get the left child node ID based on a parent node ID
 *
 * \param tree The tree inside which we perform the calculation
 * \param node The node ID for which we want to calculate the left child
 *
 * \return The node ID for the left child or 0 if there was an error
**/
node_id_t get_left_child_node_id(const self_ip_routing_tree_t* tree, const node_id_t parent_node);

/**
 * \brief Get the left child node ID based on a parent node ID
 *
 * \param tree The tree inside which we perform the calculation
 * \param node The node ID for which we want to calculate the left child
 *
 * \return The node ID for the left child or 0 if there was an error
**/
node_id_t get_right_child_node_id(const self_ip_routing_tree_t* tree, const node_id_t parent_node);

/**
 *
**/
node_id_t get_parent_node_id(self_ip_routing_tree_t tree, node_id_t child_node);

/**
 *
**/
node_id_t get_node_id_from_parent_hostname(self_ip_routing_tree_t tree, char* parent_hostname);


#endif	// __IPV6_NODE_H__
