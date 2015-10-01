#include "node.h"
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>	// For inet_ntop() and htons()
#include <stdint.h>
#include <string.h>	// For memcpy()


/* Unit test for Rmax_to_max_node_id()
 */
void test_Rmax_to_max_node_id() {
	node_id_t test_node;

	if (uint128_t_cmp(Rmax_to_max_node_id(0), uint128_t_zero()) != 0) {
		fprintf(stderr, "%d: Rmax_to_max_node_id() failed\n", __LINE__);
		//FAIL();
		exit(1);
	}
	if (uint128_t_cmp(Rmax_to_max_node_id(1), uint8_t_to_uint128_t(1)) != 0) {
		fprintf(stderr, "%d: Rmax_to_max_node_id() failed\n", __LINE__);
		//FAIL();
		exit(1);
	}
	if (uint128_t_cmp(Rmax_to_max_node_id(2), uint8_t_to_uint128_t(3)) != 0) {
		fprintf(stderr, "%d: Rmax_to_max_node_id() failed\n", __LINE__);
		//FAIL();
		exit(1);
	}
	if (uint128_t_cmp(Rmax_to_max_node_id(8), uint8_t_to_uint128_t(0xff)) != 0) {
		fprintf(stderr, "%d: Rmax_to_max_node_id() failed\n", __LINE__);
		//FAIL();
		exit(1);
	}
	if (uint128_t_cmp(Rmax_to_max_node_id(16), uint16_t_to_uint128_t(0xffff)) != 0) {
		fprintf(stderr, "%d: Rmax_to_max_node_id() failed\n", __LINE__);
		//FAIL();
		exit(1);
	}

	test_node.uint128_a8[0] = 0x7f;
	test_node.uint128_a8[1] = 0xff;
	test_node.uint128_a8[2] = 0xff;
	test_node.uint128_a8[3] = 0xff;
	test_node.uint128_a8[4] = 0xff;
	test_node.uint128_a8[5] = 0xff;
	test_node.uint128_a8[6] = 0xff;
	test_node.uint128_a8[7] = 0xff;
	test_node.uint128_a8[8] = 0xff;
	test_node.uint128_a8[9] = 0xff;
	test_node.uint128_a8[10] = 0xff;
	test_node.uint128_a8[11] = 0xff;
	test_node.uint128_a8[12] = 0xff;
	test_node.uint128_a8[13] = 0xff;
	test_node.uint128_a8[14] = 0xff;
	test_node.uint128_a8[15] = 0xff;
	if (uint128_t_cmp(Rmax_to_max_node_id(127), test_node) != 0) {
		fprintf(stderr, "%d: Rmax_to_max_node_id() failed\n", __LINE__);
		//FAIL();
		exit(1);
	}

	//Lionel: FIXME: commented-out as we get an assertion error for 128 bits (non-supported value)
//	test_node.uint128_a8[0] = 0xff;	/* uint128_t(-1) */
//	if (uint128_t_cmp(Rmax_to_max_node_id(128), test_node) != 0) {
//		fprintf(stderr, "%d: Rmax_to_max_node_id() failed\n", __LINE__);
//		//FAIL();
//		exit(1);
//	}
	printf("%s: tests passed\n", __func__);
}

/* Unit test for uint128_t_to_ipv6()
 */
void test_uint128_t_to_ipv6() {
	node_id_t test_node;
	char* expected_result;

	char result[INET6_ADDRSTRLEN+1];
	struct in6_addr dst_in_addr;

	test_node.uint128_a8[0] = 0xa5;
	test_node.uint128_a8[1] = 0xa2;
	test_node.uint128_a8[2] = 0x15;
	test_node.uint128_a8[3] = 0x02;
	test_node.uint128_a8[4] = 0x45;
	test_node.uint128_a8[5] = 0xa8;
	test_node.uint128_a8[6] = 0x87;
	test_node.uint128_a8[7] = 0xc4;
	test_node.uint128_a8[8] = 0xe5;
	test_node.uint128_a8[9] = 0x04;
	test_node.uint128_a8[10] = 0x1a;
	test_node.uint128_a8[11] = 0xfe;
	test_node.uint128_a8[12] = 0x89;
	test_node.uint128_a8[13] = 0x9c;
	test_node.uint128_a8[14] = 0x47;
	test_node.uint128_a8[15] = 0x0f;

	uint128_t_to_ipv6(test_node, &dst_in_addr);
	inet_ntop(AF_INET6, &dst_in_addr, result, INET6_ADDRSTRLEN);
	expected_result="a5a2:1502:45a8:87c4:e504:1afe:899c:470f";
	if (strcmp(result, expected_result) != 0) {
		fprintf(stderr, "%d: uint128_t_to_ipv6() failed, got:\n\"%s\", expected:\n\"%s\"\n", __LINE__, result, expected_result);
		//FAIL();
		exit(1);
	}

	test_node.uint128_a8[0] = 0x20;
	test_node.uint128_a8[1] = 0x01;
	test_node.uint128_a8[2] = 0x41;
	test_node.uint128_a8[3] = 0xc8;
	test_node.uint128_a8[4] = 0x10;
	test_node.uint128_a8[5] = 0x00;
	test_node.uint128_a8[6] = 0x00;
	test_node.uint128_a8[7] = 0x21;
	test_node.uint128_a8[8] = 0x00;
	test_node.uint128_a8[9] = 0x00;
	test_node.uint128_a8[10] = 0x00;
	test_node.uint128_a8[11] = 0x00;
	test_node.uint128_a8[12] = 0x00;
	test_node.uint128_a8[13] = 0x21;
	test_node.uint128_a8[14] = 0x00;
	test_node.uint128_a8[15] = 0x04;

	uint128_t_to_ipv6(test_node, &dst_in_addr);
	inet_ntop(AF_INET6, &dst_in_addr, result, INET6_ADDRSTRLEN);
	expected_result="2001:41c8:1000:21::21:4";
	if (strcmp(result, expected_result) != 0) {
		fprintf(stderr, "%d: uint128_t_to_ipv6() failed, got:\n\"%s\", expected:\n\"%s\"\n", __LINE__, result, expected_result);
		//FAIL();
		exit(1);
	}
	printf("%s: tests passed\n", __func__);
}

