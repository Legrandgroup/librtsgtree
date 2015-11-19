#include "node.h"
#include "test_harness.h"
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>	// For inet_ntop() and htons()
#include <stdint.h>
#include <string.h>	// For memcpy()

TEST_GROUP(node_tests) {
};

void check_result_is_uint128(uint128_t value, uint128_t expected, unsigned int func_line) {
	char value_str[33];
	char expected_str[33];
	
	if (uint128_t_cmp(value, expected) != 0) {
		uint128_t_to_hexstr((uint128_t)value, 16, value_str);
		uint128_t_to_hexstr((uint128_t)expected, 16, expected_str);
		FAILF("at source line %d: uint128 comparison failed, got:\n\"%s\", expected:\n\"%s\"\n", func_line, value_str, expected_str);
	}	
}

/* Unit test for Rmax_to_max_node_id()
 */
TEST(node_tests, test_Rmax_to_max_node_id) {
	node_id_t test_node;

	if (!U128_IS_ZERO(Rmax_to_max_node_id(0)))
		FAILF("Rmax_to_max_node_id() failed\n");
	if (uint128_t_cmp(Rmax_to_max_node_id(1), uint8_t_to_uint128_t(1)) != 0)
		FAILF("Rmax_to_max_node_id() failed\n");
	if (uint128_t_cmp(Rmax_to_max_node_id(2), uint8_t_to_uint128_t(3)) != 0)
		FAILF("Rmax_to_max_node_id() failed\n");
	if (uint128_t_cmp(Rmax_to_max_node_id(8), uint8_t_to_uint128_t(0xff)) != 0)
		FAILF("Rmax_to_max_node_id() failed\n");
	if (uint128_t_cmp(Rmax_to_max_node_id(16), uint16_t_to_uint128_t(0xffff)) != 0)
		FAILF("Rmax_to_max_node_id() failed\n");

#ifndef HAS_INT128
	U128_SET_MAX(test_node);
	test_node.uint128_a8[0] = 0x7f;
#else
	test_node = uint128_t_max() >> 1;
#endif
	if (uint128_t_cmp(Rmax_to_max_node_id(127), test_node) != 0)
		FAILF("Rmax_to_max_node_id() failed\n");

	//Lionel: FIXME: commented-out as we get an assertion error for 128 bits (non-supported value)
//	test_node.uint128_a8[0] = 0xff;	/* uint128_t(-1) */
//	if (uint128_t_cmp(Rmax_to_max_node_id(128), test_node) != 0)
//		FAILF("Rmax_to_max_node_id() failed\n");
	printf("%s: tests passed\n", __func__);
}

#ifdef IPV6_SUPPORT
/* Unit test for uint128_t_to_ipv6()
 */
TEST(node_tests, test_uint128_t_to_ipv6) {
	node_id_t test_node;
	const char* expected_result;

	char result[INET6_ADDRSTRLEN+1];
	struct in6_addr dst_in_addr;

#ifndef HAS_INT128
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
#else
	test_node = (uint128_t)0xa5a2150245a887c4 << 64 | (uint128_t)0xe5041afe899c470f;
#endif

	uint128_t_to_ipv6(test_node, &dst_in_addr);
	inet_ntop(AF_INET6, &dst_in_addr, result, INET6_ADDRSTRLEN);
	expected_result="a5a2:1502:45a8:87c4:e504:1afe:899c:470f";
	if (strcmp(result, expected_result) != 0)
		FAILF("uint128_t_to_ipv6() failed, got:\n\"%s\", expected:\n\"%s\"\n", result, expected_result);

#ifndef HAS_INT128
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
#else
	test_node = (uint128_t)0x200141c810000021 << 64 | (uint128_t)0x0000000000210004;
#endif

	uint128_t_to_ipv6(test_node, &dst_in_addr);
	inet_ntop(AF_INET6, &dst_in_addr, result, INET6_ADDRSTRLEN);
	expected_result="2001:41c8:1000:21::21:4";
	if (strcmp(result, expected_result) != 0)
		FAILF("uint128_t_to_ipv6() failed, got:\n\"%s\", expected:\n\"%s\"\n", result, expected_result);

	printf("%s: tests passed\n", __func__);
}
#endif	// IPV6_SUPPORT

/* Unit test for get_tree_prefix_len()
 */
TEST(node_tests, test_get_tree_prefix_len) {
	self_ip_routing_tree_t tree;

#ifdef IPV6_SUPPORT
	tree.Rmax = 4;
	tree.hostA = 0;
	tree.ip_type = IPV6;
	if (get_tree_prefix_len(&tree) != 124)
		FAILF("get_tree_prefix_len() failed\n");

	tree.Rmax = 120;	/* 120 ranks IPv6 tree (maximum size for private addressing) */
	if (get_tree_prefix_len(&tree) != 8)
		FAILF("get_tree_prefix_len() failed\n");

#endif	// IPV6_SUPPORT

#ifdef IPV4_SUPPORT
	tree.Rmax = 6;
	tree.hostA = 2;
	tree.ip_type = IPV4;
	if (get_tree_prefix_len(&tree) != 24)
		FAILF("get_tree_prefix_len() failed\n");

	tree.Rmax = 22;	/* 22 ranks IPv4 tree (maximum size for private addressing) */
	if (get_tree_prefix_len(&tree) != 8)
		FAILF("get_tree_prefix_len() failed\n");
#endif	// IPV4_SUPPORT

	printf("%s: tests passed\n", __func__);
}

/* Unit test for get_hosts_prefix_len()
 */
TEST(node_tests, test_get_hosts_prefix_len) {
	self_ip_routing_tree_t tree;

#ifdef IPV6_SUPPORT
	tree.hostA = 0;
	tree.ip_type = IPV6;
	if (get_hosts_prefix_len(&tree) != 128)
		FAILF("get_hosts_prefix_len() failed\n");

	tree.hostA = 2;
	if (get_hosts_prefix_len(&tree) != 126)
		FAILF("get_hosts_prefix_len() failed\n");

	tree.hostA = 8;
	if (get_hosts_prefix_len(&tree) != 120)
		FAILF("get_hosts_prefix_len() failed\n");
#endif	// IPV6_SUPPORT

#ifdef IPV4_SUPPORT
	tree.hostA = 2;
	tree.ip_type = IPV4;
	if (get_hosts_prefix_len(&tree) != 30)
		FAILF("get_hosts_prefix_len() failed\n");

	tree.hostA = 8;
	if (get_hosts_prefix_len(&tree) != 24)
		FAILF("get_hosts_prefix_len() failed\n");
#endif	// IPV4_SUPPORT

	printf("%s: tests passed\n", __func__);
}

#ifdef IPV6_SUPPORT
/* Unit test for ipv6_prefix_to_uint128_t_mask()
 */
TEST(node_tests, test_ipv6_prefix_to_uint128_t_mask) {
	uint128_t test_netmask;
	char result[33];
	const char* expected_result;

	test_netmask = ipv6_prefix_to_uint128_t_mask(0);
	uint128_t_to_hexstr((uint128_t)test_netmask, 16, result);
	expected_result = "0000000000000000" "0000000000000000";
	if (strcmp(result, expected_result) != 0)
		FAILF("ipv6_prefix_to_uint128_t_mask() failed, got:\n\"%s\", expected:\n\"%s\"\n", result, expected_result);

	test_netmask = ipv6_prefix_to_uint128_t_mask(1);
	uint128_t_to_hexstr((uint128_t)test_netmask, 16, result);
	expected_result = "8000000000000000" "0000000000000000";
	if (strcmp(result, expected_result) != 0)
		FAILF("ipv6_prefix_to_uint128_t_mask() failed, got:\n\"%s\", expected:\n\"%s\"\n", result, expected_result);

	test_netmask = ipv6_prefix_to_uint128_t_mask(64);
	uint128_t_to_hexstr((uint128_t)test_netmask, 16, result);
	expected_result = "ffffffffffffffff" "0000000000000000";
	if (strcmp(result, expected_result) != 0)
		FAILF("ipv6_prefix_to_uint128_t_mask() failed, got:\n\"%s\", expected:\n\"%s\"\n", result, expected_result);

	test_netmask = ipv6_prefix_to_uint128_t_mask(127);
	uint128_t_to_hexstr((uint128_t)test_netmask, 16, result);
	expected_result = "ffffffffffffffff" "fffffffffffffffe";
	if (strcmp(result, expected_result) != 0)
		FAILF("ipv6_prefix_to_uint128_t_mask() failed, got:\n\"%s\", expected:\n\"%s\"\n", result, expected_result);

	test_netmask = ipv6_prefix_to_uint128_t_mask(128);
	uint128_t_to_hexstr((uint128_t)test_netmask, 16, result);
	expected_result = "ffffffffffffffff" "ffffffffffffffff";
	if (strcmp(result, expected_result) != 0)
		FAILF("ipv6_prefix_to_uint128_t_mask() failed, got:\n\"%s\", expected:\n\"%s\"\n", result, expected_result);

	printf("%s: tests passed\n", __func__);
}
#endif	// IPV6_SUPPORT

/* Unit test for get_root_node_id()
 */
TEST(node_tests, test_get_root_node_id) {
	node_id_t test_node;
	char result[33];
	const char* expected_result;
	self_ip_routing_tree_t tree;

#ifdef IPV6_SUPPORT
	tree.Rmax = 4;
	tree.ip_type = IPV6;

	test_node = get_root_node_id(&tree);
	uint128_t_to_hexstr((uint128_t)test_node, 16, result);
	expected_result = "0000000000000000" "0000000000000008";	/* Root node for a tree with Rmax=4 should have ID 8 */
	if (strcmp(result, expected_result) != 0)
		FAILF("get_root_node_id() failed, got:\n\"%s\", expected:\n\"%s\"\n", result, expected_result);
#endif	// IPV6_SUPPORT

#ifdef IPV4_SUPPORT
	tree.Rmax = 6;
	tree.ip_type = IPV4;

	test_node = get_root_node_id(&tree);
	uint128_t_to_hexstr((uint128_t)test_node, 16, result);
	expected_result = "0000000000000000" "0000000000000020";	/* Root node for a tree with Rmax=6 should have ID 32 */
	if (strcmp(result, expected_result) != 0)
		FAILF("get_root_node_id() failed, got:\n\"%s\", expected:\n\"%s\"\n", result, expected_result);
#endif	// IPV4_SUPPORT

	printf("%s: tests passed\n", __func__);
}

/* Unit test for node_id_to_rank()
 */
