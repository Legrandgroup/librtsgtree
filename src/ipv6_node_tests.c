#include "ipv6_node.h"
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>	// For inet_ntop() and htons()
#include <stdint.h>
#include <string.h>	// For memcpy()

#include "utils.h"	// For assert()

/* Unit test for node_id_to_ipv6()
 */
void test_node_id_to_ipv6() {
	node_id_t this_node;
	char result[129];

	char repr[INET6_ADDRSTRLEN+1];
	struct in6_addr dst_in_addr;

	node_id_to_ipv6(this_node, &dst_in_addr);
	inet_ntop(AF_INET6, &dst_in_addr, repr, INET6_ADDRSTRLEN);
	//puts(repr);
	//puts(result);
	printf("%s: tests passed\n", __func__);
}

/* Unit test for get_root_node_id()
 */
void test_get_root_node_id() {
	node_id_t test_node;
	char result[17];
	char* expected_result;
	self_ip_routing_tree_t tree;

	tree.Rmax = 4;
	tree.ip_type = IPV6;
	tree.hostA = 0;
	tree.treeP = 124;

	test_node = get_root_node_id(&tree);
	uint128_t_to_hexstr((uint128_t)test_node, 16, result);
	expected_result = "0000000000000000" "0000000000000008";	/* Root node for a tree with Rmax=4 should have ID 8 */
	if (strcmp(result, expected_result) != 0) {
		fprintf(stderr, "%d: get_root_node_id() failed, got:\n\"%s\", expected:\n\"%s\"\n", __LINE__, result, expected_result);
		//FAIL();
		exit(1);
	}

	tree.Rmax = 6;
	tree.ip_type = IPV4;
	tree.hostA = 2;
	tree.treeP = 24;

	test_node = get_root_node_id(&tree);
	uint128_t_to_hexstr((uint128_t)test_node, 16, result);
	expected_result = "0000000000000000" "0000000000000020";	/* Root node for a tree with Rmax=6 should have ID 32 */
	if (strcmp(result, expected_result) != 0) {
		fprintf(stderr, "%d: get_root_node_id() failed, got:\n\"%s\", expected:\n\"%s\"\n", __LINE__, result, expected_result);
		//FAIL();
		exit(1);
	}

	printf("%s: tests passed\n", __func__);
}

/* Unit test for get_left_child_node_id()
 */
void test_get_left_child_node_id() {
	node_id_t test_node;
	char result[17];
	char* expected_result;
	self_ip_routing_tree_t tree;

	tree.Rmax = 4;
	tree.ip_type = IPV6;
	tree.hostA = 0;
	tree.treeP = 124;

	test_node = get_root_node_id(&tree);
	test_node = get_left_child_node_id(&tree, test_node);
	uint128_t_to_hexstr((uint128_t)test_node, 16, result);
	expected_result = "0000000000000000" "0000000000000004";	/* Left child for root node 8 should have ID 4 */
	if (strcmp(result, expected_result) != 0) {
		fprintf(stderr, "%d: get_root_node_id() failed, got:\n\"%s\", expected:\n\"%s\"\n", __LINE__, result, expected_result);
		//FAIL();
		exit(1);
	}

//	tree.Rmax = 6;
//	tree.ip_type = IPV4;
//	tree.hostA = 2;
//	tree.treeP = 24;
//
//	test_node = get_root_node_id(&tree);
//	uint128_t_to_hexstr((uint128_t)test_node, 16, result);
//	expected_result = "0000000000000000" "0000000000000020";	/* Root node for a tree with Rmax=6 should have ID 32 */
//	if (strcmp(result, expected_result) != 0) {
//		fprintf(stderr, "%d: get_root_node_id() failed, got:\n\"%s\", expected:\n\"%s\"\n", __LINE__, result, expected_result);
//		//FAIL();
//		exit(1);
//	}

	printf("%s: tests passed\n", __func__);
}