/* Unit test for get_tree_prefix_len()
 */
void test_get_tree_prefix_len() {
	self_ip_routing_tree_t tree;

	tree.Rmax = 4;
	tree.hostA = 0;
	tree.ip_type = IPV6;
	if (get_tree_prefix_len(&tree) != 124) {
		fprintf(stderr, "%d: get_tree_prefix_len() failed\n", __LINE__);
		//FAIL();
		exit(1);
	}
	tree.Rmax = 120;	/* 120 ranks IPv6 tree (maximum size for private addressing) */
	if (get_tree_prefix_len(&tree) != 8) {
		fprintf(stderr, "%d: get_tree_prefix_len() failed\n", __LINE__);
		//FAIL();
		exit(1);
	}

	tree.Rmax = 6;
	tree.hostA = 2;
	tree.ip_type = IPV4;
	if (get_tree_prefix_len(&tree) != 24) {
		fprintf(stderr, "%d: get_tree_prefix_len() failed\n", __LINE__);
		//FAIL();
		exit(1);
	}
	tree.Rmax = 22;	/* 22 ranks IPv4 tree (maximum size for private addressing) */
	if (get_tree_prefix_len(&tree) != 8) {
		fprintf(stderr, "%d: get_tree_prefix_len() failed\n", __LINE__);
		//FAIL();
		exit(1);
	}

	printf("%s: tests passed\n", __func__);
}

/* Unit test for get_hosts_prefix_len()
 */
void test_get_hosts_prefix_len() {
	self_ip_routing_tree_t tree;

	tree.hostA = 0;
	tree.ip_type = IPV6;
	if (get_hosts_prefix_len(&tree) != 128) {
		fprintf(stderr, "%d: get_hosts_prefix_len() failed\n", __LINE__);
		//FAIL();
		exit(1);
	}
	tree.hostA = 2;
	if (get_hosts_prefix_len(&tree) != 126) {
		fprintf(stderr, "%d: get_hosts_prefix_len() failed\n", __LINE__);
		//FAIL();
		exit(1);
	}
	tree.hostA = 8;
	if (get_hosts_prefix_len(&tree) != 120) {
		fprintf(stderr, "%d: get_hosts_prefix_len() failed\n", __LINE__);
		//FAIL();
		exit(1);
	}

	tree.hostA = 2;
	tree.ip_type = IPV4;
	if (get_hosts_prefix_len(&tree) != 30) {
		fprintf(stderr, "%d: get_hosts_prefix_len() failed\n", __LINE__);
		//FAIL();
		exit(1);
	}
	tree.hostA = 8;
	if (get_hosts_prefix_len(&tree) != 24) {
		fprintf(stderr, "%d: get_hosts_prefix_len() failed\n", __LINE__);
		//FAIL();
		exit(1);
	}

	printf("%s: tests passed\n", __func__);
}

/* Unit test for ipv6_prefix_to_uint128_t_mask()
 */
void test_ipv6_prefix_to_uint128_t_mask() {
	uint128_t test_netmask;
	char result[33];
	char* expected_result;

	test_netmask = ipv6_prefix_to_uint128_t_mask(0);
	uint128_t_to_hexstr((uint128_t)test_netmask, 16, result);
	expected_result = "0000000000000000" "0000000000000000";
	if (strcmp(result, expected_result) != 0) {
		fprintf(stderr, "%d: ipv6_prefix_to_uint128_t_mask() failed, got:\n\"%s\", expected:\n\"%s\"\n", __LINE__, result, expected_result);
		//FAIL();
		exit(1);
	}

	test_netmask = ipv6_prefix_to_uint128_t_mask(1);
	uint128_t_to_hexstr((uint128_t)test_netmask, 16, result);
	expected_result = "8000000000000000" "0000000000000000";
	if (strcmp(result, expected_result) != 0) {
		fprintf(stderr, "%d: ipv6_prefix_to_uint128_t_mask() failed, got:\n\"%s\", expected:\n\"%s\"\n", __LINE__, result, expected_result);
		//FAIL();
		exit(1);
	}

	test_netmask = ipv6_prefix_to_uint128_t_mask(64);
	uint128_t_to_hexstr((uint128_t)test_netmask, 16, result);
	expected_result = "ffffffffffffffff" "0000000000000000";
	if (strcmp(result, expected_result) != 0) {
		fprintf(stderr, "%d: ipv6_prefix_to_uint128_t_mask() failed, got:\n\"%s\", expected:\n\"%s\"\n", __LINE__, result, expected_result);
		//FAIL();
		exit(1);
	}

	test_netmask = ipv6_prefix_to_uint128_t_mask(127);
	uint128_t_to_hexstr((uint128_t)test_netmask, 16, result);
	expected_result = "ffffffffffffffff" "fffffffffffffffe";
	if (strcmp(result, expected_result) != 0) {
		fprintf(stderr, "%d: ipv6_prefix_to_uint128_t_mask() failed, got:\n\"%s\", expected:\n\"%s\"\n", __LINE__, result, expected_result);
		//FAIL();
		exit(1);
	}

	test_netmask = ipv6_prefix_to_uint128_t_mask(128);
	uint128_t_to_hexstr((uint128_t)test_netmask, 16, result);
	expected_result = "ffffffffffffffff" "ffffffffffffffff";
	if (strcmp(result, expected_result) != 0) {
		fprintf(stderr, "%d: ipv6_prefix_to_uint128_t_mask() failed, got:\n\"%s\", expected:\n\"%s\"\n", __LINE__, result, expected_result);
		//FAIL();
		exit(1);
	}

	printf("%s: tests passed\n", __func__);
}