TEST(node_tests, test_node_id_to_rank) {
	self_ip_routing_tree_t tree;

#ifdef IPV6_SUPPORT
	tree.Rmax = 4;
	tree.ip_type = IPV6;
	tree.hostA = 0;

	if (node_id_to_rank(&tree, (node_id_t)(uint16_t_to_uint128_t(8))) != (rank_t)1)
		FAILF("node_id_to_rank() failed for rank 1\n");

	if (node_id_to_rank(&tree, (node_id_t)(uint16_t_to_uint128_t(4))) != (rank_t)2 ||
	    node_id_to_rank(&tree, (node_id_t)(uint16_t_to_uint128_t(12))) != (rank_t)2)
		FAILF("node_id_to_rank() failed for rank 2\n");

	if (node_id_to_rank(&tree, (node_id_t)(uint16_t_to_uint128_t(2))) != (rank_t)3 ||
	    node_id_to_rank(&tree, (node_id_t)(uint16_t_to_uint128_t(6))) != (rank_t)3 ||
		node_id_to_rank(&tree, (node_id_t)(uint16_t_to_uint128_t(10))) != (rank_t)3 ||
		node_id_to_rank(&tree, (node_id_t)(uint16_t_to_uint128_t(14))) != (rank_t)3)
		FAILF("node_id_to_rank() failed for rank 3\n");

	if (node_id_to_rank(&tree, (node_id_t)(uint16_t_to_uint128_t(1))) != (rank_t)4 ||
	    node_id_to_rank(&tree, (node_id_t)(uint16_t_to_uint128_t(3))) != (rank_t)4 ||
		node_id_to_rank(&tree, (node_id_t)(uint16_t_to_uint128_t(5))) != (rank_t)4 ||
	    node_id_to_rank(&tree, (node_id_t)(uint16_t_to_uint128_t(7))) != (rank_t)4 ||
		node_id_to_rank(&tree, (node_id_t)(uint16_t_to_uint128_t(9))) != (rank_t)4 ||
	    node_id_to_rank(&tree, (node_id_t)(uint16_t_to_uint128_t(11))) != (rank_t)4 ||
		node_id_to_rank(&tree, (node_id_t)(uint16_t_to_uint128_t(13))) != (rank_t)4 ||
		node_id_to_rank(&tree, (node_id_t)(uint16_t_to_uint128_t(15))) != (rank_t)4)
		FAILF("node_id_to_rank() failed for rank 4\n");

	/* Test invalid node IDs */
	if (node_id_to_rank(&tree, (node_id_t)(uint16_t_to_uint128_t(16))) != (rank_t)0)
		FAILF("node_id_to_rank() failed for invalid node ID 16\n");

	if (node_id_to_rank(&tree, (node_id_t)(uint16_t_to_uint128_t(0xffff))) != (rank_t)0)
		FAILF("node_id_to_rank() failed for invalid node ID 0xffff\n");

	if (node_id_to_rank(&tree, (node_id_t)(uint16_t_to_uint128_t(0x8000))) != (rank_t)0)
		FAILF("node_id_to_rank() failed for invalid node ID 0x8000\n");

	if (node_id_to_rank(&tree, (node_id_t)(uint16_t_to_uint128_t(0))) != (rank_t)0)
		FAILF("node_id_to_rank() failed for invalid node ID 0\n");
#endif	// IPV6_SUPPORT

#ifdef IPV4_SUPPORT
	tree.Rmax = 6;
	tree.ip_type = IPV4;
	tree.hostA = 2;

	if (node_id_to_rank(&tree, (node_id_t)(uint16_t_to_uint128_t(32))) != (rank_t)1)
		FAILF("node_id_to_rank() failed for rank 1\n");

	if (node_id_to_rank(&tree, (node_id_t)(uint16_t_to_uint128_t(16))) != (rank_t)2 ||
	    node_id_to_rank(&tree, (node_id_t)(uint16_t_to_uint128_t(48))) != (rank_t)2)
		FAILF("node_id_to_rank() failed for rank 2\n");

	if (node_id_to_rank(&tree, (node_id_t)(uint16_t_to_uint128_t(8))) != (rank_t)3 ||
	    node_id_to_rank(&tree, (node_id_t)(uint16_t_to_uint128_t(24))) != (rank_t)3 ||
		node_id_to_rank(&tree, (node_id_t)(uint16_t_to_uint128_t(40))) != (rank_t)3 ||
		node_id_to_rank(&tree, (node_id_t)(uint16_t_to_uint128_t(56))) != (rank_t)3)
		FAILF("node_id_to_rank() failed for rank 3\n");

	if (node_id_to_rank(&tree, (node_id_t)(uint16_t_to_uint128_t(4))) != (rank_t)4 ||
	    node_id_to_rank(&tree, (node_id_t)(uint16_t_to_uint128_t(12))) != (rank_t)4 ||
		node_id_to_rank(&tree, (node_id_t)(uint16_t_to_uint128_t(20))) != (rank_t)4 ||
	    node_id_to_rank(&tree, (node_id_t)(uint16_t_to_uint128_t(28))) != (rank_t)4 ||
		node_id_to_rank(&tree, (node_id_t)(uint16_t_to_uint128_t(36))) != (rank_t)4 ||
	    node_id_to_rank(&tree, (node_id_t)(uint16_t_to_uint128_t(44))) != (rank_t)4 ||
		node_id_to_rank(&tree, (node_id_t)(uint16_t_to_uint128_t(52))) != (rank_t)4 ||
		node_id_to_rank(&tree, (node_id_t)(uint16_t_to_uint128_t(60))) != (rank_t)4)
		FAILF("node_id_to_rank() failed for rank 4\n");

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
		node_id_to_rank(&tree, (node_id_t)(uint16_t_to_uint128_t(62))) != (rank_t)5)
		FAILF("node_id_to_rank() failed for rank 5\n");

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
		node_id_to_rank(&tree, (node_id_t)(uint16_t_to_uint128_t(63))) != (rank_t)6)
		FAILF("node_id_to_rank() failed for rank 6\n");

	/* Test invalid node IDs */
	if (node_id_to_rank(&tree, (node_id_t)(uint16_t_to_uint128_t(64))) != (rank_t)0)
		FAILF("node_id_to_rank() failed for invalid node ID 64\n");

	if (node_id_to_rank(&tree, (node_id_t)(uint16_t_to_uint128_t(0xffff))) != (rank_t)0)
		FAILF("node_id_to_rank() failed for invalid node ID 0xffff\n");

	if (node_id_to_rank(&tree, (node_id_t)(uint16_t_to_uint128_t(0x8000))) != (rank_t)0)
		FAILF("node_id_to_rank() failed for invalid node ID 0x8000\n");

	if (node_id_to_rank(&tree, (node_id_t)(uint16_t_to_uint128_t(0))) != (rank_t)0)
		FAILF("node_id_to_rank() failed for invalid node ID 0\n");
#endif	// IPV4_SUPPORT

	printf("%s: tests passed\n", __func__);
}

/* Unit test for get_parent_node_id()
 */
TEST(node_tests, test_get_parent_node_id) {
	node_id_t test_node;
	self_ip_routing_tree_t tree;

#ifdef IPV6_SUPPORT
	tree.Rmax = 4;
	tree.ip_type = IPV6;
	tree.hostA = 0;

	/* Start from root's children */
	test_node = get_root_node_id(&tree);
	test_node = get_parent_node_id(&tree, test_node);	/* No parent for root */
	if (!U128_IS_ZERO(test_node))
		FAILF("get_parent_node_id() failed, no parent should be provided for root node\n");
	
	/* Get the root's left child's parent, should get root node ID again */
	test_node = get_left_child_node_id(&tree, get_root_node_id(&tree));
	check_result_is_uint128(get_parent_node_id(&tree, test_node), get_root_node_id(&tree), __LINE__);
	/* Get the root's right child's parent, should get root node ID again */
	test_node = get_right_child_node_id(&tree, get_root_node_id(&tree));
	check_result_is_uint128(get_parent_node_id(&tree, test_node), get_root_node_id(&tree), __LINE__);
	
	/* Parent for node 2 should have ID 4 */
	check_result_is_uint128(get_parent_node_id(&tree, uint16_t_to_uint128_t(2)), uint16_t_to_uint128_t(4), __LINE__);
	/* Parent for node 6 should have ID 4 */
	check_result_is_uint128(get_parent_node_id(&tree, uint16_t_to_uint128_t(6)), uint16_t_to_uint128_t(4), __LINE__);

	/* Parent for node 10 should have ID 12 */
	check_result_is_uint128(get_parent_node_id(&tree, uint16_t_to_uint128_t(10)), uint16_t_to_uint128_t(12), __LINE__);
	/* Parent for node 14 should have ID 12 */
	check_result_is_uint128(get_parent_node_id(&tree, uint16_t_to_uint128_t(14)), uint16_t_to_uint128_t(12), __LINE__);
	
	/* Parent for node 1 should have ID 2 */
	check_result_is_uint128(get_parent_node_id(&tree, uint16_t_to_uint128_t(1)), uint16_t_to_uint128_t(2), __LINE__);
	/* Parent for node 3 should have ID 2 */
	check_result_is_uint128(get_parent_node_id(&tree, uint16_t_to_uint128_t(3)), uint16_t_to_uint128_t(2), __LINE__);
	
	/* Parent for node 5 should have ID 6 */
	check_result_is_uint128(get_parent_node_id(&tree, uint16_t_to_uint128_t(5)), uint16_t_to_uint128_t(6), __LINE__);
	/* Parent for node 7 should have ID 6 */
	check_result_is_uint128(get_parent_node_id(&tree, uint16_t_to_uint128_t(7)), uint16_t_to_uint128_t(6), __LINE__);
	
	/* Parent for node 9 should have ID 10 */
	check_result_is_uint128(get_parent_node_id(&tree, uint16_t_to_uint128_t(9)), uint16_t_to_uint128_t(10), __LINE__);
	/* Parent for node 11 should have ID 10 */
	check_result_is_uint128(get_parent_node_id(&tree, uint16_t_to_uint128_t(11)), uint16_t_to_uint128_t(10), __LINE__);
	
	/* Parent for node 13 should have ID 14 */
	check_result_is_uint128(get_parent_node_id(&tree, uint16_t_to_uint128_t(13)), uint16_t_to_uint128_t(14), __LINE__);
	/* Parent for node 15 should have ID 14 */
	check_result_is_uint128(get_parent_node_id(&tree, uint16_t_to_uint128_t(15)), uint16_t_to_uint128_t(14), __LINE__);

#endif	// IPV6_SUPPORT

#ifdef IPV4_SUPPORT
	tree.Rmax = 6;
	tree.ip_type = IPV4;
	tree.hostA = 2;

	/* Start from root's children */
	test_node = get_root_node_id(&tree);
	test_node = get_parent_node_id(&tree, test_node);	/* No parent for root */
	if (!U128_IS_ZERO(test_node))
		FAILF("get_parent_node_id() failed, no parent should be provided for root node\n");

	/* Get the root's left child's parent, should get root node ID again */
	test_node = get_left_child_node_id(&tree, get_root_node_id(&tree));
	check_result_is_uint128(get_parent_node_id(&tree, test_node), get_root_node_id(&tree), __LINE__);
	/* Get the root's right child's parent, should get root node ID again */
	test_node = get_right_child_node_id(&tree, get_root_node_id(&tree));
	check_result_is_uint128(get_parent_node_id(&tree, test_node), get_root_node_id(&tree), __LINE__);
	
	/* Parent for node 16 should have ID 32 */
	check_result_is_uint128(get_parent_node_id(&tree, uint16_t_to_uint128_t(16)), uint16_t_to_uint128_t(32), __LINE__);
	/* Parent for node 48 should have ID 32 */
	check_result_is_uint128(get_parent_node_id(&tree, uint16_t_to_uint128_t(48)), uint16_t_to_uint128_t(32), __LINE__);
	
	/* Parent for node 8 should have ID 16 */
	check_result_is_uint128(get_parent_node_id(&tree, uint16_t_to_uint128_t(8)), uint16_t_to_uint128_t(16), __LINE__);
	/* Parent for node 24 should have ID 16 */
	check_result_is_uint128(get_parent_node_id(&tree, uint16_t_to_uint128_t(24)), uint16_t_to_uint128_t(16), __LINE__);
	
	/* Parent for node 40 should have ID 48 */
	check_result_is_uint128(get_parent_node_id(&tree, uint16_t_to_uint128_t(40)), uint16_t_to_uint128_t(48), __LINE__);
	/* Parent for node 56 should have ID 48 */
	check_result_is_uint128(get_parent_node_id(&tree, uint16_t_to_uint128_t(56)), uint16_t_to_uint128_t(48), __LINE__);
	
	/* Parent for node 16 should have ID 32 */
	check_result_is_uint128(get_parent_node_id(&tree, uint16_t_to_uint128_t(16)), uint16_t_to_uint128_t(32), __LINE__);
	/* Parent for node 48 should have ID 32 */
	check_result_is_uint128(get_parent_node_id(&tree, uint16_t_to_uint128_t(48)), uint16_t_to_uint128_t(32), __LINE__);
	
	/* Parent for node 20 should have ID 24 */
	check_result_is_uint128(get_parent_node_id(&tree, uint16_t_to_uint128_t(20)), uint16_t_to_uint128_t(24), __LINE__);
	/* Parent for node 28 should have ID 24 */
	check_result_is_uint128(get_parent_node_id(&tree, uint16_t_to_uint128_t(28)), uint16_t_to_uint128_t(24), __LINE__);
	
	/* Parent for node 36 should have ID 40 */
	check_result_is_uint128(get_parent_node_id(&tree, uint16_t_to_uint128_t(36)), uint16_t_to_uint128_t(40), __LINE__);
	/* Parent for node 44 should have ID 40 */
	check_result_is_uint128(get_parent_node_id(&tree, uint16_t_to_uint128_t(44)), uint16_t_to_uint128_t(40), __LINE__);
	
	/* Parent for node 52 should have ID 56 */
	check_result_is_uint128(get_parent_node_id(&tree, uint16_t_to_uint128_t(52)), uint16_t_to_uint128_t(56), __LINE__);
	/* Parent for node 60 should have ID 56 */
	check_result_is_uint128(get_parent_node_id(&tree, uint16_t_to_uint128_t(60)), uint16_t_to_uint128_t(56), __LINE__);
	
	// We don't continue checking for the bottom of the tree... only for extremes
	/* Parent for node 1 should have ID 2 */
	check_result_is_uint128(get_parent_node_id(&tree, uint16_t_to_uint128_t(1)), uint16_t_to_uint128_t(2), __LINE__);
	/* Parent for node 63 should have ID 62 */
	check_result_is_uint128(get_parent_node_id(&tree, uint16_t_to_uint128_t(63)), uint16_t_to_uint128_t(62), __LINE__);
#endif // IPV4_SUPPORT

	printf("%s: tests passed\n", __func__);
}

/* Unit test for get_left_child_node_id()
 */
