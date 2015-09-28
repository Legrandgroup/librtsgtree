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

/* Unit test for node_id_to_rank()
 */
void test_node_id_to_rank() {
	node_id_t test_node;
	char result[17];
	char* expected_result;
	self_ip_routing_tree_t tree;

	tree.Rmax = 4;
	tree.ip_type = IPV6;
	tree.hostA = 0;
	tree.treeP = 124;

	if (node_id_to_rank(&tree, (node_id_t)(uint16_t_to_uint128_t(8))) != (rank_t)1) {
		fprintf(stderr, "%d: node_id_to_rank() failed for rank 1\n", __LINE__);
		//FAIL();
		exit(1);
	}
	if (node_id_to_rank(&tree, (node_id_t)(uint16_t_to_uint128_t(4))) != (rank_t)2 ||
	    node_id_to_rank(&tree, (node_id_t)(uint16_t_to_uint128_t(12))) != (rank_t)2) {
		fprintf(stderr, "%d: node_id_to_rank() failed for rank 2\n", __LINE__);
		//FAIL();
		exit(1);
	}
	if (node_id_to_rank(&tree, (node_id_t)(uint16_t_to_uint128_t(2))) != (rank_t)3 ||
	    node_id_to_rank(&tree, (node_id_t)(uint16_t_to_uint128_t(6))) != (rank_t)3 ||
		node_id_to_rank(&tree, (node_id_t)(uint16_t_to_uint128_t(10))) != (rank_t)3 ||
		node_id_to_rank(&tree, (node_id_t)(uint16_t_to_uint128_t(14))) != (rank_t)3) {
		fprintf(stderr, "%d: node_id_to_rank() failed for rank 3\n", __LINE__);
		//FAIL();
		exit(1);
	}
	if (node_id_to_rank(&tree, (node_id_t)(uint16_t_to_uint128_t(1))) != (rank_t)4 ||
	    node_id_to_rank(&tree, (node_id_t)(uint16_t_to_uint128_t(3))) != (rank_t)4 ||
		node_id_to_rank(&tree, (node_id_t)(uint16_t_to_uint128_t(5))) != (rank_t)4 ||
	    node_id_to_rank(&tree, (node_id_t)(uint16_t_to_uint128_t(7))) != (rank_t)4 ||
		node_id_to_rank(&tree, (node_id_t)(uint16_t_to_uint128_t(9))) != (rank_t)4 ||
	    node_id_to_rank(&tree, (node_id_t)(uint16_t_to_uint128_t(11))) != (rank_t)4 ||
		node_id_to_rank(&tree, (node_id_t)(uint16_t_to_uint128_t(13))) != (rank_t)4 ||
		node_id_to_rank(&tree, (node_id_t)(uint16_t_to_uint128_t(15))) != (rank_t)4) {
		fprintf(stderr, "%d: node_id_to_rank() failed for rank 4\n", __LINE__);
		//FAIL();
		exit(1);
	}

	/* Test invalid node IDs */
	if (node_id_to_rank(&tree, (node_id_t)(uint16_t_to_uint128_t(16))) != (rank_t)0) {
		fprintf(stderr, "%d: node_id_to_rank() failed for invalid node ID 16\n", __LINE__);
		//FAIL();
		exit(1);
	}
	if (node_id_to_rank(&tree, (node_id_t)(uint16_t_to_uint128_t(0xffff))) != (rank_t)0) {
		fprintf(stderr, "%d: node_id_to_rank() failed for invalid node ID 0xffff\n", __LINE__);
		//FAIL();
		exit(1);
	}
	if (node_id_to_rank(&tree, (node_id_t)(uint16_t_to_uint128_t(0x8000))) != (rank_t)0) {
		fprintf(stderr, "%d: node_id_to_rank() failed for invalid node ID 0x8000\n", __LINE__);
		//FAIL();
		exit(1);
	}
	if (node_id_to_rank(&tree, (node_id_t)(uint16_t_to_uint128_t(0))) != (rank_t)0) {
		fprintf(stderr, "%d: node_id_to_rank() failed for invalid node ID 0\n", __LINE__);
		//FAIL();
		exit(1);
	}

	tree.Rmax = 6;
	tree.ip_type = IPV4;
	tree.hostA = 2;
	tree.treeP = 24;

	if (node_id_to_rank(&tree, (node_id_t)(uint16_t_to_uint128_t(32))) != (rank_t)1) {
		fprintf(stderr, "%d: node_id_to_rank() failed for rank 1\n", __LINE__);
		//FAIL();
		exit(1);
	}
	if (node_id_to_rank(&tree, (node_id_t)(uint16_t_to_uint128_t(16))) != (rank_t)2 ||
	    node_id_to_rank(&tree, (node_id_t)(uint16_t_to_uint128_t(48))) != (rank_t)2) {
		fprintf(stderr, "%d: node_id_to_rank() failed for rank 2\n", __LINE__);
		//FAIL();
		exit(1);
	}
	if (node_id_to_rank(&tree, (node_id_t)(uint16_t_to_uint128_t(8))) != (rank_t)3 ||
	    node_id_to_rank(&tree, (node_id_t)(uint16_t_to_uint128_t(24))) != (rank_t)3 ||
		node_id_to_rank(&tree, (node_id_t)(uint16_t_to_uint128_t(40))) != (rank_t)3 ||
		node_id_to_rank(&tree, (node_id_t)(uint16_t_to_uint128_t(56))) != (rank_t)3) {
		fprintf(stderr, "%d: node_id_to_rank() failed for rank 3\n", __LINE__);
		//FAIL();
		exit(1);
	}
	if (node_id_to_rank(&tree, (node_id_t)(uint16_t_to_uint128_t(4))) != (rank_t)4 ||
	    node_id_to_rank(&tree, (node_id_t)(uint16_t_to_uint128_t(12))) != (rank_t)4 ||
		node_id_to_rank(&tree, (node_id_t)(uint16_t_to_uint128_t(20))) != (rank_t)4 ||
	    node_id_to_rank(&tree, (node_id_t)(uint16_t_to_uint128_t(28))) != (rank_t)4 ||
		node_id_to_rank(&tree, (node_id_t)(uint16_t_to_uint128_t(36))) != (rank_t)4 ||
	    node_id_to_rank(&tree, (node_id_t)(uint16_t_to_uint128_t(44))) != (rank_t)4 ||
		node_id_to_rank(&tree, (node_id_t)(uint16_t_to_uint128_t(52))) != (rank_t)4 ||
		node_id_to_rank(&tree, (node_id_t)(uint16_t_to_uint128_t(60))) != (rank_t)4) {
		fprintf(stderr, "%d: node_id_to_rank() failed for rank 4\n", __LINE__);
		//FAIL();
		exit(1);
	}
	if (node_id_to_rank(&tree, (node_id_t)(uint16_t_to_uint128_t(2))) != (rank_t)5 ||
	    node_id_to_rank(&tree, (node_id_t)(uint16_t_to_uint128_t(6))) != (rank_t)5 ||
		node_id_to_rank(&tree, (node_id_t)(uint16_t_to_uint128_t(10))) != (rank_t)5 ||
	    node_id_to_rank(&tree, (node_id_t)(uint16_t_to_uint128_t(14))) != (rank_t)5 ||
		node_id_to_rank(&tree, (node_id_t)(uint16_t_to_uint128_t(18))) != (rank_t)5 ||
	    node_id_to_rank(&tree, (node_id_t)(uint16_t_to_uint128_t(22))) != (rank_t)5 ||
		node_id_to_rank(&tree, (node_id_t)(uint16_t_to_uint128_t(26))) != (rank_t)5 ||
	    node_id_to_rank(&tree, (node_id_t)(uint16_t_to_uint128_t(30))) != (rank_t)5 ||
		node_id_to_rank(&tree, (node_id_t)(uint16_t_to_uint128_t(34))) != (rank_t)5 ||
	    node_id_to_rank(&tree, (node_id_t)(uint16_t_to_uint128_t(38))) != (rank_t)5 ||
		node_id_to_rank(&tree, (node_id_t)(uint16_t_to_uint128_t(42))) != (rank_t)5 ||
	    node_id_to_rank(&tree, (node_id_t)(uint16_t_to_uint128_t(46))) != (rank_t)5 ||
		node_id_to_rank(&tree, (node_id_t)(uint16_t_to_uint128_t(50))) != (rank_t)5 ||
	    node_id_to_rank(&tree, (node_id_t)(uint16_t_to_uint128_t(54))) != (rank_t)5 ||
		node_id_to_rank(&tree, (node_id_t)(uint16_t_to_uint128_t(58))) != (rank_t)5 ||
		node_id_to_rank(&tree, (node_id_t)(uint16_t_to_uint128_t(62))) != (rank_t)5) {
		fprintf(stderr, "%d: node_id_to_rank() failed for rank 5\n", __LINE__);
		//FAIL();
		exit(1);
	}
	if (node_id_to_rank(&tree, (node_id_t)(uint16_t_to_uint128_t(1))) != (rank_t)6 ||
	    node_id_to_rank(&tree, (node_id_t)(uint16_t_to_uint128_t(3))) != (rank_t)6 ||
		node_id_to_rank(&tree, (node_id_t)(uint16_t_to_uint128_t(5))) != (rank_t)6 ||
	    node_id_to_rank(&tree, (node_id_t)(uint16_t_to_uint128_t(7))) != (rank_t)6 ||
		node_id_to_rank(&tree, (node_id_t)(uint16_t_to_uint128_t(9))) != (rank_t)6 ||
	    node_id_to_rank(&tree, (node_id_t)(uint16_t_to_uint128_t(11))) != (rank_t)6 ||
		node_id_to_rank(&tree, (node_id_t)(uint16_t_to_uint128_t(13))) != (rank_t)6 ||
	    node_id_to_rank(&tree, (node_id_t)(uint16_t_to_uint128_t(15))) != (rank_t)6 ||
		node_id_to_rank(&tree, (node_id_t)(uint16_t_to_uint128_t(17))) != (rank_t)6 ||
	    node_id_to_rank(&tree, (node_id_t)(uint16_t_to_uint128_t(19))) != (rank_t)6 ||
		node_id_to_rank(&tree, (node_id_t)(uint16_t_to_uint128_t(21))) != (rank_t)6 ||
	    node_id_to_rank(&tree, (node_id_t)(uint16_t_to_uint128_t(23))) != (rank_t)6 ||
		node_id_to_rank(&tree, (node_id_t)(uint16_t_to_uint128_t(25))) != (rank_t)6 ||
	    node_id_to_rank(&tree, (node_id_t)(uint16_t_to_uint128_t(27))) != (rank_t)6 ||
		node_id_to_rank(&tree, (node_id_t)(uint16_t_to_uint128_t(29))) != (rank_t)6 ||
	    node_id_to_rank(&tree, (node_id_t)(uint16_t_to_uint128_t(31))) != (rank_t)6 ||
		node_id_to_rank(&tree, (node_id_t)(uint16_t_to_uint128_t(33))) != (rank_t)6 ||
	    node_id_to_rank(&tree, (node_id_t)(uint16_t_to_uint128_t(35))) != (rank_t)6 ||
		node_id_to_rank(&tree, (node_id_t)(uint16_t_to_uint128_t(37))) != (rank_t)6 ||
	    node_id_to_rank(&tree, (node_id_t)(uint16_t_to_uint128_t(39))) != (rank_t)6 ||
		node_id_to_rank(&tree, (node_id_t)(uint16_t_to_uint128_t(41))) != (rank_t)6 ||
	    node_id_to_rank(&tree, (node_id_t)(uint16_t_to_uint128_t(43))) != (rank_t)6 ||
		node_id_to_rank(&tree, (node_id_t)(uint16_t_to_uint128_t(45))) != (rank_t)6 ||
	    node_id_to_rank(&tree, (node_id_t)(uint16_t_to_uint128_t(47))) != (rank_t)6 ||
		node_id_to_rank(&tree, (node_id_t)(uint16_t_to_uint128_t(49))) != (rank_t)6 ||
	    node_id_to_rank(&tree, (node_id_t)(uint16_t_to_uint128_t(51))) != (rank_t)6 ||
		node_id_to_rank(&tree, (node_id_t)(uint16_t_to_uint128_t(53))) != (rank_t)6 ||
	    node_id_to_rank(&tree, (node_id_t)(uint16_t_to_uint128_t(55))) != (rank_t)6 ||
		node_id_to_rank(&tree, (node_id_t)(uint16_t_to_uint128_t(57))) != (rank_t)6 ||
	    node_id_to_rank(&tree, (node_id_t)(uint16_t_to_uint128_t(59))) != (rank_t)6 ||
		node_id_to_rank(&tree, (node_id_t)(uint16_t_to_uint128_t(61))) != (rank_t)6 ||
		node_id_to_rank(&tree, (node_id_t)(uint16_t_to_uint128_t(63))) != (rank_t)6) {
		fprintf(stderr, "%d: node_id_to_rank() failed for rank 6\n", __LINE__);
		//FAIL();
		exit(1);
	}

	/* Test invalid node IDs */
	if (node_id_to_rank(&tree, (node_id_t)(uint16_t_to_uint128_t(64))) != (rank_t)0) {
		fprintf(stderr, "%d: node_id_to_rank() failed for invalid node ID 64\n", __LINE__);
		//FAIL();
		exit(1);
	}
	if (node_id_to_rank(&tree, (node_id_t)(uint16_t_to_uint128_t(0xffff))) != (rank_t)0) {
		fprintf(stderr, "%d: node_id_to_rank() failed for invalid node ID 0xffff\n", __LINE__);
		//FAIL();
		exit(1);
	}
	if (node_id_to_rank(&tree, (node_id_t)(uint16_t_to_uint128_t(0x8000))) != (rank_t)0) {
		fprintf(stderr, "%d: node_id_to_rank() failed for invalid node ID 0x8000\n", __LINE__);
		//FAIL();
		exit(1);
	}
	if (node_id_to_rank(&tree, (node_id_t)(uint16_t_to_uint128_t(0))) != (rank_t)0) {
		fprintf(stderr, "%d: node_id_to_rank() failed for invalid node ID 0\n", __LINE__);
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