/* Unit test for get_root_node_id()
 */
void test_get_root_node_id() {
	node_id_t test_node;
	char result[33];
	char* expected_result;
	self_ip_routing_tree_t tree;

	tree.Rmax = 4;
	tree.ip_type = IPV6;

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
	self_ip_routing_tree_t tree;

	tree.Rmax = 4;
	tree.ip_type = IPV6;
	tree.hostA = 0;

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
	char result[33];
	char* expected_result;
	self_ip_routing_tree_t tree;

	tree.Rmax = 4;
	tree.ip_type = IPV6;

	/* Start from root, and descent on left child at each rank */
	test_node = get_root_node_id(&tree);
	test_node = get_left_child_node_id(&tree, test_node);
	uint128_t_to_hexstr((uint128_t)test_node, 16, result);
	expected_result = "0000000000000000" "0000000000000004";	/* Left child for root node 8 should have ID 4 */
	if (strcmp(result, expected_result) != 0) {
		fprintf(stderr, "%d: get_left_child_node_id() failed, got:\n\"%s\", expected:\n\"%s\"\n", __LINE__, result, expected_result);
		//FAIL();
		exit(1);
	}
	test_node = get_left_child_node_id(&tree, test_node);
	uint128_t_to_hexstr((uint128_t)test_node, 16, result);
	expected_result = "0000000000000000" "0000000000000002";	/* Left child for node 4 should have ID 2 */
	if (strcmp(result, expected_result) != 0) {
		fprintf(stderr, "%d: get_left_child_node_id() failed, got:\n\"%s\", expected:\n\"%s\"\n", __LINE__, result, expected_result);
		//FAIL();
		exit(1);
	}
	test_node = get_left_child_node_id(&tree, test_node);
	uint128_t_to_hexstr((uint128_t)test_node, 16, result);
	expected_result = "0000000000000000" "0000000000000001";	/* Left child for node 2 should have ID 1 */
	if (strcmp(result, expected_result) != 0) {
		fprintf(stderr, "%d: get_left_child_node_id() failed, got:\n\"%s\", expected:\n\"%s\"\n", __LINE__, result, expected_result);
		//FAIL();
		exit(1);
	}
	test_node = get_left_child_node_id(&tree, test_node);
	uint128_t_to_hexstr((uint128_t)test_node, 16, result);
	expected_result = "0000000000000000" "0000000000000000";	/* Left child for node 1 is invalid */
	if (strcmp(result, expected_result) != 0) {
		fprintf(stderr, "%d: get_left_child_node_id() failed, got:\n\"%s\", expected:\n\"%s\"\n", __LINE__, result, expected_result);
		//FAIL();
		exit(1);
	}

	/* Start from node 12, and descent on left child at each rank */
	test_node = (node_id_t)uint16_t_to_uint128_t(12);
	test_node = get_left_child_node_id(&tree, test_node);
	uint128_t_to_hexstr((uint128_t)test_node, 16, result);
	expected_result = "0000000000000000" "000000000000000a";	/* Left child for node 12 should have ID 10 */
	if (strcmp(result, expected_result) != 0) {
		fprintf(stderr, "%d: get_left_child_node_id() failed, got:\n\"%s\", expected:\n\"%s\"\n", __LINE__, result, expected_result);
		//FAIL();
		exit(1);
	}
	test_node = get_left_child_node_id(&tree, test_node);
	uint128_t_to_hexstr((uint128_t)test_node, 16, result);
	expected_result = "0000000000000000" "0000000000000009";	/* Left child for node 10 should have ID 9 */
	if (strcmp(result, expected_result) != 0) {
		fprintf(stderr, "%d: get_left_child_node_id() failed, got:\n\"%s\", expected:\n\"%s\"\n", __LINE__, result, expected_result);
		//FAIL();
		exit(1);
	}
	test_node = get_left_child_node_id(&tree, test_node);
	uint128_t_to_hexstr((uint128_t)test_node, 16, result);
	expected_result = "0000000000000000" "0000000000000000";	/* Left child for node 9 is invalid */
	if (strcmp(result, expected_result) != 0) {
		fprintf(stderr, "%d: get_left_child_node_id() failed, got:\n\"%s\", expected:\n\"%s\"\n", __LINE__, result, expected_result);
		//FAIL();
		exit(1);
	}

	test_node = (node_id_t)uint16_t_to_uint128_t(0xffff);
	test_node = get_left_child_node_id(&tree, test_node);
	uint128_t_to_hexstr((uint128_t)test_node, 16, result);
	expected_result = "0000000000000000" "0000000000000000";	/* Node 0xffff is invalid */
	if (strcmp(result, expected_result) != 0) {
		fprintf(stderr, "%d: get_left_child_node_id() failed, got:\n\"%s\", expected:\n\"%s\"\n", __LINE__, result, expected_result);
		//FAIL();
		exit(1);
	}

	tree.Rmax = 6;
	tree.ip_type = IPV4;

	/* Start from root, and descent on left child at each rank */
	test_node = get_root_node_id(&tree);
	test_node = get_left_child_node_id(&tree, test_node);
	uint128_t_to_hexstr((uint128_t)test_node, 16, result);
	expected_result = "0000000000000000" "0000000000000010";	/* Left child for root node 32 should have ID 16 */
	if (strcmp(result, expected_result) != 0) {
		fprintf(stderr, "%d: get_left_child_node_id() failed, got:\n\"%s\", expected:\n\"%s\"\n", __LINE__, result, expected_result);
		//FAIL();
		exit(1);
	}
	test_node = get_left_child_node_id(&tree, test_node);
	uint128_t_to_hexstr((uint128_t)test_node, 16, result);
	expected_result = "0000000000000000" "0000000000000008";	/* Left child for node 16 should have ID 8 */
	if (strcmp(result, expected_result) != 0) {
		fprintf(stderr, "%d: get_left_child_node_id() failed, got:\n\"%s\", expected:\n\"%s\"\n", __LINE__, result, expected_result);
		//FAIL();
		exit(1);
	}
	test_node = get_left_child_node_id(&tree, test_node);
	uint128_t_to_hexstr((uint128_t)test_node, 16, result);
	expected_result = "0000000000000000" "0000000000000004";	/* Left child for node 8 should have ID 4 */
	if (strcmp(result, expected_result) != 0) {
		fprintf(stderr, "%d: get_left_child_node_id() failed, got:\n\"%s\", expected:\n\"%s\"\n", __LINE__, result, expected_result);
		//FAIL();
		exit(1);
	}
	test_node = get_left_child_node_id(&tree, test_node);
	uint128_t_to_hexstr((uint128_t)test_node, 16, result);
	expected_result = "0000000000000000" "0000000000000002";	/* Left child for node 4 should have ID 2 */
	if (strcmp(result, expected_result) != 0) {
		fprintf(stderr, "%d: get_left_child_node_id() failed, got:\n\"%s\", expected:\n\"%s\"\n", __LINE__, result, expected_result);
		//FAIL();
		exit(1);
	}
	test_node = get_left_child_node_id(&tree, test_node);
	uint128_t_to_hexstr((uint128_t)test_node, 16, result);
	expected_result = "0000000000000000" "0000000000000001";	/* Left child for node 2 should have ID 1 */
	if (strcmp(result, expected_result) != 0) {
		fprintf(stderr, "%d: get_left_child_node_id() failed, got:\n\"%s\", expected:\n\"%s\"\n", __LINE__, result, expected_result);
		//FAIL();
		exit(1);
	}
	test_node = get_left_child_node_id(&tree, test_node);
	uint128_t_to_hexstr((uint128_t)test_node, 16, result);
	expected_result = "0000000000000000" "0000000000000000";	/* Left child for node 1 is invalid */
	if (strcmp(result, expected_result) != 0) {
		fprintf(stderr, "%d: get_left_child_node_id() failed, got:\n\"%s\", expected:\n\"%s\"\n", __LINE__, result, expected_result);
		//FAIL();
		exit(1);
	}

	/* Start from node 48, and descent on left child at each rank */
	test_node = (node_id_t)uint16_t_to_uint128_t(48);
	test_node = get_left_child_node_id(&tree, test_node);
	uint128_t_to_hexstr((uint128_t)test_node, 16, result);
	expected_result = "0000000000000000" "0000000000000028";	/* Left child for node 48 should have ID 40 */
	if (strcmp(result, expected_result) != 0) {
		fprintf(stderr, "%d: get_left_child_node_id() failed, got:\n\"%s\", expected:\n\"%s\"\n", __LINE__, result, expected_result);
		//FAIL();
		exit(1);
	}
	test_node = get_left_child_node_id(&tree, test_node);
	uint128_t_to_hexstr((uint128_t)test_node, 16, result);
	expected_result = "0000000000000000" "0000000000000024";	/* Left child for node 40 should have ID 36 */
	if (strcmp(result, expected_result) != 0) {
		fprintf(stderr, "%d: get_left_child_node_id() failed, got:\n\"%s\", expected:\n\"%s\"\n", __LINE__, result, expected_result);
		//FAIL();
		exit(1);
	}
	test_node = get_left_child_node_id(&tree, test_node);
	uint128_t_to_hexstr((uint128_t)test_node, 16, result);
	expected_result = "0000000000000000" "0000000000000022";	/* Left child for node 36 should have ID 34 */
	if (strcmp(result, expected_result) != 0) {
		fprintf(stderr, "%d: get_left_child_node_id() failed, got:\n\"%s\", expected:\n\"%s\"\n", __LINE__, result, expected_result);
		//FAIL();
		exit(1);
	}
	test_node = get_left_child_node_id(&tree, test_node);
	uint128_t_to_hexstr((uint128_t)test_node, 16, result);
	expected_result = "0000000000000000" "0000000000000021";	/* Left child for node 34 should have ID 33 */
	if (strcmp(result, expected_result) != 0) {
		fprintf(stderr, "%d: get_left_child_node_id() failed, got:\n\"%s\", expected:\n\"%s\"\n", __LINE__, result, expected_result);
		//FAIL();
		exit(1);
	}
	test_node = get_left_child_node_id(&tree, test_node);
	uint128_t_to_hexstr((uint128_t)test_node, 16, result);
	expected_result = "0000000000000000" "0000000000000000";	/* Left child for node 33 is invalid */
	if (strcmp(result, expected_result) != 0) {
		fprintf(stderr, "%d: get_left_child_node_id() failed, got:\n\"%s\", expected:\n\"%s\"\n", __LINE__, result, expected_result);
		//FAIL();
		exit(1);
	}

	test_node = (node_id_t)uint16_t_to_uint128_t(0xffff);
	test_node = get_left_child_node_id(&tree, test_node);
	uint128_t_to_hexstr((uint128_t)test_node, 16, result);
	expected_result = "0000000000000000" "0000000000000000";	/* Node 0xffff is invalid */
	if (strcmp(result, expected_result) != 0) {
		fprintf(stderr, "%d: get_left_child_node_id() failed, got:\n\"%s\", expected:\n\"%s\"\n", __LINE__, result, expected_result);
		//FAIL();
		exit(1);
	}

	printf("%s: tests passed\n", __func__);
}