TEST(node_tests, test_get_left_child_node_id) {
	node_id_t test_node;
	self_ip_routing_tree_t tree;

#ifdef IPV6_SUPPORT
	tree.Rmax = 4;
	tree.ip_type = IPV6;

	/* Start from root, and descend on left child at each rank */
	test_node = get_root_node_id(&tree);
	/* Left child for root node 8 should have ID 4 */
	check_result_is_uint128(test_node = get_left_child_node_id(&tree, test_node), uint16_t_to_uint128_t(4), __LINE__);
	/* Left child for node 4 should have ID 2 */
	check_result_is_uint128(test_node = get_left_child_node_id(&tree, test_node), uint16_t_to_uint128_t(2), __LINE__);
	/* Left child for node 2 should have ID 1 */
	check_result_is_uint128(test_node = get_left_child_node_id(&tree, test_node), uint16_t_to_uint128_t(1), __LINE__);
	/* Left child for node 1 is invalid */
	check_result_is_uint128(test_node = get_left_child_node_id(&tree, test_node), uint128_t_zero(), __LINE__);
	
	/* Start from node 12, and descend on left child at each rank */
	test_node = (node_id_t)uint16_t_to_uint128_t(12);
	/* Left child for node 12 should have ID 10 */
	check_result_is_uint128(test_node = get_left_child_node_id(&tree, test_node), uint16_t_to_uint128_t(10), __LINE__);
	/* Left child for node 10 should have ID 9 */
	check_result_is_uint128(test_node = get_left_child_node_id(&tree, test_node), uint16_t_to_uint128_t(9), __LINE__);
	/* Left child for node 9 is invalid */
	check_result_is_uint128(test_node = get_left_child_node_id(&tree, test_node), uint128_t_zero(), __LINE__);

	/* Node 0xffff is invalid */
	test_node = (node_id_t)uint16_t_to_uint128_t(0xffff);
	check_result_is_uint128(test_node = get_left_child_node_id(&tree, test_node), uint128_t_zero(), __LINE__);
	/* Node (uint128_t)-1 is invalid */
	U128_SET_MAX(test_node);
	check_result_is_uint128(test_node = get_left_child_node_id(&tree, test_node), uint128_t_zero(), __LINE__);
	/* Node 0 is invalid */
	U128_SET_ZERO(test_node);
	check_result_is_uint128(test_node = get_left_child_node_id(&tree, test_node), uint128_t_zero(), __LINE__);
#endif	// IPV6_SUPPORT

#ifdef IPV4_SUPPORT
	tree.Rmax = 6;
	tree.ip_type = IPV4;

	/* Start from root, and descend on left child at each rank */
	test_node = get_root_node_id(&tree);
	/* Left child for root node 32 should have ID 16 */
	check_result_is_uint128(test_node = get_left_child_node_id(&tree, test_node), uint16_t_to_uint128_t(16), __LINE__);
	/* Left child for node 16 should have ID 8 */
	check_result_is_uint128(test_node = get_left_child_node_id(&tree, test_node), uint16_t_to_uint128_t(8), __LINE__);
	/* Left child for node 8 should have ID 4 */
	check_result_is_uint128(test_node = get_left_child_node_id(&tree, test_node), uint16_t_to_uint128_t(4), __LINE__);
	/* Left child for node 4 should have ID 2 */
	check_result_is_uint128(test_node = get_left_child_node_id(&tree, test_node), uint16_t_to_uint128_t(2), __LINE__);
	/* Left child for node 2 should have ID 1 */
	check_result_is_uint128(test_node = get_left_child_node_id(&tree, test_node), uint16_t_to_uint128_t(1), __LINE__);
	/* Left child for node 1 is invalid */
	check_result_is_uint128(test_node = get_left_child_node_id(&tree, test_node), uint128_t_zero(), __LINE__);
	
	/* Start from node 48, and descend on left child at each rank */
	test_node = (node_id_t)uint16_t_to_uint128_t(48);
	/* Left child for node 48 should have ID 40 */
	check_result_is_uint128(test_node = get_left_child_node_id(&tree, test_node), uint16_t_to_uint128_t(40), __LINE__);
	/* Left child for node 40 should have ID 36 */
	check_result_is_uint128(test_node = get_left_child_node_id(&tree, test_node), uint16_t_to_uint128_t(36), __LINE__);
	/* Left child for node 36 should have ID 34 */
	check_result_is_uint128(test_node = get_left_child_node_id(&tree, test_node), uint16_t_to_uint128_t(34), __LINE__);
	/* Left child for node 34 should have ID 33 */
	check_result_is_uint128(test_node = get_left_child_node_id(&tree, test_node), uint16_t_to_uint128_t(33), __LINE__);
	/* Left child for node 33 is invalid */
	check_result_is_uint128(test_node = get_left_child_node_id(&tree, test_node), uint128_t_zero(), __LINE__);
	
	/* Node 0xffff is invalid */
	test_node = (node_id_t)uint16_t_to_uint128_t(0xffff);
	check_result_is_uint128(test_node = get_left_child_node_id(&tree, test_node), uint128_t_zero(), __LINE__);
	/* Node (uint128_t)-1 is invalid */
	U128_SET_MAX(test_node);
	check_result_is_uint128(test_node = get_left_child_node_id(&tree, test_node), uint128_t_zero(), __LINE__);
	/* Node 0 is invalid */
	U128_SET_ZERO(test_node);
	check_result_is_uint128(test_node = get_left_child_node_id(&tree, test_node), uint128_t_zero(), __LINE__);
#endif	// IPV4_SUPPORT

	printf("%s: tests passed\n", __func__);
}

/* Unit test for get_right_child_node_id()
 */
TEST(node_tests, test_get_right_child_node_id) {
	node_id_t test_node;
	self_ip_routing_tree_t tree;

#ifdef IPV6_SUPPORT
	tree.Rmax = 4;
	tree.ip_type = IPV6;

	/* Start from root, and descend on right child at each rank */
	test_node = get_root_node_id(&tree);
	/* Right child for root node 8 should have ID 12 */
	check_result_is_uint128(test_node = get_right_child_node_id(&tree, test_node), uint16_t_to_uint128_t(12), __LINE__);
	/* Right child for node 12 should have ID 14 */
	check_result_is_uint128(test_node = get_right_child_node_id(&tree, test_node), uint16_t_to_uint128_t(14), __LINE__);
	/* Right child for node 14 should have ID 15 */
	check_result_is_uint128(test_node = get_right_child_node_id(&tree, test_node), uint16_t_to_uint128_t(15), __LINE__);
	/* Right child for node 15 is invalid */
	check_result_is_uint128(test_node = get_right_child_node_id(&tree, test_node), uint128_t_zero(), __LINE__);
	
	/* Start from node 4, and descend on right child at each rank */
	test_node = (node_id_t)uint16_t_to_uint128_t(4);
	/* Right child for node 4 should have ID 6 */
	check_result_is_uint128(test_node = get_right_child_node_id(&tree, test_node), uint16_t_to_uint128_t(6), __LINE__);
	/* Right child for node 6 should have ID 7 */
	check_result_is_uint128(test_node = get_right_child_node_id(&tree, test_node), uint16_t_to_uint128_t(7), __LINE__);
	/* Right child for node 7 is invalid */
	check_result_is_uint128(test_node = get_right_child_node_id(&tree, test_node), uint128_t_zero(), __LINE__);

	/* Node 0xffff is invalid */
	test_node = (node_id_t)uint16_t_to_uint128_t(0xffff);
	check_result_is_uint128(test_node = get_right_child_node_id(&tree, test_node), uint128_t_zero(), __LINE__);
	/* Node (uint128_t)-1 is invalid */
	U128_SET_MAX(test_node);
	check_result_is_uint128(test_node = get_right_child_node_id(&tree, test_node), uint128_t_zero(), __LINE__);
	/* Node 0 is invalid */
	U128_SET_ZERO(test_node);
	check_result_is_uint128(test_node = get_right_child_node_id(&tree, test_node), uint128_t_zero(), __LINE__);
#endif	// IPV6_SUPPORT

#ifdef IPV4_SUPPORT
	tree.Rmax = 6;
	tree.ip_type = IPV4;

	/* Start from root, and descend on right child at each rank */
	test_node = get_root_node_id(&tree);
	/* Right child for root node 32 should have ID 48 */
	check_result_is_uint128(test_node = get_right_child_node_id(&tree, test_node), uint16_t_to_uint128_t(48), __LINE__);
	/* Right child for node 48 should have ID 56 */
	check_result_is_uint128(test_node = get_right_child_node_id(&tree, test_node), uint16_t_to_uint128_t(56), __LINE__);
	/* Right child for node 56 should have ID 60 */
	check_result_is_uint128(test_node = get_right_child_node_id(&tree, test_node), uint16_t_to_uint128_t(60), __LINE__);
	/* Right child for node 60 should have ID 62 */
	check_result_is_uint128(test_node = get_right_child_node_id(&tree, test_node), uint16_t_to_uint128_t(62), __LINE__);
	/* Right child for node 62 should have ID 63 */
	check_result_is_uint128(test_node = get_right_child_node_id(&tree, test_node), uint16_t_to_uint128_t(63), __LINE__);
	/* Right child for node 63 is invalid */
	check_result_is_uint128(test_node = get_right_child_node_id(&tree, test_node), uint128_t_zero(), __LINE__);
	
	/* Start from node 16, and descend on right child at each rank */
	test_node = (node_id_t)uint16_t_to_uint128_t(16);
	/* Right child for node 16 should have ID 24 */
	check_result_is_uint128(test_node = get_right_child_node_id(&tree, test_node), uint16_t_to_uint128_t(24), __LINE__);
	/* Right child for node 24 should have ID 28 */
	check_result_is_uint128(test_node = get_right_child_node_id(&tree, test_node), uint16_t_to_uint128_t(28), __LINE__);
	/* Right child for node 28 should have ID 30 */
	check_result_is_uint128(test_node = get_right_child_node_id(&tree, test_node), uint16_t_to_uint128_t(30), __LINE__);
	/* Right child for node 30 should have ID 31 */
	check_result_is_uint128(test_node = get_right_child_node_id(&tree, test_node), uint16_t_to_uint128_t(31), __LINE__);
	/* Right child for node 31 is invalid */
	check_result_is_uint128(test_node = get_right_child_node_id(&tree, test_node), uint128_t_zero(), __LINE__);
	
	/* Node 0xffff is invalid */
	test_node = (node_id_t)uint16_t_to_uint128_t(0xffff);
	check_result_is_uint128(test_node = get_right_child_node_id(&tree, test_node), uint128_t_zero(), __LINE__);
	/* Node (uint128_t)-1 is invalid */
	U128_SET_MAX(test_node);
	check_result_is_uint128(test_node = get_right_child_node_id(&tree, test_node), uint128_t_zero(), __LINE__);
	/* Node 0 is invalid */
	U128_SET_ZERO(test_node);
	check_result_is_uint128(test_node = get_right_child_node_id(&tree, test_node), uint128_t_zero(), __LINE__);
#endif	// IPV4_SUPPORT
	
	printf("%s: tests passed\n", __func__);
}

/* Unit test for get_top_interface_config()
 */