/* Unit test for get_right_child_node_id()
 */
void test_get_right_child_node_id() {
	node_id_t test_node;
	char result[33];
	char* expected_result;
	self_ip_routing_tree_t tree;

	tree.Rmax = 4;
	tree.ip_type = IPV6;

	/* Start from root, and descent on right child at each rank */
	test_node = get_root_node_id(&tree);
	test_node = get_right_child_node_id(&tree, test_node);
	uint128_t_to_hexstr((uint128_t)test_node, 16, result);
	expected_result = "0000000000000000" "000000000000000c";	/* Right child for root node 8 should have ID 12 */
	if (strcmp(result, expected_result) != 0) {
		fprintf(stderr, "%d: get_right_child_node_id() failed, got:\n\"%s\", expected:\n\"%s\"\n", __LINE__, result, expected_result);
		//FAIL();
		exit(1);
	}
	test_node = get_right_child_node_id(&tree, test_node);
	uint128_t_to_hexstr((uint128_t)test_node, 16, result);
	expected_result = "0000000000000000" "000000000000000e";	/* Right child for node 12 should have ID 14 */
	if (strcmp(result, expected_result) != 0) {
		fprintf(stderr, "%d: get_right_child_node_id() failed, got:\n\"%s\", expected:\n\"%s\"\n", __LINE__, result, expected_result);
		//FAIL();
		exit(1);
	}
	test_node = get_right_child_node_id(&tree, test_node);
	uint128_t_to_hexstr((uint128_t)test_node, 16, result);
	expected_result = "0000000000000000" "000000000000000f";	/* Right child for node 14 should have ID 15 */
	if (strcmp(result, expected_result) != 0) {
		fprintf(stderr, "%d: get_right_child_node_id() failed, got:\n\"%s\", expected:\n\"%s\"\n", __LINE__, result, expected_result);
		//FAIL();
		exit(1);
	}
	test_node = get_right_child_node_id(&tree, test_node);
	uint128_t_to_hexstr((uint128_t)test_node, 16, result);
	expected_result = "0000000000000000" "0000000000000000";	/* Right child for node 15 is invalid */
	if (strcmp(result, expected_result) != 0) {
		fprintf(stderr, "%d: get_right_child_node_id() failed, got:\n\"%s\", expected:\n\"%s\"\n", __LINE__, result, expected_result);
		//FAIL();
		exit(1);
	}

	/* Start from node 4, and descent on right child at each rank */
	test_node = (node_id_t)uint16_t_to_uint128_t(4);
	test_node = get_right_child_node_id(&tree, test_node);
	uint128_t_to_hexstr((uint128_t)test_node, 16, result);
	expected_result = "0000000000000000" "0000000000000006";	/* Right child for node 4 should have ID 6 */
	if (strcmp(result, expected_result) != 0) {
		fprintf(stderr, "%d: get_right_child_node_id() failed, got:\n\"%s\", expected:\n\"%s\"\n", __LINE__, result, expected_result);
		//FAIL();
		exit(1);
	}
	test_node = get_right_child_node_id(&tree, test_node);
	uint128_t_to_hexstr((uint128_t)test_node, 16, result);
	expected_result = "0000000000000000" "0000000000000007";	/* Right child for node 6 should have ID 7 */
	if (strcmp(result, expected_result) != 0) {
		fprintf(stderr, "%d: get_right_child_node_id() failed, got:\n\"%s\", expected:\n\"%s\"\n", __LINE__, result, expected_result);
		//FAIL();
		exit(1);
	}
	test_node = get_right_child_node_id(&tree, test_node);
	uint128_t_to_hexstr((uint128_t)test_node, 16, result);
	expected_result = "0000000000000000" "0000000000000000";	/* Right child for node 7 is invalid */
	if (strcmp(result, expected_result) != 0) {
		fprintf(stderr, "%d: get_right_child_node_id() failed, got:\n\"%s\", expected:\n\"%s\"\n", __LINE__, result, expected_result);
		//FAIL();
		exit(1);
	}

	test_node = (node_id_t)uint16_t_to_uint128_t(0xffff);
	test_node = get_right_child_node_id(&tree, test_node);
	uint128_t_to_hexstr((uint128_t)test_node, 16, result);
	expected_result = "0000000000000000" "0000000000000000";	/* Node 0xffff is invalid */
	if (strcmp(result, expected_result) != 0) {
		fprintf(stderr, "%d: get_right_child_node_id() failed, got:\n\"%s\", expected:\n\"%s\"\n", __LINE__, result, expected_result);
		//FAIL();
		exit(1);
	}

	tree.Rmax = 6;
	tree.ip_type = IPV4;

	/* Start from root, and descent on right child at each rank */
	test_node = get_root_node_id(&tree);
	test_node = get_right_child_node_id(&tree, test_node);
	uint128_t_to_hexstr((uint128_t)test_node, 16, result);
	expected_result = "0000000000000000" "0000000000000030";	/* Right child for root node 32 should have ID 48 */
	if (strcmp(result, expected_result) != 0) {
		fprintf(stderr, "%d: get_right_child_node_id() failed, got:\n\"%s\", expected:\n\"%s\"\n", __LINE__, result, expected_result);
		//FAIL();
		exit(1);
	}
	test_node = get_right_child_node_id(&tree, test_node);
	uint128_t_to_hexstr((uint128_t)test_node, 16, result);
	expected_result = "0000000000000000" "0000000000000038";	/* Right child for node 48 should have ID 56 */
	if (strcmp(result, expected_result) != 0) {
		fprintf(stderr, "%d: get_right_child_node_id() failed, got:\n\"%s\", expected:\n\"%s\"\n", __LINE__, result, expected_result);
		//FAIL();
		exit(1);
	}
	test_node = get_right_child_node_id(&tree, test_node);
	uint128_t_to_hexstr((uint128_t)test_node, 16, result);
	expected_result = "0000000000000000" "000000000000003c";	/* Right child for node 56 should have ID 60 */
	if (strcmp(result, expected_result) != 0) {
		fprintf(stderr, "%d: get_right_child_node_id() failed, got:\n\"%s\", expected:\n\"%s\"\n", __LINE__, result, expected_result);
		//FAIL();
		exit(1);
	}
	test_node = get_right_child_node_id(&tree, test_node);
	uint128_t_to_hexstr((uint128_t)test_node, 16, result);
	expected_result = "0000000000000000" "000000000000003e";	/* Right child for node 60 should have ID 62 */
	if (strcmp(result, expected_result) != 0) {
		fprintf(stderr, "%d: get_right_child_node_id() failed, got:\n\"%s\", expected:\n\"%s\"\n", __LINE__, result, expected_result);
		//FAIL();
		exit(1);
	}
	test_node = get_right_child_node_id(&tree, test_node);
	uint128_t_to_hexstr((uint128_t)test_node, 16, result);
	expected_result = "0000000000000000" "000000000000003f";	/* Right child for node 62 should have ID 63 */
	if (strcmp(result, expected_result) != 0) {
		fprintf(stderr, "%d: get_right_child_node_id() failed, got:\n\"%s\", expected:\n\"%s\"\n", __LINE__, result, expected_result);
		//FAIL();
		exit(1);
	}
	test_node = get_right_child_node_id(&tree, test_node);
	uint128_t_to_hexstr((uint128_t)test_node, 16, result);
	expected_result = "0000000000000000" "0000000000000000";	/* Right child for node 63 is invalid */
	if (strcmp(result, expected_result) != 0) {
		fprintf(stderr, "%d: get_right_child_node_id() failed, got:\n\"%s\", expected:\n\"%s\"\n", __LINE__, result, expected_result);
		//FAIL();
		exit(1);
	}

	/* Start from node 16, and descent on right child at each rank */
	test_node = (node_id_t)uint16_t_to_uint128_t(16);
	test_node = get_right_child_node_id(&tree, test_node);
	uint128_t_to_hexstr((uint128_t)test_node, 16, result);
	expected_result = "0000000000000000" "0000000000000018";	/* Right child for node 16 should have ID 24 */
	if (strcmp(result, expected_result) != 0) {
		fprintf(stderr, "%d: get_right_child_node_id() failed, got:\n\"%s\", expected:\n\"%s\"\n", __LINE__, result, expected_result);
		//FAIL();
		exit(1);
	}
	test_node = get_right_child_node_id(&tree, test_node);
	uint128_t_to_hexstr((uint128_t)test_node, 16, result);
	expected_result = "0000000000000000" "000000000000001c";	/* Right child for node 24 should have ID 28 */
	if (strcmp(result, expected_result) != 0) {
		fprintf(stderr, "%d: get_right_child_node_id() failed, got:\n\"%s\", expected:\n\"%s\"\n", __LINE__, result, expected_result);
		//FAIL();
		exit(1);
	}
	test_node = get_right_child_node_id(&tree, test_node);
	uint128_t_to_hexstr((uint128_t)test_node, 16, result);
	expected_result = "0000000000000000" "000000000000001e";	/* Right child for node 28 should have ID 30 */
	if (strcmp(result, expected_result) != 0) {
		fprintf(stderr, "%d: get_right_child_node_id() failed, got:\n\"%s\", expected:\n\"%s\"\n", __LINE__, result, expected_result);
		//FAIL();
		exit(1);
	}
	test_node = get_right_child_node_id(&tree, test_node);
	uint128_t_to_hexstr((uint128_t)test_node, 16, result);
	expected_result = "0000000000000000" "000000000000001f";	/* Right child for node 30 should have ID 31 */
	if (strcmp(result, expected_result) != 0) {
		fprintf(stderr, "%d: get_right_child_node_id() failed, got:\n\"%s\", expected:\n\"%s\"\n", __LINE__, result, expected_result);
		//FAIL();
		exit(1);
	}
	test_node = get_right_child_node_id(&tree, test_node);
	uint128_t_to_hexstr((uint128_t)test_node, 16, result);
	expected_result = "0000000000000000" "0000000000000000";	/* Right child for node 33 is invalid */
	if (strcmp(result, expected_result) != 0) {
		fprintf(stderr, "%d: get_right_child_node_id() failed, got:\n\"%s\", expected:\n\"%s\"\n", __LINE__, result, expected_result);
		//FAIL();
		exit(1);
	}

	test_node = (node_id_t)uint16_t_to_uint128_t(0xffff);
	test_node = get_right_child_node_id(&tree, test_node);
	uint128_t_to_hexstr((uint128_t)test_node, 16, result);
	expected_result = "0000000000000000" "0000000000000000";	/* Node 0xffff is invalid */
	if (strcmp(result, expected_result) != 0) {
		fprintf(stderr, "%d: get_right_child_node_id() failed, got:\n\"%s\", expected:\n\"%s\"\n", __LINE__, result, expected_result);
		//FAIL();
		exit(1);
	}

	printf("%s: tests passed\n", __func__);
}

/* Unit test for get_top_interface_config()
 */
void test_get_top_interface_config() {
	node_id_t test_node;
	self_ip_routing_tree_t tree;
	if_ip_addr_t ip_addr_result;
	char ip_addr_str[INET6_ADDRSTRLEN+1];

	tree.ip_type = IPV6;
	tree.Rmax = 4;
	tree.hostA = 0;
	set_zero_uint128_t(tree.prefix);
	tree.prefix.uint128_a8[0] = 0xfd;	/* Prefix is fd00::/124 */

	test_node = get_root_node_id(&tree);	/* Will get 8 */
	ip_addr_result = get_top_interface_config(&tree, test_node);

	inet_ntop(AF_INET6, &(ip_addr_result.in_addr.__ipv6_in6_addr), ip_addr_str, INET6_ADDRSTRLEN);

	//TODO: check that tree was not altered as side effect when passed as a reference above
	if (ip_addr_result.ip_type != IPV6) {	/* ip_type should have been propagated as is to result */
		fprintf(stderr, "%d: get_top_interface_config() modified ip_type field\n", __LINE__);
		//FAIL();
		exit(1);
	}
	if (strcmp(ip_addr_str, "fd00::8") != 0) {	/* Should get fd00::8 for root node's top interface */
		fprintf(stderr, "%d: get_top_interface_config() got wrong IP address: %s\n", __LINE__, ip_addr_str);
		//FAIL();
		exit(1);
	}
	if (ip_addr_result.prefix != 128) {	/* Should get /128 for root node's top interface netmask */
		fprintf(stderr, "%d: get_top_interface_config() got wrong netmask: %d\n", __LINE__, ip_addr_result.prefix);
		//FAIL();
		exit(1);
	}

	test_node = get_left_child_node_id(&tree, get_root_node_id(&tree));	/* Will get 4 */
	ip_addr_result = get_top_interface_config(&tree, test_node);

	inet_ntop(AF_INET6, &(ip_addr_result.in_addr.__ipv6_in6_addr), ip_addr_str, INET6_ADDRSTRLEN);

	//TODO: check that tree was not altered as side effect when passed as a reference above
	if (ip_addr_result.ip_type != IPV6) {	/* ip_type should have been propagated as is to result */
		fprintf(stderr, "%d: get_top_interface_config() modified ip_type field\n", __LINE__);
		//FAIL();
		exit(1);
	}
	if (strcmp(ip_addr_str, "fd00::4") != 0) {	/* Should get fd00::8 for root node's top interface */
		fprintf(stderr, "%d: get_top_interface_config() got wrong IP address: %s\n", __LINE__, ip_addr_str);
		//FAIL();
		exit(1);
	}
	if (ip_addr_result.prefix != 128) {	/* Should get /128 for root node's top interface netmask */
		fprintf(stderr, "%d: get_top_interface_config() got wrong netmask: %d\n", __LINE__, ip_addr_result.prefix);
		//FAIL();
		exit(1);
	}

	test_node = get_right_child_node_id(&tree, get_root_node_id(&tree));	/* Will get 12 */
	ip_addr_result = get_top_interface_config(&tree, test_node);

	inet_ntop(AF_INET6, &(ip_addr_result.in_addr.__ipv6_in6_addr), ip_addr_str, INET6_ADDRSTRLEN);

	//TODO: check that tree was not altered as side effect when passed as a reference above
	if (ip_addr_result.ip_type != IPV6) {	/* ip_type should have been propagated as is to result */
		fprintf(stderr, "%d: get_top_interface_config() modified ip_type field\n", __LINE__);
		//FAIL();
		exit(1);
	}
	if (strcmp(ip_addr_str, "fd00::c") != 0) {	/* Should get fd00::8 for root node's top interface */
		fprintf(stderr, "%d: get_top_interface_config() got wrong IP address: %s\n", __LINE__, ip_addr_str);
		//FAIL();
		exit(1);
	}
	if (ip_addr_result.prefix != 128) {	/* Should get /128 for root node's top interface netmask */
		fprintf(stderr, "%d: get_top_interface_config() got wrong netmask: %d\n", __LINE__, ip_addr_result.prefix);
		//FAIL();
		exit(1);
	}

	test_node = uint8_t_to_uint128_t(1);	/* Take left-most leaf of tree (ID 1) */
	ip_addr_result = get_top_interface_config(&tree, test_node);

	inet_ntop(AF_INET6, &(ip_addr_result.in_addr.__ipv6_in6_addr), ip_addr_str, INET6_ADDRSTRLEN);

	//TODO: check that tree was not altered as side effect when passed as a reference above
	if (ip_addr_result.ip_type != IPV6) {	/* ip_type should have been propagated as is to result */
		fprintf(stderr, "%d: get_top_interface_config() modified ip_type field\n", __LINE__);
		//FAIL();
		exit(1);
	}
	if (strcmp(ip_addr_str, "fd00::1") != 0) {	/* Should get fd00::f for tree's left-most leaf's top interface */
		fprintf(stderr, "%d: get_top_interface_config() got wrong IP address: %s\n", __LINE__, ip_addr_str);
		//FAIL();
		exit(1);
	}
	if (ip_addr_result.prefix != 128) {	/* Should get /128 for root node's top interface netmask */
		fprintf(stderr, "%d: get_top_interface_config() got wrong netmask: %d\n", __LINE__, ip_addr_result.prefix);
		//FAIL();
		exit(1);
	}
	test_node = uint8_t_to_uint128_t(15);	/* Take right-most leaf of tree (ID 15) */
	ip_addr_result = get_top_interface_config(&tree, test_node);

	inet_ntop(AF_INET6, &(ip_addr_result.in_addr.__ipv6_in6_addr), ip_addr_str, INET6_ADDRSTRLEN);

	//TODO: check that tree was not altered as side effect when passed as a reference above
	if (ip_addr_result.ip_type != IPV6) {	/* ip_type should have been propagated as is to result */
		fprintf(stderr, "%d: get_top_interface_config() modified ip_type field\n", __LINE__);
		//FAIL();
		exit(1);
	}
	if (strcmp(ip_addr_str, "fd00::f") != 0) {	/* Should get fd00::f for tree's right-most leaf's top interface */
		fprintf(stderr, "%d: get_top_interface_config() got wrong IP address: %s\n", __LINE__, ip_addr_str);
		//FAIL();
		exit(1);
	}
	if (ip_addr_result.prefix != 128) {	/* Should get /128 for root node's top interface netmask */
		fprintf(stderr, "%d: get_top_interface_config() got wrong netmask: %d\n", __LINE__, ip_addr_result.prefix);
		//FAIL();
		exit(1);
	}

	tree.ip_type = IPV4;
	tree.Rmax = 6;
	tree.hostA = 2;
	set_zero_uint128_t(tree.prefix);
	tree.prefix.uint128_a8[12] = 192;
	tree.prefix.uint128_a8[13] = 168;
	tree.prefix.uint128_a8[14] = 0;
	tree.prefix.uint128_a8[15] = 0;	/* Prefix is 192.168.0.0/24 */

	//ip_addr_result = get_top_interface_config(&tree, test_node);
	//FIXME: Add unit test for get_top_interface_config() on IPv4 trees

	//if (ip_addr_result.ip_type != IPV4) {	/* ip_type should have been propagated as is to result */
	//	fprintf(stderr, "%d: get_top_interface_config() modified ip_type field\n", __LINE__);
	//	//FAIL();
	//	exit(1);
	//}

	printf("%s: tests passed\n", __func__);
}