TEST(node_tests, test_get_top_interface_config) {
	node_id_t test_node;
	self_ip_routing_tree_t tree;
	if_ip_addr_t ip_addr_result;
	char ip_addr_str[INET6_ADDRSTRLEN+1];
	prefix_t current_hostA;

#ifdef IPV6_SUPPORT
	tree.ip_type = IPV6;
	tree.Rmax = 4;
#ifndef HAS_INT128
	U128_SET_ZERO(tree.prefix);
	tree.prefix.uint128_a8[0] = 0xfd;	/* Prefix is fd00::/124 */
#else
	tree.prefix = (uint128_t)0xfd << 120;	/* Prefix is fd00::/124 */
#endif
	
	tree.hostA = current_hostA = 0; /* First perform test without any local network attached to nodes (hostA=0) */
	
	test_node = get_root_node_id(&tree);	/* Will get 8 */
	ip_addr_result = get_top_interface_config(&tree, test_node);
	inet_ntop(AF_INET6, &(ip_addr_result.in_addr.__ipv6_in6_addr), ip_addr_str, INET6_ADDRSTRLEN);
	if (tree.ip_type != IPV6 || tree.Rmax != 4 || tree.hostA != 0)
		FAILF("get_top_interface_config() modified the input tree argument\n");
	if (ip_addr_result.ip_type != IPV6)	/* ip_type should have been propagated as is to result */
		FAILF("get_top_interface_config() modified ip_type field\n");
	if (strcmp(ip_addr_str, "fd00::8") != 0)	/* This is the expected root node's top interface */
		FAILF("get_top_interface_config() got wrong IP address: %s\n", ip_addr_str);
	if (ip_addr_result.prefix != 128)	/* Should get /128 for root node's top interface netmask */
		FAILF("get_top_interface_config() got wrong netmask: %d\n", ip_addr_result.prefix);

	test_node = get_left_child_node_id(&tree, get_root_node_id(&tree));	/* Will get 4 */
	ip_addr_result = get_top_interface_config(&tree, test_node);
	inet_ntop(AF_INET6, &(ip_addr_result.in_addr.__ipv6_in6_addr), ip_addr_str, INET6_ADDRSTRLEN);
	if (ip_addr_result.ip_type != IPV6)	/* ip_type should have been propagated as is to result */
		FAILF("get_top_interface_config() modified ip_type field\n");
	if (strcmp(ip_addr_str, "fd00::4") != 0)	/* This is the expected root left child node's top interface */
		FAILF("get_top_interface_config() got wrong IP address: %s\n", ip_addr_str);
	if (ip_addr_result.prefix != 128)	/* Should get /128 for root node's top interface netmask */
		FAILF("get_top_interface_config() got wrong netmask: %d\n", ip_addr_result.prefix);
	
	test_node = get_right_child_node_id(&tree, get_root_node_id(&tree));	/* Will get 12 */
	ip_addr_result = get_top_interface_config(&tree, test_node);
	inet_ntop(AF_INET6, &(ip_addr_result.in_addr.__ipv6_in6_addr), ip_addr_str, INET6_ADDRSTRLEN);
	if (ip_addr_result.ip_type != IPV6)	/* ip_type should have been propagated as is to result */
		FAILF("get_top_interface_config() modified ip_type field\n");
	if (strcmp(ip_addr_str, "fd00::c") != 0)	/* This is the expected root right child node's top interface */
		FAILF("get_top_interface_config() got wrong IP address: %s\n", ip_addr_str);
	if (ip_addr_result.prefix != 128)	/* Should get /128 for root node's top interface netmask */
		FAILF("get_top_interface_config() got wrong netmask: %d\n", ip_addr_result.prefix);

	test_node = uint8_t_to_uint128_t(1);	/* Take left-most leaf of tree (ID 1) */
	ip_addr_result = get_top_interface_config(&tree, test_node);
	inet_ntop(AF_INET6, &(ip_addr_result.in_addr.__ipv6_in6_addr), ip_addr_str, INET6_ADDRSTRLEN);
	if (ip_addr_result.ip_type != IPV6)	/* ip_type should have been propagated as is to result */
		FAILF("get_top_interface_config() modified ip_type field\n");
	if (strcmp(ip_addr_str, "fd00::1") != 0)	/* This is the expected tree's left-most leaf's top interface */
		FAILF("get_top_interface_config() got wrong IP address: %s\n", ip_addr_str);
	if (ip_addr_result.prefix != 128)	/* Should get /128 for root node's top interface netmask */
		FAILF("get_top_interface_config() got wrong netmask: %d\n", ip_addr_result.prefix);

	test_node = uint8_t_to_uint128_t(15);	/* Take right-most leaf of tree (ID 15) */
	ip_addr_result = get_top_interface_config(&tree, test_node);
	inet_ntop(AF_INET6, &(ip_addr_result.in_addr.__ipv6_in6_addr), ip_addr_str, INET6_ADDRSTRLEN);
	if (ip_addr_result.ip_type != IPV6)	/* ip_type should have been propagated as is to result */
		FAILF("get_top_interface_config() modified ip_type field\n");
	if (strcmp(ip_addr_str, "fd00::f") != 0)	/* This is the expected tree's right-most leaf's top interface */
		FAILF("get_top_interface_config() got wrong IP address: %s\n", ip_addr_str);
	if (ip_addr_result.prefix != 128)	/* Should get /128 for root node's top interface netmask */
		FAILF("get_top_interface_config() got wrong netmask: %d\n", ip_addr_result.prefix);

	/* Now, test with routable local networks (hostA!=0) */
	for (current_hostA = 1; current_hostA < 128; current_hostA++) {	/* Perform the check for all hostA values between 0 and 127... we should get the same result */
		tree.hostA = current_hostA;

		test_node = get_root_node_id(&tree);	/* Will get 8 */
		ip_addr_result = get_top_interface_config(&tree, test_node);
		if (tree.ip_type != IPV6 || tree.Rmax != 4 || tree.hostA != current_hostA)
			FAILF("get_top_interface_config() modified the input tree argument\n");
		if (ip_addr_result.ip_type != NONE)	/* on IPv6 trees with hostA=0, there is no bottom route */
			FAILF("get_top_interface_config() should return no configuration\n");
	
		test_node = get_left_child_node_id(&tree, get_root_node_id(&tree));	/* Will get 4 */
		ip_addr_result = get_top_interface_config(&tree, test_node);
		if (ip_addr_result.ip_type != NONE)	/* on IPv6 trees with hostA=0, there is no bottom route */
			FAILF("get_top_interface_config() should return no configuration\n");
	
		test_node = get_right_child_node_id(&tree, get_root_node_id(&tree));	/* Will get 12 */
		ip_addr_result = get_top_interface_config(&tree, test_node);
		if (ip_addr_result.ip_type != NONE)	/* on IPv6 trees with hostA=0, there is no bottom route */
			FAILF("get_top_interface_config() should return no configuration\n");
	
		test_node = uint8_t_to_uint128_t(1);	/* Take left-most leaf of tree (ID 1) */
		ip_addr_result = get_top_interface_config(&tree, test_node);
		if (ip_addr_result.ip_type != NONE)	/* on IPv6 trees with hostA=0, there is no bottom route */
			FAILF("get_top_interface_config() should return no configuration\n");
	
		test_node = uint8_t_to_uint128_t(15);	/* Take right-most leaf of tree (ID 15) */
		ip_addr_result = get_top_interface_config(&tree, test_node);
		if (ip_addr_result.ip_type != NONE)	/* on IPv6 trees with hostA=0, there is no bottom route */
			FAILF("get_top_interface_config() should return no configuration\n");
	}
#endif	// IPV6_SUPPORT

#ifdef IPV4_SUPPORT
	tree.ip_type = IPV4;
	tree.Rmax = 6;
	tree.hostA = 2;
#ifndef HAS_INT128
	U128_SET_ZERO(tree.prefix);
	tree.prefix.uint128_a8[12] = 192;
	tree.prefix.uint128_a8[13] = 168;
	tree.prefix.uint128_a8[14] = 0;
	tree.prefix.uint128_a8[15] = 0;	/* Prefix is 192.168.0.0/24 */
#else
	tree.prefix = (uint128_t)192<<24 | (uint128_t)168<<16 | (uint128_t)0<<8 | (uint128_t)0;
#endif

#warning Test for IPv4 is not implemented yet
	//ip_addr_result = get_top_interface_config(&tree, test_node);
	//FIXME: Add unit test for get_top_interface_config() on IPv4 trees

	//if (ip_addr_result.ip_type != IPV4)	/* ip_type should have been propagated as is to result */
	//	FAILF("get_top_interface_config() modified ip_type field\n");
#endif	// IPV4_SUPPORT

	printf("%s: tests passed\n", __func__);
}

/* Unit test for get_left_interface_config()
 */
TEST(node_tests, test_get_left_interface_config) {
	node_id_t test_node;
	self_ip_routing_tree_t tree;
	if_ip_addr_t ip_addr_result;
	char ip_addr_str[INET6_ADDRSTRLEN+1];
	prefix_t current_hostA;

#ifdef IPV6_SUPPORT
	tree.ip_type = IPV6;
	tree.Rmax = 4;
#ifndef HAS_INT128
	U128_SET_ZERO(tree.prefix);
	tree.prefix.uint128_a8[0] = 0xfd;	/* Prefix is fd00::/124 */
#else
	tree.prefix = (uint128_t)0xfd << 120;   /* Prefix is fd00::/124 */
#endif

	for (current_hostA = 0; current_hostA < 128; current_hostA++) {	/* Perform the check for all hostA values between 0 and 127... we should get the same result */
		tree.hostA = current_hostA;
		test_node = get_root_node_id(&tree);	/* Will get 8 */
		ip_addr_result = get_left_interface_config(&tree, test_node);
		inet_ntop(AF_INET6, &(ip_addr_result.in_addr.__ipv6_in6_addr), ip_addr_str, INET6_ADDRSTRLEN);
		if (tree.ip_type != IPV6 || tree.Rmax != 4 || tree.hostA != current_hostA)
			FAILF("get_left_interface_config() modified the input tree argument\n");
		if (ip_addr_result.ip_type != NONE)	/* IP type should be set to none, IPv6 trees have no IPv6 addresses on interfaces to children */
			FAILF("get_left_interface_config() should return no config\n");
	}
#endif	// IPV6_SUPPORT

#ifdef IPV4_SUPPORT
	tree.ip_type = IPV4;
	tree.Rmax = 6;
	tree.hostA = 2;
#ifndef HAS_INT128
	U128_SET_ZERO(tree.prefix);
	tree.prefix.uint128_a8[12] = 192;
	tree.prefix.uint128_a8[13] = 168;
	tree.prefix.uint128_a8[14] = 0;
	tree.prefix.uint128_a8[15] = 0;	/* Prefix is 192.168.0.0/24 */
#else
	tree.prefix = (uint128_t)192<<24 | (uint128_t)168<<16 | (uint128_t)0<<8 | (uint128_t)0;
#endif

#warning Test for IPv4 is not implemented yet
	//ip_addr_result = get_left_interface_config(&tree, test_node);
	//FIXME: Add unit test for get_left_interface_config() on IPv4 trees

	//if (ip_addr_result.ip_type != IPV4)	/* ip_type should have been propagated as is to result */
	//	FAILF("get_left_interface_config() modified ip_type field\n");
#endif	// IPV4_SUPPORT

	printf("%s: tests passed\n", __func__);
}

/* Unit test for get_right_interface_config()
 */
TEST(node_tests, test_get_right_interface_config) {
	node_id_t test_node;
	self_ip_routing_tree_t tree;
	if_ip_addr_t ip_addr_result;
	char ip_addr_str[INET6_ADDRSTRLEN+1];
	prefix_t current_hostA;

#ifdef IPV6_SUPPORT
	tree.ip_type = IPV6;
	tree.Rmax = 4;
#ifndef HAS_INT128
	U128_SET_ZERO(tree.prefix);
	tree.prefix.uint128_a8[0] = 0xfd;	/* Prefix is fd00::/124 */
#else
	tree.prefix = (uint128_t)0xfd << 120;   /* Prefix is fd00::/124 */
#endif

	for (current_hostA = 0; current_hostA < 128; current_hostA++) {	/* Perform the check for all hostA values between 0 and 127... we should get the same result */
		tree.hostA = current_hostA;
		test_node = get_root_node_id(&tree);	/* Will get 8 */
		ip_addr_result = get_left_interface_config(&tree, test_node);
		inet_ntop(AF_INET6, &(ip_addr_result.in_addr.__ipv6_in6_addr), ip_addr_str, INET6_ADDRSTRLEN);
		if (tree.ip_type != IPV6 || tree.Rmax != 4 || tree.hostA != current_hostA)
			FAILF("get_right_interface_config() modified the input tree argument\n");
		if (ip_addr_result.ip_type != NONE)	/* IP type should be set to none, IPv6 trees have no IPv6 addresses on interfaces to children */
			FAILF("get_right_interface_config() should return no config\n");
	}
#endif	// IPV6_SUPPORT

#ifdef IPV4_SUPPORT
	tree.ip_type = IPV4;
	tree.Rmax = 6;
	tree.hostA = 2;
#ifndef HAS_INT128
	U128_SET_ZERO(tree.prefix);
	tree.prefix.uint128_a8[12] = 192;
	tree.prefix.uint128_a8[13] = 168;
	tree.prefix.uint128_a8[14] = 0;
	tree.prefix.uint128_a8[15] = 0;	/* Prefix is 192.168.0.0/24 */
#else
	tree.prefix = (uint128_t)192<<24 | (uint128_t)168<<16 | (uint128_t)0<<8 | (uint128_t)0;
#endif

#warning Test for IPv4 is not implemented yet
	//ip_addr_result = get_right_interface_config(&tree, test_node);
	//FIXME: Add unit test for get_right_interface_config() on IPv4 trees

	//if (ip_addr_result.ip_type != IPV4)	/* ip_type should have been propagated as is to result */
	//	FAILF("get_right_interface_config() modified ip_type field\n");

#endif	// IPV4_SUPPORT

	printf("%s: tests passed\n", __func__);
}

/* Unit test for get_bottom_interface_config()
 */