/* Unit test for get_left_interface_config()
 */
void test_get_left_interface_config() {
	node_id_t test_node;
	self_ip_routing_tree_t tree;
	if_ip_addr_t ip_addr_result;
	char ip_addr_str[INET6_ADDRSTRLEN+1];

	tree.ip_type = IPV6;
	tree.Rmax = 4;
	tree.hostA = 0;
	set_zero_uint128_t(tree.prefix);
	tree.prefix.uint128_a8[0] = 0xfd;	/* Prefix is fd00::/124 */

	test_node = get_root_node_id(&tree);	/* Will get 8 */
	ip_addr_result = get_left_interface_config(&tree, test_node);

	inet_ntop(AF_INET6, &(ip_addr_result.in_addr.__ipv6_in6_addr), ip_addr_str, INET6_ADDRSTRLEN);

	//TODO: check that tree was not altered as side effect when passed as a reference above
	if (ip_addr_result.ip_type != NONE) {	/* IP type should be set to none, IPv6 trees do require setting addresses to interfaces to children */
		fprintf(stderr, "%d: get_left_interface_config() modified ip_type field\n", __LINE__);
		//FAIL();
		exit(1);
	}

	tree.ip_type = IPV4;
	tree.Rmax = 6;
	tree.hostA = 2;
	set_zero_uint128_t(tree.prefix);
	tree.prefix.uint128_a8[12] = 192;
	tree.prefix.uint128_a8[13] = 168;
	tree.prefix.uint128_a8[14] = 0;
	tree.prefix.uint128_a8[15] = 0;	/* Prefix is 192.168.0.0/24 */

	//ip_addr_result = get_left_interface_config(&tree, test_node);
	//FIXME: Add unit test for get_left_interface_config() on IPv4 trees

	//if (ip_addr_result.ip_type != IPV4) {	/* ip_type should have been propagated as is to result */
	//	fprintf(stderr, "%d: get_left_interface_config() modified ip_type field\n", __LINE__);
	//	//FAIL();
	//	exit(1);
	//}

	printf("%s: tests passed\n", __func__);
}

/* Unit test for get_right_interface_config()
 */
void test_get_right_interface_config() {
	node_id_t test_node;
	self_ip_routing_tree_t tree;
	if_ip_addr_t ip_addr_result;
	char ip_addr_str[INET6_ADDRSTRLEN+1];

	tree.ip_type = IPV6;
	tree.Rmax = 4;
	tree.hostA = 0;
	set_zero_uint128_t(tree.prefix);
	tree.prefix.uint128_a8[0] = 0xfd;	/* Prefix is fd00::/124 */

	test_node = get_root_node_id(&tree);	/* Will get 8 */
	ip_addr_result = get_left_interface_config(&tree, test_node);

	inet_ntop(AF_INET6, &(ip_addr_result.in_addr.__ipv6_in6_addr), ip_addr_str, INET6_ADDRSTRLEN);

	//TODO: check that tree was not altered as side effect when passed as a reference above
	if (ip_addr_result.ip_type != NONE) {	/* IP type should be set to none, IPv6 trees do require setting addresses to interfaces to children */
		fprintf(stderr, "%d: get_right_interface_config() modified ip_type field\n", __LINE__);
		//FAIL();
		exit(1);
	}

	tree.ip_type = IPV4;
	tree.Rmax = 6;
	tree.hostA = 2;
	set_zero_uint128_t(tree.prefix);
	tree.prefix.uint128_a8[12] = 192;
	tree.prefix.uint128_a8[13] = 168;
	tree.prefix.uint128_a8[14] = 0;
	tree.prefix.uint128_a8[15] = 0;	/* Prefix is 192.168.0.0/24 */

	//ip_addr_result = get_right_interface_config(&tree, test_node);
	//FIXME: Add unit test for get_right_interface_config() on IPv4 trees

	//if (ip_addr_result.ip_type != IPV4) {	/* ip_type should have been propagated as is to result */
	//	fprintf(stderr, "%d: get_right_interface_config() modified ip_type field\n", __LINE__);
	//	//FAIL();
	//	exit(1);
	//}

	printf("%s: tests passed\n", __func__);
}