TEST(node_tests, test_get_bottom_interface_config) {
	node_id_t test_node;
	self_ip_routing_tree_t tree;
	if_ip_addr_t ip_addr_result;
	char ip_addr_str[INET6_ADDRSTRLEN+1];

#ifdef IPV6_SUPPORT
	tree.ip_type = IPV6;
	tree.Rmax = 4;
	/* First perform test without any local network attached to nodes (hostA=0) */
#ifndef HAS_INT128
	U128_SET_ZERO(tree.prefix);
	tree.prefix.uint128_a8[0] = 0xfd;	/* Prefix is fd00::/124 */
#else
	tree.prefix = (uint128_t)0xfd << 120;   /* Prefix is fd00::/124 */
#endif

	tree.hostA = 0;	/* First perform test without any local network attached to nodes (hostA=0) */
	
	test_node = get_root_node_id(&tree);	/* Will get 8 */
	ip_addr_result = get_bottom_interface_config(&tree, test_node);
	inet_ntop(AF_INET6, &(ip_addr_result.in_addr.__ipv6_in6_addr), ip_addr_str, INET6_ADDRSTRLEN);
	if (tree.ip_type != IPV6 || tree.Rmax != 4 || tree.hostA != 0)
		FAILF("get_bottom_interface_config() modified the input tree argument\n");
	if (ip_addr_result.ip_type != NONE)	/* IP type should be set to none, IPv6 trees with hostA=0 have no bottom interface */
		FAILF("get_bottom_interface_config() should return no config\n");

	test_node = get_left_child_node_id(&tree, get_root_node_id(&tree));	/* Will get 4 */
	ip_addr_result = get_bottom_interface_config(&tree, test_node);
	inet_ntop(AF_INET6, &(ip_addr_result.in_addr.__ipv6_in6_addr), ip_addr_str, INET6_ADDRSTRLEN);
	if (ip_addr_result.ip_type != NONE)	/* IP type should be set to none, IPv6 trees with hostA=0 have no bottom interface */
		FAILF("get_bottom_interface_config() should return no config\n");

	test_node = get_right_child_node_id(&tree, get_root_node_id(&tree));	/* Will get 12 */
	ip_addr_result = get_bottom_interface_config(&tree, test_node);
	if (ip_addr_result.ip_type != NONE)	/* IP type should be set to none, IPv6 trees with hostA=0 have no bottom interface */
		FAILF("get_bottom_interface_config() should return no config\n");

	test_node = uint8_t_to_uint128_t(1);	/* Take left-most leaf of tree (ID 1) */
	ip_addr_result = get_bottom_interface_config(&tree, test_node);
	if (ip_addr_result.ip_type != NONE)	/* IP type should be set to none, IPv6 trees with hostA=0 have no bottom interface */
		FAILF("get_bottom_interface_config() should return no config\n");

	test_node = uint8_t_to_uint128_t(15);	/* Take right-most leaf of tree (ID 15) */
	ip_addr_result = get_bottom_interface_config(&tree, test_node);
	if (ip_addr_result.ip_type != NONE)	/* IP type should be set to none, IPv6 trees with hostA=0 have no bottom interface */
		FAILF("get_bottom_interface_config() should return no config\n");

	/* Test with routable local networks (hostA!=0) */
	tree.hostA = 64;	/* Each node has a /64 network attached */
	
	test_node = get_root_node_id(&tree);	/* Will get 8 */
	ip_addr_result = get_bottom_interface_config(&tree, test_node);
	inet_ntop(AF_INET6, &(ip_addr_result.in_addr.__ipv6_in6_addr), ip_addr_str, INET6_ADDRSTRLEN);
	if (tree.ip_type != IPV6 || tree.Rmax != 4 || tree.hostA != 64)
		FAILF("get_bottom_interface_config() modified the input tree argument\n");
	if (ip_addr_result.ip_type != IPV6)	/* ip_type should have been propagated as is to result */
		FAILF("get_bottom_interface_config() modified ip_type field\n");
	if (strcmp(ip_addr_str, "fd00:0:0:8::1") != 0)	/* This is the expected root node's bottom interface */
		FAILF("get_bottom_interface_config() got wrong IP address: %s\n", ip_addr_str);
	if (ip_addr_result.prefix != 64)	/* Should get /64 for root node's bottom interface netmask */
		FAILF("get_bottom_interface_config() got wrong netmask: %d\n", ip_addr_result.prefix);

	test_node = get_left_child_node_id(&tree, get_root_node_id(&tree));	/* Will get 4 */
	ip_addr_result = get_bottom_interface_config(&tree, test_node);
	inet_ntop(AF_INET6, &(ip_addr_result.in_addr.__ipv6_in6_addr), ip_addr_str, INET6_ADDRSTRLEN);
	if (ip_addr_result.ip_type != IPV6)	/* ip_type should have been propagated as is to result */
		FAILF("get_bottom_interface_config() modified ip_type field\n");
	if (strcmp(ip_addr_str, "fd00:0:0:4::1") != 0)	/* This is the expected root left child node's bottom interface */
		FAILF("get_bottom_interface_config() got wrong IP address: %s\n", ip_addr_str);
	if (ip_addr_result.prefix != 64)	/* Should get /64 for root node's bottom interface netmask */
		FAILF("get_bottom_interface_config() got wrong netmask: %d\n", ip_addr_result.prefix);
	
	test_node = get_right_child_node_id(&tree, get_root_node_id(&tree));	/* Will get 12 */
	ip_addr_result = get_bottom_interface_config(&tree, test_node);
	inet_ntop(AF_INET6, &(ip_addr_result.in_addr.__ipv6_in6_addr), ip_addr_str, INET6_ADDRSTRLEN);
	if (ip_addr_result.ip_type != IPV6)	/* ip_type should have been propagated as is to result */
		FAILF("get_bottom_interface_config() modified ip_type field\n");
	if (strcmp(ip_addr_str, "fd00:0:0:c::1") != 0)	/* This is the expected root right child node's bottom interface */
		FAILF("get_bottom_interface_config() got wrong IP address: %s\n", ip_addr_str);
	if (ip_addr_result.prefix != 64)	/* Should get /64 for root node's bottom interface netmask */
		FAILF("get_bottom_interface_config() got wrong netmask: %d\n", ip_addr_result.prefix);

	test_node = uint8_t_to_uint128_t(1);	/* Take left-most leaf of tree (ID 1) */
	ip_addr_result = get_bottom_interface_config(&tree, test_node);
	inet_ntop(AF_INET6, &(ip_addr_result.in_addr.__ipv6_in6_addr), ip_addr_str, INET6_ADDRSTRLEN);
	if (ip_addr_result.ip_type != IPV6)	/* ip_type should have been propagated as is to result */
		FAILF("get_bottom_interface_config() modified ip_type field\n");
	if (strcmp(ip_addr_str, "fd00:0:0:1::1") != 0)	/* This is the expected tree's left-most leaf's bottom interface */
		FAILF("get_bottom_interface_config() got wrong IP address: %s\n", ip_addr_str);
	if (ip_addr_result.prefix != 64)	/* Should get /64 for root node's bottom interface netmask */
		FAILF("get_bottom_interface_config() got wrong netmask: %d\n", ip_addr_result.prefix);

	test_node = uint8_t_to_uint128_t(15);	/* Take right-most leaf of tree (ID 15) */
	ip_addr_result = get_bottom_interface_config(&tree, test_node);
	inet_ntop(AF_INET6, &(ip_addr_result.in_addr.__ipv6_in6_addr), ip_addr_str, INET6_ADDRSTRLEN);
	if (ip_addr_result.ip_type != IPV6)	/* ip_type should have been propagated as is to result */
		FAILF("get_bottom_interface_config() modified ip_type field\n");
	if (strcmp(ip_addr_str, "fd00:0:0:f::1") != 0)	/*This is the expected tree's right-most leaf's bottom interface */
		FAILF("get_bottom_interface_config() got wrong IP address: %s\n", ip_addr_str);
	if (ip_addr_result.prefix != 64)	/* Should get /64 for root node's bottom interface netmask */
		FAILF("get_bottom_interface_config() got wrong netmask: %d\n", ip_addr_result.prefix);

	tree.hostA = 32;	/* Each node has a /96 network attached (32 bits reserved for host part) */
	
	test_node = get_root_node_id(&tree);	/* Will get 8 */
	ip_addr_result = get_bottom_interface_config(&tree, test_node);
	inet_ntop(AF_INET6, &(ip_addr_result.in_addr.__ipv6_in6_addr), ip_addr_str, INET6_ADDRSTRLEN);
	if (tree.ip_type != IPV6 || tree.Rmax != 4 || tree.hostA != 32)
		FAILF("get_bottom_interface_config() modified the input tree argument\n");
	if (ip_addr_result.ip_type != IPV6)	/* ip_type should have been propagated as is to result */
		FAILF("get_bottom_interface_config() modified ip_type field\n");
	if (strcmp(ip_addr_str, "fd00::8:0:1") != 0)	/* This is the expected root node's bottom interface */
		FAILF("get_bottom_interface_config() got wrong IP address: %s\n", ip_addr_str);
	if (ip_addr_result.prefix != 96)	/* Should get /64 for root node's bottom interface netmask */
		FAILF("get_bottom_interface_config() got wrong netmask: %d\n", ip_addr_result.prefix);

	test_node = get_left_child_node_id(&tree, get_root_node_id(&tree));	/* Will get 4 */
	ip_addr_result = get_bottom_interface_config(&tree, test_node);
	inet_ntop(AF_INET6, &(ip_addr_result.in_addr.__ipv6_in6_addr), ip_addr_str, INET6_ADDRSTRLEN);
	if (ip_addr_result.ip_type != IPV6)	/* ip_type should have been propagated as is to result */
		FAILF("get_bottom_interface_config() modified ip_type field\n");
	if (strcmp(ip_addr_str, "fd00::4:0:1") != 0)	/* This is the expected root left child node's bottom interface */
		FAILF("get_bottom_interface_config() got wrong IP address: %s\n", ip_addr_str);
	if (ip_addr_result.prefix != 96)	/* Should get /64 for root node's bottom interface netmask */
		FAILF("get_bottom_interface_config() got wrong netmask: %d\n", ip_addr_result.prefix);
	
	test_node = get_right_child_node_id(&tree, get_root_node_id(&tree));	/* Will get 12 */
	ip_addr_result = get_bottom_interface_config(&tree, test_node);
	inet_ntop(AF_INET6, &(ip_addr_result.in_addr.__ipv6_in6_addr), ip_addr_str, INET6_ADDRSTRLEN);
	if (ip_addr_result.ip_type != IPV6)	/* ip_type should have been propagated as is to result */
		FAILF("get_bottom_interface_config() modified ip_type field\n");
	if (strcmp(ip_addr_str, "fd00::c:0:1") != 0)	/* This is the expected root right child node's bottom interface */
		FAILF("get_bottom_interface_config() got wrong IP address: %s\n", ip_addr_str);
	if (ip_addr_result.prefix != 96)	/* Should get /64 for root node's bottom interface netmask */
		FAILF("get_bottom_interface_config() got wrong netmask: %d\n", ip_addr_result.prefix);

	test_node = uint8_t_to_uint128_t(1);	/* Take left-most leaf of tree (ID 1) */
	ip_addr_result = get_bottom_interface_config(&tree, test_node);
	inet_ntop(AF_INET6, &(ip_addr_result.in_addr.__ipv6_in6_addr), ip_addr_str, INET6_ADDRSTRLEN);
	if (ip_addr_result.ip_type != IPV6)	/* ip_type should have been propagated as is to result */
		FAILF("get_bottom_interface_config() modified ip_type field\n");
	if (strcmp(ip_addr_str, "fd00::1:0:1") != 0)	/* This is the expected tree's left-most leaf's bottom interface */
		FAILF("get_bottom_interface_config() got wrong IP address: %s\n", ip_addr_str);
	if (ip_addr_result.prefix != 96)	/* Should get /96 for root node's bottom interface netmask */
		FAILF("get_bottom_interface_config() got wrong netmask: %d\n", ip_addr_result.prefix);

	test_node = uint8_t_to_uint128_t(15);	/* Take right-most leaf of tree (ID 15) */
	ip_addr_result = get_bottom_interface_config(&tree, test_node);
	inet_ntop(AF_INET6, &(ip_addr_result.in_addr.__ipv6_in6_addr), ip_addr_str, INET6_ADDRSTRLEN);
	if (ip_addr_result.ip_type != IPV6)	/* ip_type should have been propagated as is to result */
		FAILF("get_bottom_interface_config() modified ip_type field\n");
	if (strcmp(ip_addr_str, "fd00::f:0:1") != 0)	/*This is the expected tree's right-most leaf's bottom interface */
		FAILF("get_bottom_interface_config() got wrong IP address: %s\n", ip_addr_str);
	if (ip_addr_result.prefix != 96)	/* Should get /96 for root node's bottom interface netmask */
		FAILF("get_bottom_interface_config() got wrong netmask: %d\n", ip_addr_result.prefix);

#endif	// IPV6_SUPPORT

#ifdef IPV4_SUPPORT
	tree.ip_type = IPV4;
	tree.Rmax = 6;
	tree.hostA = 2;
#ifndef HAS_INT128
	U128_SET_ZERO(tree.prefix);
	tree.prefix.uint128_a8[12] = 192;
	tree.prefix.uint128_a8[13] = 168;
	tree.prefix.uint128_a8[14] = 0;
	tree.prefix.uint128_a8[15] = 0;	/* Prefix is 192.168.0.0/24 */
#else
	tree.prefix = (uint128_t)192<<24 | (uint128_t)168<<16 | (uint128_t)0<<8 | (uint128_t)0;
#endif

#warning Test for IPv4 is not implemented yet
	//ip_addr_result = get_right_interface_config(&tree, test_node);
	//FIXME: Add unit test for get_right_interface_config() on IPv4 trees

	//if (ip_addr_result.ip_type != IPV4)	/* ip_type should have been propagated as is to result */
	//	FAILF("get_right_interface_config() modified ip_type field\n");

#endif	// IPV4_SUPPORT

	printf("%s: tests passed\n", __func__);
}

/**
 * \brief Private function that converts an ip_route_t variable to its string representation (using the / prefix notation)
 *
 * \param[in] input The route to output
 * \param[out] output The buffer where the resulting string will be stored, which should be of INET6_ADDRSTRLEN+1+4 bytes for IPv6 and INET_ADDRSTRLEN+1+3 for IPv4
 */
void ip_route_to_str(const ip_route_t input, char* output) {
	size_t pos;
#ifdef IPV6_SUPPORT
	if (input.ip_type == IPV6) {
		/* First dump the IPv6 address */
		if (inet_ntop(AF_INET6, &(input.in_addr.__ipv6_in6_addr), output, INET6_ADDRSTRLEN) == NULL) {
			output[0] = '\0';	/* Failure */
			return;
		}
		pos=strlen(output);
		output[pos++]='/';	/* Add the prefix separator '/' */
		snprintf(&(output[pos]), 4, "%u", input.prefix);	/* Prepend with 3 digits max of prefix (+ terminating '\0') */
		return;
	}
#endif

#ifdef IPV4_SUPPORT
	if (input.ip_type == IPV4)
		FAILF("tests on IPV4 not supported yet\n");
#endif

	output[0] = '\0';	/* Empty string if ip_type == NONE */
}

/* Unit test for get_left_interface_route(), get_right_interface_route(), get_top_interface_route() and get_bottom_interface_route()
 */
TEST(node_tests, test_get_left_right_top_bottom_interface_route) {
	node_id_t test_node;
	self_ip_routing_tree_t tree;
	ip_route_t ip_route_result;
	char ipv6_route_str[INET6_ADDRSTRLEN+1+4];	/* +4 to accomodate for /aaa prefixes */

#ifdef IPV6_SUPPORT
	tree.ip_type = IPV6;
	tree.Rmax = 4;
#ifndef HAS_INT128
	U128_SET_ZERO(tree.prefix);
	tree.prefix.uint128_a8[0] = 0xfd;	/* Prefix is fd00::/124 */
#else
	tree.prefix = (uint128_t)0xfd << 120;   /* Prefix is fd00::/124 */
#endif

	tree.hostA = 0;	/* First perform test without any local network attached to nodes (hostA=0) */
	test_node = get_root_node_id(&tree);	/* Will get 8 */
	ip_route_to_str(ip_route_result = get_left_interface_route(&tree, test_node), ipv6_route_str);
	if (ip_route_result.ip_type != IPV6)	/* ip_type should have been propagated as is to ip_route_result */
		FAILF("get_left_interface_route() modified ip_type field\n");
	if (strcmp(ipv6_route_str, "fd00::/125") != 0)
		FAILF("get_left_interface_route() got wrong IP address: %s\n", ipv6_route_str);

	ip_route_to_str(ip_route_result = get_right_interface_route(&tree, test_node), ipv6_route_str);
	if (ip_route_result.ip_type != IPV6)	/* ip_type should have been propagated as is to ip_route_result */
		FAILF("get_right_interface_route() modified ip_type field\n");
	if (strcmp(ipv6_route_str, "fd00::8/125") != 0)
		FAILF("get_right_interface_route() got wrong IP address: %s\n", ipv6_route_str);

	ip_route_result = get_top_interface_route(&tree, test_node);
	if (ip_route_result.ip_type != NONE)	/* root node has no top route */
		FAILF("get_top_interface_route() should return no route\n");

	ip_route_result = get_bottom_interface_route(&tree, test_node);
	if (ip_route_result.ip_type != NONE)	/* on IPv6 trees with hostA=0, there is no bottom route */
		FAILF("get_bottom_interface_route() should return no route\n");

	ip_route_to_str(get_left_interface_route(&tree, uint8_t_to_uint128_t(4)), ipv6_route_str);
	if (strcmp(ipv6_route_str, "fd00::/126") != 0)
		FAILF("get_left_interface_route() got wrong IP address: %s\n", ipv6_route_str);

	ip_route_to_str(get_right_interface_route(&tree, uint8_t_to_uint128_t(4)), ipv6_route_str);
	if (strcmp(ipv6_route_str, "fd00::4/126") != 0)
		FAILF("get_right_interface_route() got wrong IP address: %s\n", ipv6_route_str);

	ip_route_to_str(get_top_interface_route(&tree, uint8_t_to_uint128_t(4)), ipv6_route_str);
	if (strcmp(ipv6_route_str, "fd00::8/128") != 0)
		FAILF("get_top_interface_route() got wrong IP address: %s\n", ipv6_route_str);

	ip_route_result = get_bottom_interface_route(&tree, uint8_t_to_uint128_t(4));
	if (ip_route_result.ip_type != NONE)	/* on IPv6 trees with hostA=0, there is no bottom route */
		FAILF("get_bottom_interface_route() should return no route\n");

	ip_route_to_str(get_left_interface_route(&tree, uint8_t_to_uint128_t(12)), ipv6_route_str);
	if (strcmp(ipv6_route_str, "fd00::8/126") != 0)
		FAILF("get_left_interface_route() got wrong IP address: %s\n", ipv6_route_str);

	ip_route_to_str(get_right_interface_route(&tree, uint8_t_to_uint128_t(12)), ipv6_route_str);
	if (strcmp(ipv6_route_str, "fd00::c/126") != 0)
		FAILF("get_right_interface_route() got wrong IP address: %s\n", ipv6_route_str);

	ip_route_to_str(get_top_interface_route(&tree, uint8_t_to_uint128_t(12)), ipv6_route_str);
	if (strcmp(ipv6_route_str, "fd00::8/128") != 0)
		FAILF("get_top_interface_route() got wrong IP address: %s\n", ipv6_route_str);

	ip_route_result = get_bottom_interface_route(&tree, uint8_t_to_uint128_t(12));
	if (ip_route_result.ip_type != NONE)	/* on IPv6 trees with hostA=0, there is no bottom route */
		FAILF("get_bottom_interface_route() should return no route\n");

	ip_route_to_str(get_left_interface_route(&tree, uint8_t_to_uint128_t(2)), ipv6_route_str);
	if (strcmp(ipv6_route_str, "fd00::1/128") != 0)
		FAILF("get_left_interface_route() got wrong IP address: %s\n", ipv6_route_str);

	ip_route_to_str(get_right_interface_route(&tree, uint8_t_to_uint128_t(2)), ipv6_route_str);
	if (strcmp(ipv6_route_str, "fd00::3/128") != 0)
		FAILF("get_right_interface_route() got wrong IP address: %s\n", ipv6_route_str);

	ip_route_to_str(get_top_interface_route(&tree, uint8_t_to_uint128_t(2)), ipv6_route_str);
	if (strcmp(ipv6_route_str, "fd00::4/128") != 0)
		FAILF("get_top_interface_route() got wrong IP address: %s\n", ipv6_route_str);

	ip_route_result = get_bottom_interface_route(&tree, uint8_t_to_uint128_t(2));
	if (ip_route_result.ip_type != NONE)	/* on IPv6 trees with hostA=0, there is no bottom route */
		FAILF("get_bottom_interface_route() should return no route\n");

	ip_route_to_str(get_left_interface_route(&tree, uint8_t_to_uint128_t(6)), ipv6_route_str);
	if (strcmp(ipv6_route_str, "fd00::5/128") != 0)
		FAILF("get_left_interface_route() got wrong IP address: %s\n", ipv6_route_str);

	ip_route_to_str(get_right_interface_route(&tree, uint8_t_to_uint128_t(6)), ipv6_route_str);
	if (strcmp(ipv6_route_str, "fd00::7/128") != 0)
		FAILF("get_right_interface_route() got wrong IP address: %s\n", ipv6_route_str);

	ip_route_to_str(get_top_interface_route(&tree, uint8_t_to_uint128_t(6)), ipv6_route_str);
	if (strcmp(ipv6_route_str, "fd00::4/128") != 0)
		FAILF("get_top_interface_route() got wrong IP address: %s\n", ipv6_route_str);

	ip_route_result = get_bottom_interface_route(&tree, uint8_t_to_uint128_t(6));
	if (ip_route_result.ip_type != NONE)	/* on IPv6 trees with hostA=0, there is no bottom route */
		FAILF("get_bottom_interface_route() should return no route\n");

	ip_route_to_str(get_left_interface_route(&tree, uint8_t_to_uint128_t(10)), ipv6_route_str);
	if (strcmp(ipv6_route_str, "fd00::9/128") != 0)
		FAILF("get_left_interface_route() got wrong IP address: %s\n", ipv6_route_str);

	ip_route_to_str(get_right_interface_route(&tree, uint8_t_to_uint128_t(10)), ipv6_route_str);
	if (strcmp(ipv6_route_str, "fd00::b/128") != 0)
		FAILF("get_right_interface_route() got wrong IP address: %s\n", ipv6_route_str);

	ip_route_to_str(get_top_interface_route(&tree, uint8_t_to_uint128_t(10)), ipv6_route_str);
	if (strcmp(ipv6_route_str, "fd00::c/128") != 0)
		FAILF("get_top_interface_route() got wrong IP address: %s\n", ipv6_route_str);

	ip_route_result = get_bottom_interface_route(&tree, uint8_t_to_uint128_t(10));
	if (ip_route_result.ip_type != NONE)	/* on IPv6 trees with hostA=0, there is no bottom route */
		FAILF("get_bottom_interface_route() should return no route\n");

	ip_route_to_str(get_left_interface_route(&tree, uint8_t_to_uint128_t(14)), ipv6_route_str);
	if (strcmp(ipv6_route_str, "fd00::d/128") != 0)
		FAILF("get_left_interface_route() got wrong IP address: %s\n", ipv6_route_str);

	ip_route_to_str(get_right_interface_route(&tree, uint8_t_to_uint128_t(14)), ipv6_route_str);
	if (strcmp(ipv6_route_str, "fd00::f/128") != 0)
		FAILF("get_right_interface_route() got wrong IP address: %s\n", ipv6_route_str);

	ip_route_to_str(get_top_interface_route(&tree, uint8_t_to_uint128_t(14)), ipv6_route_str);
	if (strcmp(ipv6_route_str, "fd00::c/128") != 0)
		FAILF("get_top_interface_route() got wrong IP address: %s\n", ipv6_route_str);

	ip_route_result = get_bottom_interface_route(&tree, uint8_t_to_uint128_t(14));
	if (ip_route_result.ip_type != NONE)	/* on IPv6 trees with hostA=0, there is no bottom route */
		FAILF("get_bottom_interface_route() should return no route\n");

	ip_route_result = get_left_interface_route(&tree, uint8_t_to_uint128_t(1));
	if (ip_route_result.ip_type != NONE)
		FAILF("get_left_interface_route() should return no route\n");

	ip_route_result = get_right_interface_route(&tree, uint8_t_to_uint128_t(1));
	if (ip_route_result.ip_type != NONE)
		FAILF("get_right_interface_route() should return no route\n");

	ip_route_to_str(get_top_interface_route(&tree, uint8_t_to_uint128_t(1)), ipv6_route_str);
	if (strcmp(ipv6_route_str, "fd00::2/128") != 0)
		FAILF("get_top_interface_route() got wrong IP address: %s\n", ipv6_route_str);

	ip_route_result = get_bottom_interface_route(&tree, uint8_t_to_uint128_t(1));
	if (ip_route_result.ip_type != NONE)	/* on IPv6 trees with hostA=0, there is no bottom route */
		FAILF("get_bottom_interface_route() should return no route\n");

	ip_route_result = get_left_interface_route(&tree, uint8_t_to_uint128_t(15));
	if (ip_route_result.ip_type != NONE)
		FAILF("get_left_interface_route() should return no route\n");

	ip_route_result = get_right_interface_route(&tree, uint8_t_to_uint128_t(15));
	if (ip_route_result.ip_type != NONE)
		FAILF("get_right_interface_route() should return no route\n");

	ip_route_to_str(get_top_interface_route(&tree, uint8_t_to_uint128_t(15)), ipv6_route_str);
	if (strcmp(ipv6_route_str, "fd00::e/128") != 0)
		FAILF("get_top_interface_route() got wrong IP address: %s\n", ipv6_route_str);

	ip_route_result = get_bottom_interface_route(&tree, uint8_t_to_uint128_t(15));
	if (ip_route_result.ip_type != NONE)	/* on IPv6 trees with hostA=0, there is no bottom route */
		FAILF("get_bottom_interface_route() should return no route\n");

	/* Test with routable local networks (hostA!=0) */
	tree.hostA = 64;	/* Each node has a /64 network attached */

	test_node = get_root_node_id(&tree);	/* Will get 8 */
	ip_route_to_str(ip_route_result = get_left_interface_route(&tree, test_node), ipv6_route_str);
	if (ip_route_result.ip_type != IPV6)	/* ip_type should have been propagated as is to ip_route_result */
		FAILF("get_left_interface_route() modified ip_type field\n");
	if (strcmp(ipv6_route_str, "fd00::/61") != 0)
		FAILF("get_left_interface_route() got wrong IP address: %s\n", ipv6_route_str);

	ip_route_to_str(ip_route_result = get_right_interface_route(&tree, test_node), ipv6_route_str);
	if (ip_route_result.ip_type != IPV6)	/* ip_type should have been propagated as is to ip_route_result */
		FAILF("get_right_interface_route() modified ip_type field\n");
	if (strcmp(ipv6_route_str, "fd00:0:0:8::/61") != 0)
		FAILF("get_right_interface_route() got wrong IP address: %s\n", ipv6_route_str);

	ip_route_result = get_top_interface_route(&tree, test_node);
	if (ip_route_result.ip_type != NONE)	/* root node has no top route */
		FAILF("get_top_interface_route() should return no route\n");

	ip_route_to_str(ip_route_result = get_bottom_interface_route(&tree, test_node), ipv6_route_str);
	if (ip_route_result.ip_type != IPV6)	/* ip_type should have been propagated as is to ip_route_result */
		FAILF("get_bottom_interface_route() modified ip_type field\n");
	if (strcmp(ipv6_route_str, "fd00:0:0:8::/64") != 0)
		FAILF("get_bottom_interface_route() got wrong IP address: %s\n", ipv6_route_str);

	ip_route_to_str(get_left_interface_route(&tree, uint8_t_to_uint128_t(4)), ipv6_route_str);
	if (strcmp(ipv6_route_str, "fd00::/62") != 0)
		FAILF("get_left_interface_route() got wrong IP address: %s\n", ipv6_route_str);

	ip_route_to_str(get_right_interface_route(&tree, uint8_t_to_uint128_t(4)), ipv6_route_str);
	if (strcmp(ipv6_route_str, "fd00:0:0:4::/62") != 0)
		FAILF("get_right_interface_route() got wrong IP address: %s\n", ipv6_route_str);

	ip_route_to_str(get_top_interface_route(&tree, uint8_t_to_uint128_t(4)), ipv6_route_str);
	if (strcmp(ipv6_route_str, "fd00:0:0:8::/64") != 0)
		FAILF("get_top_interface_route() got wrong IP address: %s\n", ipv6_route_str);

	ip_route_to_str(get_bottom_interface_route(&tree, uint8_t_to_uint128_t(4)), ipv6_route_str);
	if (strcmp(ipv6_route_str, "fd00:0:0:4::/64") != 0)
		FAILF("get_bottom_interface_route() got wrong IP address: %s\n", ipv6_route_str);

	ip_route_to_str(get_left_interface_route(&tree, uint8_t_to_uint128_t(12)), ipv6_route_str);
	if (strcmp(ipv6_route_str, "fd00:0:0:8::/62") != 0)
		FAILF("get_left_interface_route() got wrong IP address: %s\n", ipv6_route_str);

	ip_route_to_str(get_right_interface_route(&tree, uint8_t_to_uint128_t(12)), ipv6_route_str);
	if (strcmp(ipv6_route_str, "fd00:0:0:c::/62") != 0)
		FAILF("get_right_interface_route() got wrong IP address: %s\n", ipv6_route_str);

	ip_route_to_str(get_top_interface_route(&tree, uint8_t_to_uint128_t(12)), ipv6_route_str);
	if (strcmp(ipv6_route_str, "fd00:0:0:8::/64") != 0)
		FAILF("get_top_interface_route() got wrong IP address: %s\n", ipv6_route_str);

	ip_route_to_str(get_bottom_interface_route(&tree, uint8_t_to_uint128_t(12)), ipv6_route_str);
	if (strcmp(ipv6_route_str, "fd00:0:0:c::/64") != 0)
		FAILF("get_bottom_interface_route() got wrong IP address: %s\n", ipv6_route_str);

	ip_route_to_str(get_left_interface_route(&tree, uint8_t_to_uint128_t(2)), ipv6_route_str);
	if (strcmp(ipv6_route_str, "fd00:0:0:1::/64") != 0)
		FAILF("get_left_interface_route() got wrong IP address: %s\n", ipv6_route_str);

	ip_route_to_str(get_right_interface_route(&tree, uint8_t_to_uint128_t(2)), ipv6_route_str);
	if (strcmp(ipv6_route_str, "fd00:0:0:3::/64") != 0)
		FAILF("get_right_interface_route() got wrong IP address: %s\n", ipv6_route_str);

	ip_route_to_str(get_top_interface_route(&tree, uint8_t_to_uint128_t(2)), ipv6_route_str);
	if (strcmp(ipv6_route_str, "fd00:0:0:4::/64") != 0)
		FAILF("get_top_interface_route() got wrong IP address: %s\n", ipv6_route_str);

	ip_route_to_str(get_bottom_interface_route(&tree, uint8_t_to_uint128_t(2)), ipv6_route_str);
	if (strcmp(ipv6_route_str, "fd00:0:0:2::/64") != 0)
		FAILF("get_bottom_interface_route() got wrong IP address: %s\n", ipv6_route_str);

	ip_route_to_str(get_left_interface_route(&tree, uint8_t_to_uint128_t(6)), ipv6_route_str);
	if (strcmp(ipv6_route_str, "fd00:0:0:5::/64") != 0)
		FAILF("get_left_interface_route() got wrong IP address: %s\n", ipv6_route_str);
	
	ip_route_to_str(get_right_interface_route(&tree, uint8_t_to_uint128_t(6)), ipv6_route_str);
	if (strcmp(ipv6_route_str, "fd00:0:0:7::/64") != 0)
		FAILF("get_right_interface_route() got wrong IP address: %s\n", ipv6_route_str);

	ip_route_to_str(get_top_interface_route(&tree, uint8_t_to_uint128_t(6)), ipv6_route_str);
	if (strcmp(ipv6_route_str, "fd00:0:0:4::/64") != 0)
		FAILF("get_top_interface_route() got wrong IP address: %s\n", ipv6_route_str);

	ip_route_to_str(get_bottom_interface_route(&tree, uint8_t_to_uint128_t(6)), ipv6_route_str);
	if (strcmp(ipv6_route_str, "fd00:0:0:6::/64") != 0)
		FAILF("get_bottom_interface_route() got wrong IP address: %s\n", ipv6_route_str);

	ip_route_to_str(get_left_interface_route(&tree, uint8_t_to_uint128_t(10)), ipv6_route_str);
	if (strcmp(ipv6_route_str, "fd00:0:0:9::/64") != 0)
		FAILF("get_left_interface_route() got wrong IP address: %s\n", ipv6_route_str);

	ip_route_to_str(get_right_interface_route(&tree, uint8_t_to_uint128_t(10)), ipv6_route_str);
	if (strcmp(ipv6_route_str, "fd00:0:0:b::/64") != 0)
		FAILF("get_right_interface_route() got wrong IP address: %s\n", ipv6_route_str);

	ip_route_to_str(get_top_interface_route(&tree, uint8_t_to_uint128_t(10)), ipv6_route_str);
	if (strcmp(ipv6_route_str, "fd00:0:0:c::/64") != 0)
		FAILF("get_top_interface_route() got wrong IP address: %s\n", ipv6_route_str);

	ip_route_to_str(get_bottom_interface_route(&tree, uint8_t_to_uint128_t(10)), ipv6_route_str);
	if (strcmp(ipv6_route_str, "fd00:0:0:a::/64") != 0)
		FAILF("get_bottom_interface_route() got wrong IP address: %s\n", ipv6_route_str);

	ip_route_to_str(get_left_interface_route(&tree, uint8_t_to_uint128_t(14)), ipv6_route_str);
	if (strcmp(ipv6_route_str, "fd00:0:0:d::/64") != 0)
		FAILF("get_left_interface_route() got wrong IP address: %s\n", ipv6_route_str);

	ip_route_to_str(get_right_interface_route(&tree, uint8_t_to_uint128_t(14)), ipv6_route_str);
	if (strcmp(ipv6_route_str, "fd00:0:0:f::/64") != 0)
		FAILF("get_right_interface_route() got wrong IP address: %s\n", ipv6_route_str);

	ip_route_to_str(get_top_interface_route(&tree, uint8_t_to_uint128_t(14)), ipv6_route_str);
	if (strcmp(ipv6_route_str, "fd00:0:0:c::/64") != 0)
		FAILF("get_top_interface_route() got wrong IP address: %s\n", ipv6_route_str);

	ip_route_to_str(get_bottom_interface_route(&tree, uint8_t_to_uint128_t(14)), ipv6_route_str);
	if (strcmp(ipv6_route_str, "fd00:0:0:e::/64") != 0)
		FAILF("get_bottom_interface_route() got wrong IP address: %s\n", ipv6_route_str);

	ip_route_result = get_left_interface_route(&tree, uint8_t_to_uint128_t(1));
	if (ip_route_result.ip_type != NONE)
		FAILF("get_left_interface_route() should return no route\n");

	ip_route_result = get_right_interface_route(&tree, uint8_t_to_uint128_t(1));
	if (ip_route_result.ip_type != NONE)
		FAILF("get_right_interface_route() should return no route\n");

	ip_route_to_str(get_top_interface_route(&tree, uint8_t_to_uint128_t(1)), ipv6_route_str);
	if (strcmp(ipv6_route_str, "fd00:0:0:2::/64") != 0)
		FAILF("get_top_interface_route() got wrong IP address: %s\n", ipv6_route_str);

	ip_route_to_str(get_bottom_interface_route(&tree, uint8_t_to_uint128_t(1)), ipv6_route_str);
	if (strcmp(ipv6_route_str, "fd00:0:0:1::/64") != 0)
		FAILF("get_bottom_interface_route() got wrong IP address: %s\n", ipv6_route_str);

	ip_route_result = get_left_interface_route(&tree, uint8_t_to_uint128_t(15));
	if (ip_route_result.ip_type != NONE)
		FAILF("get_left_interface_route() should return no route\n");

	ip_route_result = get_right_interface_route(&tree, uint8_t_to_uint128_t(15));
	if (ip_route_result.ip_type != NONE)
		FAILF("get_right_interface_route() should return no route\n");

	ip_route_to_str(get_top_interface_route(&tree, uint8_t_to_uint128_t(15)), ipv6_route_str);
	if (strcmp(ipv6_route_str, "fd00:0:0:e::/64") != 0)
		FAILF("get_top_interface_route() got wrong IP address: %s\n", ipv6_route_str);

	ip_route_to_str(get_bottom_interface_route(&tree, uint8_t_to_uint128_t(15)), ipv6_route_str);
	if (strcmp(ipv6_route_str, "fd00:0:0:f::/64") != 0)
		FAILF("get_bottom_interface_route() got wrong IP address: %s\n", ipv6_route_str);

	tree.hostA = 40;	/* Just a few additional checks when each node has a /88 network attached (40 bits allocated for local networks hosts) */
	/* Here we only test the root node, its two children, and the two extreme leaves (first (ID=1) and last (ID=15)) */

	test_node = get_root_node_id(&tree);	/* Will get 8 */
	ip_route_to_str(ip_route_result = get_left_interface_route(&tree, test_node), ipv6_route_str);
	if (ip_route_result.ip_type != IPV6)	/* ip_type should have been propagated as is to ip_route_result */
		FAILF("get_left_interface_route() modified ip_type field\n");
	if (strcmp(ipv6_route_str, "fd00::/85") != 0)
		FAILF("get_left_interface_route() got wrong IP address: %s\n", ipv6_route_str);

	ip_route_to_str(ip_route_result = get_right_interface_route(&tree, test_node), ipv6_route_str);
	if (ip_route_result.ip_type != IPV6)	/* ip_type should have been propagated as is to ip_route_result */
		FAILF("get_right_interface_route() modified ip_type field\n");
	if (strcmp(ipv6_route_str, "fd00::800:0:0/85") != 0)
		FAILF("get_right_interface_route() got wrong IP address: %s\n", ipv6_route_str);

	ip_route_result = get_top_interface_route(&tree, test_node);
	if (ip_route_result.ip_type != NONE)	/* root node has no top route */
		FAILF("get_top_interface_route() should return no route\n");

	ip_route_to_str(ip_route_result = get_bottom_interface_route(&tree, test_node), ipv6_route_str);
	if (ip_route_result.ip_type != IPV6)	/* ip_type should have been propagated as is to ip_route_result */
		FAILF("get_bottom_interface_route() modified ip_type field\n");
	if (strcmp(ipv6_route_str, "fd00::800:0:0/88") != 0)
		FAILF("get_bottom_interface_route() got wrong IP address: %s\n", ipv6_route_str);

	ip_route_to_str(get_left_interface_route(&tree, uint8_t_to_uint128_t(4)), ipv6_route_str);
	if (strcmp(ipv6_route_str, "fd00::/86") != 0)
		FAILF("get_left_interface_route() got wrong IP address: %s\n", ipv6_route_str);

	ip_route_to_str(get_right_interface_route(&tree, uint8_t_to_uint128_t(4)), ipv6_route_str);
	if (strcmp(ipv6_route_str, "fd00::400:0:0/86") != 0)
		FAILF("get_right_interface_route() got wrong IP address: %s\n", ipv6_route_str);

	ip_route_to_str(get_top_interface_route(&tree, uint8_t_to_uint128_t(4)), ipv6_route_str);
	if (strcmp(ipv6_route_str, "fd00::800:0:0/88") != 0)
		FAILF("get_top_interface_route() got wrong IP address: %s\n", ipv6_route_str);

	ip_route_to_str(get_bottom_interface_route(&tree, uint8_t_to_uint128_t(4)), ipv6_route_str);
	if (strcmp(ipv6_route_str, "fd00::400:0:0/88") != 0)
		FAILF("get_bottom_interface_route() got wrong IP address: %s\n", ipv6_route_str);

	ip_route_to_str(get_left_interface_route(&tree, uint8_t_to_uint128_t(12)), ipv6_route_str);
	if (strcmp(ipv6_route_str, "fd00::800:0:0/86") != 0)
		FAILF("get_left_interface_route() got wrong IP address: %s\n", ipv6_route_str);

	ip_route_to_str(get_right_interface_route(&tree, uint8_t_to_uint128_t(12)), ipv6_route_str);
	if (strcmp(ipv6_route_str, "fd00::c00:0:0/86") != 0)
		FAILF("get_right_interface_route() got wrong IP address: %s\n", ipv6_route_str);

	ip_route_to_str(get_top_interface_route(&tree, uint8_t_to_uint128_t(12)), ipv6_route_str);
	if (strcmp(ipv6_route_str, "fd00::800:0:0/88") != 0)
		FAILF("get_top_interface_route() got wrong IP address: %s\n", ipv6_route_str);

	ip_route_to_str(get_bottom_interface_route(&tree, uint8_t_to_uint128_t(12)), ipv6_route_str);
	if (strcmp(ipv6_route_str, "fd00::c00:0:0/88") != 0)
		FAILF("get_bottom_interface_route() got wrong IP address: %s\n", ipv6_route_str);

	ip_route_result = get_left_interface_route(&tree, uint8_t_to_uint128_t(1));
	if (ip_route_result.ip_type != NONE)
		FAILF("get_left_interface_route() should return no route\n");

	ip_route_result = get_right_interface_route(&tree, uint8_t_to_uint128_t(1));
	if (ip_route_result.ip_type != NONE)
		FAILF("get_right_interface_route() should return no route\n");

	ip_route_to_str(get_top_interface_route(&tree, uint8_t_to_uint128_t(1)), ipv6_route_str);
	if (strcmp(ipv6_route_str, "fd00::200:0:0/88") != 0)
		FAILF("get_top_interface_route() got wrong IP address: %s\n", ipv6_route_str);

	ip_route_to_str(get_bottom_interface_route(&tree, uint8_t_to_uint128_t(1)), ipv6_route_str);
	if (strcmp(ipv6_route_str, "fd00::100:0:0/88") != 0)
		FAILF("get_bottom_interface_route() got wrong IP address: %s\n", ipv6_route_str);

	ip_route_result = get_left_interface_route(&tree, uint8_t_to_uint128_t(15));
	if (ip_route_result.ip_type != NONE)
		FAILF("get_left_interface_route() should return no route\n");

	ip_route_result = get_right_interface_route(&tree, uint8_t_to_uint128_t(15));
	if (ip_route_result.ip_type != NONE)
		FAILF("get_right_interface_route() should return no route\n");

	ip_route_to_str(get_top_interface_route(&tree, uint8_t_to_uint128_t(15)), ipv6_route_str);
	if (strcmp(ipv6_route_str, "fd00::e00:0:0/88") != 0)
		FAILF("get_top_interface_route() got wrong IP address: %s\n", ipv6_route_str);

	ip_route_to_str(get_bottom_interface_route(&tree, uint8_t_to_uint128_t(15)), ipv6_route_str);
	if (strcmp(ipv6_route_str, "fd00::f00:0:0/88") != 0)
		FAILF("get_bottom_interface_route() got wrong IP address: %s\n", ipv6_route_str);

	tree.hostA = 16;	/* And yet a few more checks when each node has a /102 network attached (16 bits allocated for local networks hosts) */

	/* Here, we increase the Rmax to 16 (tree depth, max for node ID is thus 65535 (see Rmax_to_max_node_id()), which will
	 * increase the node addressing space of the tree compared to previous tests
	 */
	tree.Rmax = 16;

	/* We also give a more specific prefix, using all tree-specific bits (128 - 16 - 16): fd00:0123:4567:89ab:cdef:a55a/96
	 * This prefix will be used for the test tree. In order to check that prefix does not have side effect on addresses, we actually
	 * set to one more bits (outside of the accepted tree common prefix), so we will set tree prefix to:
	 * fd00:0123:4567:89ab:cdef:a55a: ff00:ff00 (bits 8 to 15 and bits 24 to 31 are set (0xff), but should be discarded */

#ifndef HAS_INT128
	U128_SET_ZERO(tree.prefix);
	tree.prefix.uint128_a8[0] = 0xfd;
	tree.prefix.uint128_a8[1] = 0x00;
	tree.prefix.uint128_a8[2] = 0x01;
	tree.prefix.uint128_a8[3] = 0x23;
	tree.prefix.uint128_a8[4] = 0x45;
	tree.prefix.uint128_a8[5] = 0x67;
	tree.prefix.uint128_a8[6] = 0x89;
	tree.prefix.uint128_a8[7] = 0xab;
	tree.prefix.uint128_a8[8] = 0xcd;
	tree.prefix.uint128_a8[9] = 0xef;
	tree.prefix.uint128_a8[10] = 0xa5;
	tree.prefix.uint128_a8[11] = 0x5a;
	/* Note:
	 * Bytes 12 & 13  will be used for nodes in tree addressing (node ID) (16 bits... Rmax=16)
	 * and byte 14 & 15 for local network host addressing (16 bits... hostA=16)
	 * But we are nasty here and also pollute bytes 12 and 14 to test tree prefix overflow side effects as explained above
	 */
	tree.prefix.uint128_a8[12] = 0xff;
	tree.prefix.uint128_a8[14] = 0xff;

#else
	tree.prefix = (uint128_t)0xfd;
	tree.prefix |= tree.prefix << 8 | (uint128_t)0x00;
	tree.prefix |= tree.prefix << 8 | (uint128_t)0x01;
	tree.prefix |= tree.prefix << 8 | (uint128_t)0x23;
	tree.prefix |= tree.prefix << 8 | (uint128_t)0x45;
	tree.prefix |= tree.prefix << 8 | (uint128_t)0x67;
	tree.prefix |= tree.prefix << 8 | (uint128_t)0x89;
	tree.prefix |= tree.prefix << 8 | (uint128_t)0xab;
	tree.prefix |= tree.prefix << 8 | (uint128_t)0xcd;
	tree.prefix |= tree.prefix << 8 | (uint128_t)0xef;
	tree.prefix |= tree.prefix << 8 | (uint128_t)0xa5;
	tree.prefix |= tree.prefix << 8 | (uint128_t)0x5a;
	tree.prefix |= tree.prefix << 8 | (uint128_t)0xff;	/* Pollution on byte 12 */
	tree.prefix <<= 8;	/* Skip byte 13 */
	tree.prefix |= tree.prefix << 8 | (uint128_t)0xff;	/* Pollution on byte 14 */
	tree.prefix <<= 8;	/* Skip byte 15 */
#endif
	if (get_tree_prefix_len(&tree) != 128 - 16 - 16)
		FAILF("get_tree_prefix_len() return value error on IPv6 tree with hostA=16, Rmax=16\n");

	/* As for above, we only test the root node, its two children, and the two extreme leaves (first (ID=1) and last (ID=65535)) */

	test_node = get_root_node_id(&tree);	/* Will get 2^15 = 32768 */

	if (uint128_t_cmp(test_node, power2_to_uint128_t(15)) != 0)	/* Make sure we get 32768 */
		FAILF("get_root_node_id() should return 32768\n");

	ip_route_to_str(ip_route_result = get_left_interface_route(&tree, test_node), ipv6_route_str);
	if (ip_route_result.ip_type != IPV6)	/* ip_type should have been propagated as is to ip_route_result */
		FAILF("get_left_interface_route() modified ip_type field\n");
	if (strcmp(ipv6_route_str, "fd00:0123:4567:89ab:cdef:a55a::/97") != 0)
		FAILF("get_left_interface_route() got wrong IP address: %s\n", ipv6_route_str);

	ip_route_to_str(ip_route_result = get_right_interface_route(&tree, test_node), ipv6_route_str);
	if (ip_route_result.ip_type != IPV6)	/* ip_type should have been propagated as is to ip_route_result */
		FAILF("get_right_interface_route() modified ip_type field\n");
	if (strcmp(ipv6_route_str, "fd00:0123:4567:89ab:cdef:a55a::/97") != 0)
		FAILF("get_right_interface_route() got wrong IP address: %s\n", ipv6_route_str);

	ip_route_result = get_top_interface_route(&tree, test_node);
	if (ip_route_result.ip_type != NONE)	/* root node has no top route */
		FAILF("get_top_interface_route() should return no route\n");

	ip_route_to_str(ip_route_result = get_bottom_interface_route(&tree, test_node), ipv6_route_str);
	if (ip_route_result.ip_type != IPV6)	/* ip_type should have been propagated as is to ip_route_result */
		FAILF("get_bottom_interface_route() modified ip_type field\n");
	if (strcmp(ipv6_route_str, "fd00:0123:4567:89ab:cdef:a55a:8000::/112") != 0)
		FAILF("get_bottom_interface_route() got wrong IP address: %s\n", ipv6_route_str);

	ip_route_to_str(get_left_interface_route(&tree, uint16_t_to_uint128_t(0x4000)), ipv6_route_str);
	if (strcmp(ipv6_route_str, "fd00:0123:4567:89ab:cdef:a55a::/98") != 0)
		FAILF("get_left_interface_route() got wrong IP address: %s\n", ipv6_route_str);

	ip_route_to_str(get_right_interface_route(&tree, uint16_t_to_uint128_t(0x4000)), ipv6_route_str);
	if (strcmp(ipv6_route_str, "fd00:0123:4567:89ab:cdef:a55a:4000::/98") != 0)
		FAILF("get_right_interface_route() got wrong IP address: %s\n", ipv6_route_str);

	ip_route_to_str(get_top_interface_route(&tree, uint16_t_to_uint128_t(0x4000)), ipv6_route_str);
	if (strcmp(ipv6_route_str, "fd00:0123:4567:89ab:cdef:a55a:8000::/112") != 0)
		FAILF("get_top_interface_route() got wrong IP address: %s\n", ipv6_route_str);

	ip_route_to_str(get_bottom_interface_route(&tree, uint16_t_to_uint128_t(0x4000)), ipv6_route_str);
	if (strcmp(ipv6_route_str, "fd00:0123:4567:89ab:cdef:a55a:4000::/112") != 0)
		FAILF("get_bottom_interface_route() got wrong IP address: %s\n", ipv6_route_str);

	ip_route_to_str(get_left_interface_route(&tree, uint16_t_to_uint128_t(0xc000)), ipv6_route_str);
	if (strcmp(ipv6_route_str, "fd00:0123:4567:89ab:cdef:a55a:8000/98") != 0)
		FAILF("get_left_interface_route() got wrong IP address: %s\n", ipv6_route_str);

	ip_route_to_str(get_right_interface_route(&tree, uint16_t_to_uint128_t(0xc000)), ipv6_route_str);
	if (strcmp(ipv6_route_str, "fd00:0123:4567:89ab:cdef:a55a:c000/98") != 0)
		FAILF("get_right_interface_route() got wrong IP address: %s\n", ipv6_route_str);

	ip_route_to_str(get_top_interface_route(&tree, uint16_t_to_uint128_t(0xc000)), ipv6_route_str);
	if (strcmp(ipv6_route_str, "fd00:0123:4567:89ab:cdef:a55a:8000/112") != 0)
		FAILF("get_top_interface_route() got wrong IP address: %s\n", ipv6_route_str);

	ip_route_to_str(get_bottom_interface_route(&tree, uint16_t_to_uint128_t(0xc000)), ipv6_route_str);
	if (strcmp(ipv6_route_str, "fd00:0123:4567:89ab:cdef:a55a:c000::/112") != 0)
		FAILF("get_bottom_interface_route() got wrong IP address: %s\n", ipv6_route_str);

	ip_route_result = get_left_interface_route(&tree, uint16_t_to_uint128_t(1));
	if (ip_route_result.ip_type != NONE)
		FAILF("get_left_interface_route() should return no route\n");

	ip_route_result = get_right_interface_route(&tree, uint16_t_to_uint128_t(1));
	if (ip_route_result.ip_type != NONE)
		FAILF("get_right_interface_route() should return no route\n");

	ip_route_to_str(get_top_interface_route(&tree, uint16_t_to_uint128_t(1)), ipv6_route_str);
	if (strcmp(ipv6_route_str, "fd00:0123:4567:89ab:cdef:a55a:2::/112") != 0)
		FAILF("get_top_interface_route() got wrong IP address: %s\n", ipv6_route_str);

	ip_route_to_str(get_bottom_interface_route(&tree, uint16_t_to_uint128_t(1)), ipv6_route_str);
	if (strcmp(ipv6_route_str, "fd00:0123:4567:89ab:cdef:a55a:1::/112") != 0)
		FAILF("get_bottom_interface_route() got wrong IP address: %s\n", ipv6_route_str);

	ip_route_result = get_left_interface_route(&tree, uint16_t_to_uint128_t(0xffff));
	if (ip_route_result.ip_type != NONE)
		FAILF("get_left_interface_route() should return no route\n");

	ip_route_result = get_right_interface_route(&tree, uint16_t_to_uint128_t(0xffff));
	if (ip_route_result.ip_type != NONE)
		FAILF("get_right_interface_route() should return no route\n");

	ip_route_to_str(get_top_interface_route(&tree, uint16_t_to_uint128_t(0xffff)), ipv6_route_str);
	if (strcmp(ipv6_route_str, "fd00:0123:4567:89ab:cdef:a55a:fffe::/112") != 0)
		FAILF("get_top_interface_route() got wrong IP address: %s\n", ipv6_route_str);

	ip_route_to_str(get_bottom_interface_route(&tree, uint16_t_to_uint128_t(0xffff)), ipv6_route_str);
	if (strcmp(ipv6_route_str, "fd00:0123:4567:89ab:cdef:a55a:ffff::/112") != 0)
		FAILF("get_bottom_interface_route() got wrong IP address: %s\n", ipv6_route_str);

#endif	// IPV6_SUPPORT

#ifdef IPV4_SUPPORT
	tree.ip_type = IPV4;
	tree.Rmax = 6;
	tree.hostA = 2;
#ifndef HAS_INT128
	U128_SET_ZERO(tree.prefix);
	tree.prefix.uint128_a8[12] = 192;
	tree.prefix.uint128_a8[13] = 168;
	tree.prefix.uint128_a8[14] = 0;
	tree.prefix.uint128_a8[15] = 0;	/* Prefix is 192.168.0.0/24 */
#else
	tree.prefix = (uint128_t)192<<24 | (uint128_t)168<<16 | (uint128_t)0<<8 | (uint128_t)0;
#endif

#warning Test for IPv4 is not implemented yet
	//ip_route_result = get_right_interface_config(&tree, test_node);
	//FIXME: Add unit test for get_left_interface_route() on IPv4 trees

	//if (ip_route_result.ip_type != IPV4) {	/* ip_type should have been propagated as is to result */
	//	FAILF("get_left_interface_route() modified ip_type field\n");
	//}
#endif	// IPV4_SUPPORT

	printf("%s: tests passed\n", __func__);
}

#ifndef USE_CPPUTEST
void unit_tests_node() {
#ifdef IPV6_SUPPORT
	test_uint128_t_to_ipv6();
#endif
	test_get_tree_prefix_len();
	test_get_hosts_prefix_len();
	test_Rmax_to_max_node_id();
#ifdef IPV6_SUPPORT
	test_ipv6_prefix_to_uint128_t_mask();
#endif
	test_node_id_to_rank();
	test_get_root_node_id();
	test_get_parent_node_id();
	test_get_left_child_node_id();
	test_get_right_child_node_id();
	test_get_top_interface_config();
	test_get_left_interface_config();
	test_get_right_interface_config();
	test_get_bottom_interface_config();
	test_get_left_right_top_bottom_interface_route();
}
#endif	// USE_CPPUTEST
