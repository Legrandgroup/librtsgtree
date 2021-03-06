#include "uint128.h"
#include "test_harness.h"
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>	// For htons()
#include <stdint.h>
#include <string.h>	// For memcpy()

#define zero_word_16bits "00000000" "00000000"

TEST_GROUP(uint128_tests) {
};

/* Unit test for U128_SET_ZERO()
 */
TEST(uint128_tests, test_zero_max_uint128_t) {
	uint128_t test_u128;

	U128_SET_MAX(test_u128);
	U128_SET_ZERO(test_u128);
#ifndef HAS_INT128
	if (test_u128.uint128_a16[0] == 0 &&
	    test_u128.uint128_a16[1] == 0 &&
	    test_u128.uint128_a16[2] == 0 &&
	    test_u128.uint128_a16[3] == 0 &&
	    test_u128.uint128_a16[4] == 0 &&
	    test_u128.uint128_a16[5] == 0 &&
	    test_u128.uint128_a16[6] == 0 &&
	    test_u128.uint128_a16[7] == 0) {
#else
	if (test_u128 == 0) {
#endif
	}
	else
		FAILF("U128_SET_ZERO() Failed\n");

	if (!U128_IS_ZERO(test_u128))
		FAILF("U128_IS_ZERO() Failed\n");

	U128_SET_MAX(test_u128);
#ifndef HAS_INT128
	if (test_u128.uint128_a16[0] == 0xffff &&
	    test_u128.uint128_a16[1] == 0xffff &&
	    test_u128.uint128_a16[2] == 0xffff &&
	    test_u128.uint128_a16[3] == 0xffff &&
	    test_u128.uint128_a16[4] == 0xffff &&
	    test_u128.uint128_a16[5] == 0xffff &&
	    test_u128.uint128_a16[6] == 0xffff &&
	    test_u128.uint128_a16[7] == 0xffff) {
#else
	if (test_u128 == (uint128_t)(-1)) {
#endif
	}
	else
		FAILF("U128_SET_MAX() Failed\n");

	if (!U128_IS_MAX(test_u128))
		FAILF("U128_IS_MAX() Failed\n");

	test_u128 = uint128_t_zero();
	if (!U128_IS_ZERO(test_u128))
		FAILF("uint128_t_zero() Failed\n");

	test_u128 = uint128_t_max();
	if (!U128_IS_MAX(test_u128))
		FAILF("uint128_t_max() Failed\n");

	NOTIFYPASS();
}

/* Unit test for uint128_t_get_byte_no()
 */

TEST(uint128_tests, test_uint128_t_get_byte_no) {
	uint128_t test_u128;
	uint8_t byte;

#ifndef HAS_INT128
	test_u128.uint128_a8[0] = 0xf0;
	test_u128.uint128_a8[1] = 0x1e;
	test_u128.uint128_a8[2] = 0x2d;
	test_u128.uint128_a8[3] = 0x3c;
	test_u128.uint128_a8[4] = 0x4b;
	test_u128.uint128_a8[5] = 0x5a;
	test_u128.uint128_a8[6] = 0x69;
	test_u128.uint128_a8[7] = 0x78;
	test_u128.uint128_a8[8] = 0x87;
	test_u128.uint128_a8[9] = 0x96;
	test_u128.uint128_a8[10] = 0xa5;
	test_u128.uint128_a8[11] = 0xb4;
	test_u128.uint128_a8[12] = 0xc3;
	test_u128.uint128_a8[13] = 0xd2;
	test_u128.uint128_a8[14] = 0xe1;
	test_u128.uint128_a8[15] = 0x0f;
#else
	test_u128 = ((uint128_t)0xf01e2d3c4b5a6978)<<64 | (uint128_t)0x8796a5b4c3d2e10f;
#endif
	if ((byte = uint128_t_get_byte_no(test_u128, 15)) != 0xf0)
		FAILF("uint128_t_get_byte_no() failed extracting byte #15: got %02x\n", byte);
	if ((byte = uint128_t_get_byte_no(test_u128, 14)) != 0x1e)
		FAILF("uint128_t_get_byte_no() failed extracting byte #14: got %02x\n", byte);
	if ((byte = uint128_t_get_byte_no(test_u128, 13)) != 0x2d)
		FAILF("uint128_t_get_byte_no() failed extracting byte #13: got %02x\n", byte);
	if ((byte = uint128_t_get_byte_no(test_u128, 12)) != 0x3c)
		FAILF("uint128_t_get_byte_no() failed extracting byte #12: got %02x\n", byte);
	if ((byte = uint128_t_get_byte_no(test_u128, 11)) != 0x4b)
		FAILF("uint128_t_get_byte_no() failed extracting byte #11: got %02x\n", byte);
	if ((byte = uint128_t_get_byte_no(test_u128, 10)) != 0x5a)
		FAILF("uint128_t_get_byte_no() failed extracting byte #10: got %02x\n", byte);
	if ((byte = uint128_t_get_byte_no(test_u128, 9)) != 0x69)
		FAILF("uint128_t_get_byte_no() failed extracting byte #9: got %02x\n", byte);
	if ((byte = uint128_t_get_byte_no(test_u128, 8)) != 0x78)
		FAILF("uint128_t_get_byte_no() failed extracting byte #8: got %02x\n", byte);
	if ((byte = uint128_t_get_byte_no(test_u128, 7)) != 0x87)
		FAILF("uint128_t_get_byte_no() failed extracting byte #7: got %02x\n", byte);
	if ((byte = uint128_t_get_byte_no(test_u128, 6)) != 0x96)
		FAILF("uint128_t_get_byte_no() failed extracting byte #6: got %02x\n", byte);
	if ((byte = uint128_t_get_byte_no(test_u128, 5)) != 0xa5)
		FAILF("uint128_t_get_byte_no() failed extracting byte #5: got %02x\n", byte);
	if ((byte = uint128_t_get_byte_no(test_u128, 4)) != 0xb4)
		FAILF("uint128_t_get_byte_no() failed extracting byte #4: got %02x\n", byte);
	if ((byte = uint128_t_get_byte_no(test_u128, 3)) != 0xc3)
		FAILF("uint128_t_get_byte_no() failed extracting byte #3: got %02x\n", byte);
	if ((byte = uint128_t_get_byte_no(test_u128, 2)) != 0xd2)
		FAILF("uint128_t_get_byte_no() failed extracting byte #2: got %02x\n", byte);
	if ((byte = uint128_t_get_byte_no(test_u128, 1)) != 0xe1)
		FAILF("uint128_t_get_byte_no() failed extracting byte #1: got %02x\n", byte);
	if ((byte = uint128_t_get_byte_no(test_u128, 0)) != 0x0f)
		FAILF("uint128_t_get_byte_no() failed extracting byte #0: got %02x\n", byte);

	NOTIFYPASS();
}

/* Unit test for uint128_t_to_binstr()
 */
TEST(uint128_tests, test_uint128_t_to_binstr) {
	uint128_t test_u128;
	char result[129];
	const char* expected_result;
	uint8_t nb_bits_missing;

	U128_SET_ZERO(test_u128);
	uint128_t_to_binstr(test_u128, 128, result);
	expected_result = zero_word_16bits /* Word 0 */ \
	                  zero_word_16bits /* Word 1 */ \
	                  zero_word_16bits /* Word 2 */ \
	                  zero_word_16bits /* Word 3 */ \
	                  zero_word_16bits /* Word 4 */ \
	                  zero_word_16bits /* Word 5 */ \
	                  zero_word_16bits /* Word 6 */ \
	                  zero_word_16bits;/* Word 7 */
	if (strcmp(result, expected_result) != 0)
		FAILF("uint128_t_to_binstr() failed, got:\n\"%s\", expected:\n\"%s\"\n", result, expected_result);

#ifndef HAS_INT128
	U128_SET_ZERO(test_u128);
	test_u128.uint128_a16[0] = htons(0xfe80);
	test_u128.uint128_a16[6] = htons(0x8000);
	test_u128.uint128_a16[7] = htons(0xa5a5);
#else
	test_u128 = ((uint128_t)0x0000fe80)<<112 | (uint128_t)0x8000a5a5;
#endif
	uint128_t_to_binstr(test_u128, 128, result);
	expected_result = "1111111010000000"    /* Word 0 */ \
                      zero_word_16bits      /* Word 1 */ \
                      zero_word_16bits      /* Word 2 */ \
                      zero_word_16bits      /* Word 3 */ \
                      zero_word_16bits      /* Word 4 */ \
                      zero_word_16bits      /* Word 5 */ \
                      "10000000" "00000000" /* Word 6 */ \
                      "10100101" "10100101";/* Word 7 */
	if (strcmp(result, expected_result) != 0)
		FAILF("uint128_t_to_binstr() failed, got:\n\"%s\", expected:\n\"%s\"\n", result, expected_result);

	/* Test printing ipv6.google.com (2a00:1450:4007:80e::200e) IPv6 address as a unit128_t */
#ifndef HAS_INT128
	U128_SET_ZERO(test_u128);
	test_u128.uint128_a16[0] = htons(0x2a00);
	test_u128.uint128_a16[1] = htons(0x1450);
	test_u128.uint128_a16[2] = htons(0x4007);
	test_u128.uint128_a16[3] = htons(0x080e);
	test_u128.uint128_a16[7] = htons(0x200e);
#else
	test_u128 = ((uint128_t)0x2a0014504007080e)<<64 | (uint128_t)0x200e;
#endif
	uint128_t_to_binstr(test_u128, 128, result);
	expected_result = "0010101000000000" /* Word 0 */ \
	                  "0001010001010000" /* Word 1 */ \
	                  "0100000000000111" /* Word 2 */ \
	                  "0000100000001110" /* Word 3 */ \
	                  zero_word_16bits   /* Word 4 */ \
	                  zero_word_16bits   /* Word 5 */ \
	                  zero_word_16bits   /* Word 6 */ \
	                  "0010000000001110";/* Word 7 */
	if (strcmp(result, expected_result) != 0)
		FAILF("uint128_t_to_binstr() failed, got:\n\"%s\", expected:\n\"%s\"\n", result, expected_result);

	/* Same 128-bit value (2a00:1450:4007:80e::200e), but assigned byte-per-byte */
#ifndef HAS_INT128	/* Assignment byte per byte is only interesting on non-native 128 arithmetic */
	U128_SET_ZERO(test_u128);
	test_u128.uint128_a8[0] = 0x2a;
	test_u128.uint128_a8[1] = 0x00;
	test_u128.uint128_a8[2] = 0x14;
	test_u128.uint128_a8[3] = 0x50;
	test_u128.uint128_a8[4] = 0x40;
	test_u128.uint128_a8[5] = 0x07;
	test_u128.uint128_a8[6] = 0x08;
	test_u128.uint128_a8[7] = 0x0e;
	test_u128.uint128_a8[14] = 0x20;
	test_u128.uint128_a8[15] = 0x0e;
	uint128_t_to_binstr(test_u128, 128, result);
	if (strcmp(result, expected_result) != 0)
		FAILF("uint128_t_to_binstr() failed, got:\n\"%s\", expected:\n\"%s\"\n", result, expected_result);
#endif

#ifndef HAS_INT128
	U128_SET_ZERO(test_u128);
	test_u128.uint128_a8[0] = 0xa5;
	test_u128.uint128_a8[1] = 0xa2;
	test_u128.uint128_a8[2] = 0x15;
	test_u128.uint128_a8[3] = 0x02;
	test_u128.uint128_a8[4] = 0x45;
	test_u128.uint128_a8[5] = 0xa8;
	test_u128.uint128_a8[6] = 0x87;
	test_u128.uint128_a8[7] = 0xc4;
	test_u128.uint128_a8[8] = 0xe5;
	test_u128.uint128_a8[9] = 0x04;
	test_u128.uint128_a8[10] = 0x1a;
	test_u128.uint128_a8[11] = 0xfe;
	test_u128.uint128_a8[12] = 0x89;
	test_u128.uint128_a8[13] = 0x9c;
	test_u128.uint128_a8[14] = 0x47;
	test_u128.uint128_a8[15] = 0x0f;
#else
	test_u128 = ((uint128_t)0xa5a2150245a887c4)<<64 | (uint128_t)0xe5041afe899c470f;
#endif
	uint128_t_to_binstr(test_u128, 128, result);
	expected_result = "1010" "0101" /* Byte 0 */ \
	                  "1010" "0010" /* Byte 1 */ \
	                  "0001" "0101" /* Byte 2 */ \
	                  "0000" "0010" /* Byte 3 */ \
	                  "0100" "0101" /* Byte 4 */ \
	                  "1010" "1000" /* Byte 5 */ \
	                  "1000" "0111" /* Byte 6 */ \
	                  "1100" "0100" /* Byte 7 */ \
	                  "1110" "0101" /* Byte 8 */ \
	                  "0000" "0100" /* Byte 9 */ \
	                  "0001" "1010" /* Byte 10 */ \
	                  "1111" "1110" /* Byte 11 */ \
	                  "1000" "1001" /* Byte 12 */ \
	                  "1001" "1100" /* Byte 13 */ \
	                  "0100" "0111" /* Byte 14 */ \
	                  "0000" "1111";/* Byte 15 */
	if (strcmp(result, expected_result) != 0)
		FAILF("uint128_t_to_binstr() failed, got:\n\"%s\", expected:\n\"%s\"\n", result, expected_result);
	/* Now dump the same value with less that 128 bits (from 127 LSB to 0 LSB), and check that the string reduces progressively to empty */
	for (nb_bits_missing=1; nb_bits_missing<=128; nb_bits_missing++) {
		uint128_t_to_binstr(test_u128, 128-nb_bits_missing, result);
		if (strcmp(result, expected_result+nb_bits_missing) != 0)
			FAILF("uint128_t_to_binstr() failed, got:\n\"%s\", expected:\n\"%s\"\n", result, expected_result+nb_bits_missing);
	}
	NOTIFYPASS();
}

/* Unit test for uint128_t_to_hexstr()
 */
TEST(uint128_tests, test_uint128_t_to_hexstr) {
	uint128_t test_u128;
	char result[33];
	const char* expected_result;
	uint8_t nb_bytes_missing;

#ifndef HAS_INT128
	U128_SET_ZERO(test_u128);
	test_u128.uint128_a8[0] = 0xa5;
	test_u128.uint128_a8[1] = 0xa2;
	test_u128.uint128_a8[2] = 0x15;
	test_u128.uint128_a8[3] = 0x02;
	test_u128.uint128_a8[4] = 0x45;
	test_u128.uint128_a8[5] = 0xa8;
	test_u128.uint128_a8[6] = 0x87;
	test_u128.uint128_a8[7] = 0xc4;
	test_u128.uint128_a8[8] = 0xe5;
	test_u128.uint128_a8[9] = 0x04;
	test_u128.uint128_a8[10] = 0x1a;
	test_u128.uint128_a8[11] = 0xfe;
	test_u128.uint128_a8[12] = 0x89;
	test_u128.uint128_a8[13] = 0x9c;
	test_u128.uint128_a8[14] = 0x47;
	test_u128.uint128_a8[15] = 0x0f;
#else
	test_u128 = ((uint128_t)0xa5a2150245a887c4)<<64 | (uint128_t)0xe5041afe899c470f;
#endif
	uint128_t_to_hexstr(test_u128, 16, result);
	expected_result = "a5a2" "1502" "45a8" "87c4" "e504" "1afe" "899c" "470f";
	if (strcmp(result, expected_result) != 0)
		FAILF("uint128_t_to_hexstr() failed, got:\n\"%s\", expected:\n\"%s\"\n", result, expected_result);

#ifndef HAS_INT128
	test_u128.uint128_a8[0] = 0x0f;
	test_u128.uint128_a8[1] = 0x1e;
	test_u128.uint128_a8[2] = 0x2d;
	test_u128.uint128_a8[3] = 0x3c;
	test_u128.uint128_a8[4] = 0x4b;
	test_u128.uint128_a8[5] = 0x5a;
	test_u128.uint128_a8[6] = 0x69;
	test_u128.uint128_a8[7] = 0x78;
	test_u128.uint128_a8[8] = 0x87;
	test_u128.uint128_a8[9] = 0x96;
	test_u128.uint128_a8[10] = 0xa5;
	test_u128.uint128_a8[11] = 0xb4;
	test_u128.uint128_a8[12] = 0xc3;
	test_u128.uint128_a8[13] = 0xd2;
	test_u128.uint128_a8[14] = 0xe1;
	test_u128.uint128_a8[15] = 0xf0;
#else
	test_u128 = ((uint128_t)0x0f1e2d3c4b5a6978)<<64 | (uint128_t)0x8796a5b4c3d2e1f0;
#endif
	uint128_t_to_hexstr(test_u128, 16, result);
	expected_result = "0f1e2d3c4b5a69788796a5b4c3d2e1f0";
	if (strcmp(result, expected_result) != 0)
		FAILF("uint128_t_to_hexstr() failed, got:\n\"%s\", expected:\n\"%s\"\n", result, expected_result);

	/* Now dump the same value with less that 16 bytes (from 15 LSB to 0 LSB), and check that the string reduces progressively to empty */
	for (nb_bytes_missing=1; nb_bytes_missing<=16; nb_bytes_missing++) {
		uint128_t_to_hexstr(test_u128, 16-nb_bytes_missing, result);
		if (strcmp(result, expected_result+nb_bytes_missing*2) != 0)	// nb_bytes_missing*2 because there are two hex digits output per byte
			FAILF("uint128_t_to_hexstr() failed, got:\n\"%s\", expected:\n\"%s\"\n", result, expected_result+nb_bytes_missing*2);
	}
	NOTIFYPASS();
}

/* Unit test for power2_to_uint128_t()
 * Note: uint8_t_to_binstr() will be covered while testing this */
TEST(uint128_tests, test_power2_to_uint128_t) {
	uint128_t test_u128;
	char result[129];
	char expected;

	uint8_t power;
	uint8_t string_pos;

	for (power = 0; power < 128; power++) {
		test_u128 = power2_to_uint128_t(power);
		uint128_t_to_binstr(test_u128, 128, result);
		for (string_pos = 0; string_pos<128; string_pos++) {
			if (127-string_pos == power) {
				expected = '1';
			}
			else {
				expected = '0';
			}
			if (result[string_pos] != expected)
				FAILF("power2_to_uint128_t() failed at pos %d, got:\n\"%c\", expected:\n\"%c\"\n", string_pos, result[string_pos], expected);
		}
	}
	if (result[128] != '\0')
		FAILF("power2_to_uint128_t() did not get a NULL termination at pos 128\n");

	NOTIFYPASS();
}

/* Unit test for uint128_t_right_shift()
 */
TEST(uint128_tests, test_uint128_t_right_shift) {
	uint128_t test_u128;
	char result[129];
	char expected;
	const char* expected_result;

	int8_t power;
	uint8_t shift;
	uint8_t string_pos;

	test_u128 = power2_to_uint128_t(127);
	for (power = 126; power >= 0; power--) {	/* We will right shift a first time before starting testing, so we expect 2^126 as first result */
		test_u128 = uint128_t_right_shift(test_u128);
		uint128_t_to_binstr(test_u128, 128, result);
		for (string_pos = 0; string_pos<128; string_pos++) {
			if (127-string_pos == power) {
				expected = '1';
			}
			else {
				expected = '0';
			}
			if (result[string_pos] != expected)
				FAILF("uint128_t_right_shift() failed at pos %d, got:\n\"%c\", expected:\n\"%c\"\n", string_pos, result[string_pos], expected);
		}
	}
	if (result[128] != '\0')
		FAILF("uint128_t_right_shift() did not get a NULL termination at pos 128\n");

#ifndef HAS_INT128
	test_u128.uint128_a8[0] = 0xf0;
	test_u128.uint128_a8[1] = 0x1e;
	test_u128.uint128_a8[2] = 0x2d;
	test_u128.uint128_a8[3] = 0x3c;
	test_u128.uint128_a8[4] = 0x4b;
	test_u128.uint128_a8[5] = 0x5a;
	test_u128.uint128_a8[6] = 0x69;
	test_u128.uint128_a8[7] = 0x78;
	test_u128.uint128_a8[8] = 0x87;
	test_u128.uint128_a8[9] = 0x96;
	test_u128.uint128_a8[10] = 0xa5;
	test_u128.uint128_a8[11] = 0xb4;
	test_u128.uint128_a8[12] = 0xc3;
	test_u128.uint128_a8[13] = 0xd2;
	test_u128.uint128_a8[14] = 0xe1;
	test_u128.uint128_a8[15] = 0x0f;
#else
	test_u128 = ((uint128_t)0xf01e2d3c4b5a6978)<<64 | (uint128_t)0x8796a5b4c3d2e10f;
#endif


	expected_result = "f01e2d3c4b5a69788796a5b4c3d2e10f";
	//uint128_t_to_hexstr(test_u128, 16, result);
	//fprintf(stderr, "Starting with \"%s\"\n", result);
	//fprintf(stderr, "Expecting \"%s\"\n", expected_result);

	for (shift = 4; shift < 128; shift+=4) {	/* We will right shift nibble per nibble in order to easily compare hex representation */
		test_u128 = uint128_t_right_shift(test_u128);
		test_u128 = uint128_t_right_shift(test_u128);
		test_u128 = uint128_t_right_shift(test_u128);
		test_u128 = uint128_t_right_shift(test_u128); /* 4 bit shift */
		uint128_t_to_hexstr(test_u128, 16, result);
		//fprintf(stderr, "At shift=%d, getting \"%s\"\n", shift, result);

		for (string_pos = 0; string_pos<32; string_pos++) {	/* Hex string: 32 characters */
			if (string_pos < shift/4) {
				expected = '0';	/* All newly inserted bits on left should lead to string '0' */
				//fprintf(stderr, "string_pos=%d, expecting '0'\n", string_pos);
			}
			else {
				expected = expected_result[string_pos-shift/4];
				//fprintf(stderr, "string_pos=%d, expected_result[%d]=='%c'\n", string_pos, string_pos-shift/4, expected);
			}
			if (result[string_pos] != expected)
				FAILF("uint128_t_right_shift() failed at pos %d, got:\n\"%c\", expected:\n\"%c\"\n", string_pos, result[string_pos], expected);
		}
	}
	NOTIFYPASS();
}

/* Unit test for uint128_t_right_shift_n()
 */
TEST(uint128_tests, test_uint128_t_right_shift_n) {

	uint128_t test_u128;
	char result[129];
	const char* expected_result;

#ifndef HAS_INT128
	test_u128.uint128_a8[0] = 0xf0;
	test_u128.uint128_a8[1] = 0x1e;
	test_u128.uint128_a8[2] = 0x2d;
	test_u128.uint128_a8[3] = 0x3c;
	test_u128.uint128_a8[4] = 0x4b;
	test_u128.uint128_a8[5] = 0x5a;
	test_u128.uint128_a8[6] = 0x69;
	test_u128.uint128_a8[7] = 0x78;
	test_u128.uint128_a8[8] = 0x87;
	test_u128.uint128_a8[9] = 0x96;
	test_u128.uint128_a8[10] = 0xa5;
	test_u128.uint128_a8[11] = 0xb4;
	test_u128.uint128_a8[12] = 0xc3;
	test_u128.uint128_a8[13] = 0xd2;
	test_u128.uint128_a8[14] = 0xe1;
	test_u128.uint128_a8[15] = 0x0f;
#else
	test_u128 = ((uint128_t)0xf01e2d3c4b5a6978)<<64 | (uint128_t)0x8796a5b4c3d2e10f;
#endif


	test_u128 = uint128_t_right_shift_n(test_u128, 0);	/* No shift */
	uint128_t_to_hexstr(test_u128, 16, result);
	expected_result = "f01e2d3c4b5a69788796a5b4c3d2e10f";
	if (strcmp(result, expected_result) != 0)
		FAILF("uint128_t_right_shift_n() failed, got:\n\"%s\", expected:\n\"%s\"\n", result, expected_result);

	test_u128 = uint128_t_right_shift_n(test_u128, 4);
	uint128_t_to_hexstr(test_u128, 16, result);
	expected_result = "0f01e2d3c4b5a69788796a5b4c3d2e10";
	if (strcmp(result, expected_result) != 0)
		FAILF("uint128_t_right_shift_n() failed, got:\n\"%s\", expected:\n\"%s\"\n", result, expected_result);

	test_u128 = uint128_t_right_shift_n(test_u128, 8);
	uint128_t_to_hexstr(test_u128, 16, result);
	expected_result = "000f01e2d3c4b5a69788796a5b4c3d2e";
	if (strcmp(result, expected_result) != 0)
		FAILF("uint128_t_right_shift_n() failed, got:\n\"%s\", expected:\n\"%s\"\n", result, expected_result);

	test_u128 = uint128_t_right_shift_n(test_u128, 16);
	uint128_t_to_hexstr(test_u128, 16, result);
	expected_result = "0000000f01e2d3c4b5a69788796a5b4c";
	if (strcmp(result, expected_result) != 0)
		FAILF("uint128_t_right_shift_n() failed, got:\n\"%s\", expected:\n\"%s\"\n", result, expected_result);

	test_u128 = uint128_t_right_shift_n(test_u128, 32);
	uint128_t_to_hexstr(test_u128, 16, result);
	expected_result = "000000000000000f01e2d3c4b5a69788";
	if (strcmp(result, expected_result) != 0)
		FAILF("uint128_t_right_shift_n() failed, got:\n\"%s\", expected:\n\"%s\"\n", result, expected_result);

	U128_SET_MAX(test_u128);	/* All bits set to 1 */
	test_u128 = uint128_t_right_shift_n(test_u128, 127);
	uint128_t_to_hexstr(test_u128, 16, result);
	expected_result = "00000000000000000000000000000001";
	if (strcmp(result, expected_result) != 0)
		FAILF("uint128_t_right_shift_n() failed, got:\n\"%s\", expected:\n\"%s\"\n", result, expected_result);


	U128_SET_MAX(test_u128);	/* All bits set to 1 */
	test_u128 = uint128_t_right_shift_n(test_u128, 128);
	uint128_t_to_hexstr(test_u128, 16, result);
	expected_result = "00000000000000000000000000000000";
	if (strcmp(result, expected_result) != 0)
		FAILF("uint128_t_right_shift_n() failed, got:\n\"%s\", expected:\n\"%s\"\n", result, expected_result);

	NOTIFYPASS();
}

/* Unit test for uint128_t_left_shift()
 */
TEST(uint128_tests, test_uint128_t_left_shift) {
	uint128_t test_u128;
	char result[129];
	char expected;
	const char* expected_result;

	uint8_t power;
	uint8_t shift;
	uint8_t string_pos;

	test_u128 = power2_to_uint128_t(0);	/* This means value = 1 */
	for (power = 1; power < 128; power++) {	/* We will left shift a first time before starting testing, so we expect 2^2 as first result */
		test_u128 = uint128_t_left_shift(test_u128);
		uint128_t_to_binstr(test_u128, 128, result);
		for (string_pos = 0; string_pos<128; string_pos++) {
			if (127-string_pos == power) {
				expected = '1';
			}
			else {
				expected = '0';
			}
			if (result[string_pos] != expected)
				FAILF("uint128_t_left_shift() failed at pos %d, got:\n\"%c\", expected:\n\"%c\"\n", string_pos, result[string_pos], expected);
		}
	}
	if (result[128] != '\0')
		FAILF("uint128_t_left_shift() did not get a NULL termination at pos 128\n");

#ifndef HAS_INT128
	test_u128.uint128_a8[0] = 0xf0;
	test_u128.uint128_a8[1] = 0x1e;
	test_u128.uint128_a8[2] = 0x2d;
	test_u128.uint128_a8[3] = 0x3c;
	test_u128.uint128_a8[4] = 0x4b;
	test_u128.uint128_a8[5] = 0x5a;
	test_u128.uint128_a8[6] = 0x69;
	test_u128.uint128_a8[7] = 0x78;
	test_u128.uint128_a8[8] = 0x87;
	test_u128.uint128_a8[9] = 0x96;
	test_u128.uint128_a8[10] = 0xa5;
	test_u128.uint128_a8[11] = 0xb4;
	test_u128.uint128_a8[12] = 0xc3;
	test_u128.uint128_a8[13] = 0xd2;
	test_u128.uint128_a8[14] = 0xe1;
	test_u128.uint128_a8[15] = 0x0f;
#else
	test_u128 = ((uint128_t)0xf01e2d3c4b5a6978)<<64 | (uint128_t)0x8796a5b4c3d2e10f;
#endif


	expected_result = "f01e2d3c4b5a69788796a5b4c3d2e10f";
	//uint128_t_to_hexstr(test_u128, 16, result);
	//fprintf(stderr, "Starting with \"%s\"\n", result);
	//fprintf(stderr, "Expecting \"%s\"\n", expected_result);

	for (shift = 4; shift < 128; shift+=4) {	/* We will left shift nibble per nibble in order to easily compare hex representation */
		test_u128 = uint128_t_left_shift(test_u128);
		test_u128 = uint128_t_left_shift(test_u128);
		test_u128 = uint128_t_left_shift(test_u128);
		test_u128 = uint128_t_left_shift(test_u128); /* 4 bit shift */
		uint128_t_to_hexstr(test_u128, 16, result);
		//fprintf(stderr, "At shift=%d, getting \"%s\"\n", shift, result);

		for (string_pos = 0; string_pos<32; string_pos++) {	/* Hex string: 32 characters */
			if (string_pos > 31-shift/4) {
				expected = '0';	/* All newly inserted bits on left should lead to string '0' */
				//fprintf(stderr, "string_pos=%d, expecting '0'\n", string_pos);
			}
			else {
				expected = expected_result[string_pos+shift/4];
				//fprintf(stderr, "string_pos=%d, expected_result[%d]=='%c'\n", string_pos, string_pos+shift/4, expected);
			}
			if (result[string_pos] != expected)
				FAILF("uint128_t_right_shift() failed at pos %d, got:\n\"%c\", expected:\n\"%c\"\n", string_pos, result[string_pos], expected);
		}
	}

	NOTIFYPASS();
}

/* Unit test for uint128_t_left_shift_n()
 */
TEST(uint128_tests, test_uint128_t_left_shift_n) {

	uint128_t test_u128;
	char result[129];
	const char* expected_result;

#ifndef HAS_INT128
	test_u128.uint128_a8[0] = 0xf0;
	test_u128.uint128_a8[1] = 0x1e;
	test_u128.uint128_a8[2] = 0x2d;
	test_u128.uint128_a8[3] = 0x3c;
	test_u128.uint128_a8[4] = 0x4b;
	test_u128.uint128_a8[5] = 0x5a;
	test_u128.uint128_a8[6] = 0x69;
	test_u128.uint128_a8[7] = 0x78;
	test_u128.uint128_a8[8] = 0x87;
	test_u128.uint128_a8[9] = 0x96;
	test_u128.uint128_a8[10] = 0xa5;
	test_u128.uint128_a8[11] = 0xb4;
	test_u128.uint128_a8[12] = 0xc3;
	test_u128.uint128_a8[13] = 0xd2;
	test_u128.uint128_a8[14] = 0xe1;
	test_u128.uint128_a8[15] = 0x0f;
#else
	test_u128 = ((uint128_t)0xf01e2d3c4b5a6978)<<64 | (uint128_t)0x8796a5b4c3d2e10f;
#endif


	test_u128 = uint128_t_left_shift_n(test_u128, 0);	/* No shift */
	uint128_t_to_hexstr(test_u128, 16, result);
	expected_result = "f01e2d3c4b5a69788796a5b4c3d2e10f";
	if (strcmp(result, expected_result) != 0)
		FAILF("uint128_t_left_shift_n() failed, got:\n\"%s\", expected:\n\"%s\"\n", result, expected_result);

	test_u128 = uint128_t_left_shift_n(test_u128, 4);
	uint128_t_to_hexstr(test_u128, 16, result);
	expected_result = "01e2d3c4b5a69788796a5b4c3d2e10f0";
	if (strcmp(result, expected_result) != 0)
		FAILF("uint128_t_left_shift_n() failed, got:\n\"%s\", expected:\n\"%s\"\n", result, expected_result);

	test_u128 = uint128_t_left_shift_n(test_u128, 8);
	uint128_t_to_hexstr(test_u128, 16, result);
	expected_result = "e2d3c4b5a69788796a5b4c3d2e10f000";
	if (strcmp(result, expected_result) != 0)
		FAILF("uint128_t_left_shift_n() failed, got:\n\"%s\", expected:\n\"%s\"\n", result, expected_result);

	test_u128 = uint128_t_left_shift_n(test_u128, 16);
	uint128_t_to_hexstr(test_u128, 16, result);
	expected_result = "c4b5a69788796a5b4c3d2e10f0000000";
	if (strcmp(result, expected_result) != 0)
		FAILF("uint128_t_left_shift_n() failed, got:\n\"%s\", expected:\n\"%s\"\n", result, expected_result);

	test_u128 = uint128_t_left_shift_n(test_u128, 32);
	uint128_t_to_hexstr(test_u128, 16, result);
	expected_result = "88796a5b4c3d2e10f000000000000000";
	if (strcmp(result, expected_result) != 0)
		FAILF("uint128_t_left_shift_n() failed, got:\n\"%s\", expected:\n\"%s\"\n", result, expected_result);

	U128_SET_MAX(test_u128);	/* All bits set to 1 */
	test_u128 = uint128_t_left_shift_n(test_u128, 127);
	uint128_t_to_hexstr(test_u128, 16, result);
	expected_result = "80000000000000000000000000000000";
	if (strcmp(result, expected_result) != 0)
		FAILF("uint128_t_left_shift_n() failed, got:\n\"%s\", expected:\n\"%s\"\n", result, expected_result);

	U128_SET_MAX(test_u128);	/* All bits set to 1 */
	test_u128 = uint128_t_left_shift_n(test_u128, 128);
	uint128_t_to_hexstr(test_u128, 16, result);
	expected_result = "00000000000000000000000000000000";
	if (strcmp(result, expected_result) != 0)
		FAILF("uint128_t_left_shift_n() failed, got:\n\"%s\", expected:\n\"%s\"\n", result, expected_result);

	NOTIFYPASS();
}

/* Unit test for mixed uint128_t_left_shift() and uint128_t_right_shift()
 */
TEST(uint128_tests, test_uint128_t_mix_shift) {
	uint128_t test1_u128;
	uint128_t test2_u128;
	char result1[129];
	char result2[129];
	char expected;
	char expected_result[129];

	int i;
	uint8_t shift;
	uint8_t string_pos;

#ifndef HAS_INT128
	test1_u128.uint128_a8[0] = 0xf0;
	test1_u128.uint128_a8[1] = 0x1e;
	test1_u128.uint128_a8[2] = 0x2d;
	test1_u128.uint128_a8[3] = 0x3c;
	test1_u128.uint128_a8[4] = 0x4b;
	test1_u128.uint128_a8[5] = 0x5a;
	test1_u128.uint128_a8[6] = 0x69;
	test1_u128.uint128_a8[7] = 0x78;
	test1_u128.uint128_a8[8] = 0x87;
	test1_u128.uint128_a8[9] = 0x96;
	test1_u128.uint128_a8[10] = 0xa5;
	test1_u128.uint128_a8[11] = 0xb4;
	test1_u128.uint128_a8[12] = 0xc3;
	test1_u128.uint128_a8[13] = 0xd2;
	test1_u128.uint128_a8[14] = 0xe1;
	test1_u128.uint128_a8[15] = 0x0f;
#else
	test1_u128 = ((uint128_t)0xf01e2d3c4b5a6978)<<64 | (uint128_t)0x8796a5b4c3d2e10f;
#endif

	test2_u128 = test1_u128;	/* Copy test1 into test2 */

	uint128_t_to_binstr(test1_u128, 128, expected_result);
	//uint128_t_to_hexstr(test_u128, 16, result);
	//fprintf(stderr, "Starting with \"%s\"\n", result);
	//fprintf(stderr, "Expecting \"%s\"\n", expected_result);

	for (shift = 0; shift < 128; shift++) {	/* We will left shift nibble per nibble in order to easily compare hex representation */
		for (i = 0; i < shift; i++)	// Left shift "shift" times
			test1_u128 = uint128_t_left_shift(test1_u128);
		for (i = 0; i < shift; i++)	// Right shift "shift" times
			test1_u128 = uint128_t_right_shift(test1_u128);
		uint128_t_to_binstr(test1_u128, 128, result1);
		//fprintf(stderr, "At shift=%d, getting result1=\"%s\"\n", shift, result1);
		for (i = 0; i < shift; i++)	// Right shift "shift" times
			test2_u128 = uint128_t_right_shift(test2_u128);
		for (i = 0; i < shift; i++)	// left shift "shift" times
			test2_u128 = uint128_t_left_shift(test2_u128);
		uint128_t_to_binstr(test2_u128, 128, result2);
		//fprintf(stderr, "At shift=%d, getting result2=\"%s\"\n", shift, result2);

		for (string_pos = 0; string_pos<128; string_pos++) {	/* Hex string: 32 characters */
			if (string_pos < shift) {
				expected = '0';	/* All left bits that we taken off test1_u128 by shift overflow should be set to '0' */
				//fprintf(stderr, "expecting result1[%d]='0' (removed)\n", string_pos);
			}
			else {
				expected = expected_result[string_pos];
				//fprintf(stderr, "expecting result1[%d]='%c'\n", string_pos, expected);
			}
			if (result1[string_pos] != expected)
				FAILF("uint128_t_right_shift() failed on result1 at pos %d, got:\n\"%c\", expected:\n\"%c\"\n", string_pos, result1[string_pos], expected);

			if (string_pos > 127-shift) {
				expected = '0';	/* All right bits that we taken off test2_u128 by shift overflow should be set to '0' */
				//fprintf(stderr, "expecting result2[%d]='0' (removed)\n", string_pos);
			}
			else {
				expected = expected_result[string_pos];
				//fprintf(stderr, "expecting result2[%d]='%c'\n", string_pos, expected);
			}
			if (result2[string_pos] != expected)
				FAILF("uint128_t_right_shift() failed on result2 at pos %d, got:\n\"%c\", expected:\n\"%c\"\n", string_pos, result1[string_pos], expected);
		}
	}
	NOTIFYPASS();
}

/* Unit test for uint128_t_inc()
 */
TEST(uint128_tests, test_uint128_t_inc) {
	uint128_t test_u128;
	char result[33];
	const char* expected_result;

	uint16_t inc_count;
	uint16_t last_word16;

	U128_SET_ZERO(test_u128);
	for (inc_count = 0; inc_count != 65535; inc_count++) {
		test_u128 = uint128_t_inc(test_u128);
		//uint128_t_to_hexstr(test_u128, 16, result);
		//printf("Value: %s\n", result);
#ifndef HAS_INT128
		last_word16 = ntohs(test_u128.uint128_a16[7]);
#else
		last_word16 = test_u128 & 0xffff;
#endif

		if (last_word16-1 != inc_count)
			FAILF("uint128_t_inc() failed at value %u: got %u\n", inc_count, last_word16);
	}
	test_u128 = uint128_t_inc(test_u128);	/* Test next increment... will overflow last 16 bits, and also increment bit 16 */
	uint128_t_to_hexstr(test_u128, 16, result);
	expected_result = "00000000000000000000000000010000";
	if (strcmp(result, expected_result) != 0)
		FAILF("uint128_t_inc() failed, got:\n\"%s\", expected:\n\"%s\"\n", result, expected_result);

	U128_SET_MAX(test_u128);

	expected_result = "00000000000000000000000000000000";
	test_u128 = uint128_t_inc(test_u128);
	uint128_t_to_hexstr(test_u128, 16, result);
	//printf("Overflow lead to \"%s\"\n", result);
	//printf("Expecting \"%s\"\n", expected_result);

	if (strcmp(result, expected_result) != 0)
		FAILF("uint128_t_inc() failed, got:\n\"%s\", expected:\n\"%s\"\n", result, expected_result);

	NOTIFYPASS();
}

/* Unit test for uint128_t_dec()
 */
TEST(uint128_tests, test_uint128_t_dec) {
	uint128_t test_u128;
	char result[33];
	const char* expected_result;

	uint16_t dec_count;
	uint16_t last_word16;

	U128_SET_MAX(test_u128);
	for (dec_count = 0; dec_count != 65535; dec_count++) {
		test_u128 = uint128_t_dec(test_u128);
		//uint128_t_to_hexstr(test_u128, 16, result);
		//printf("Value: %s\n", result);
#ifndef HAS_INT128
		last_word16 = ntohs(test_u128.uint128_a16[7]);
#else
		last_word16 = test_u128 & 0xffff;
#endif
		if (last_word16+1 != 65535-dec_count)
			FAILF("uint128_t_dec() failed at value %u: got %u\n", dec_count, last_word16);
	}
	test_u128 = uint128_t_dec(test_u128);	/* Test next decrement... will overflow last 16 bits, and also decrement bit 16 */
	uint128_t_to_hexstr(test_u128, 16, result);
	expected_result = "fffffffffffffffffffffffffffeffff";
	if (strcmp(result, expected_result) != 0)
		FAILF("uint128_t_dec() failed, got:\n\"%s\", expected:\n\"%s\"\n", result, expected_result);

	U128_SET_ZERO(test_u128);

	expected_result = "ffffffffffffffffffffffffffffffff";
	test_u128 = uint128_t_dec(test_u128);
	uint128_t_to_hexstr(test_u128, 16, result);
	//printf("Overflow lead to \"%s\"\n", result);
	//printf("Expecting \"%s\"\n", expected_result);

	if (strcmp(result, expected_result) != 0)
		FAILF("uint128_t_dec() failed, got:\n\"%s\", expected:\n\"%s\"\n", result, expected_result);

	NOTIFYPASS();
}

/* Unit test for uint16_t_to_uint128_t()
 */
TEST(uint128_tests, test_uint16_t_to_uint128_t) {
	uint16_t source;
	uint16_t last_word16;
	uint128_t result_u128;

	for (source = 0; source != 65535; source++) {
		result_u128 = uint16_t_to_uint128_t(source);
		//uint128_t_to_hexstr(result_u128, 16, result);
		//printf("Value: %s\n", result);
#ifndef HAS_INT128
		last_word16 = ntohs(result_u128.uint128_a16[7]);
#else
		last_word16 = result_u128 & 0xffff;
#endif
		if (last_word16 != source)
			FAILF("uint16_t_to_uint128_t() failed on source value %u\n", source);

#ifndef HAS_INT128
		if (result_u128.uint128_a16[0] != 0 ||	/* All other words (0 to 6) should remain 0 */
			result_u128.uint128_a16[1] != 0 ||
			result_u128.uint128_a16[2] != 0 ||
			result_u128.uint128_a16[3] != 0 ||
			result_u128.uint128_a16[4] != 0 ||
			result_u128.uint128_a16[5] != 0 ||
			result_u128.uint128_a16[6] != 0) {
#else
		if ((result_u128 & ((uint128_t)0xffffffffffffff << 64)) != 0) {
#endif
			FAILF("uint16_t_to_uint128_t() failed on source value %u: MSW have been altered\n", source);
		}
	}
	result_u128 = uint16_t_to_uint128_t(source);	/* Do the test again on maximum uint16_t (0xffff) */
	//uint128_t_to_hexstr(result_u128, 16, result);
	//printf("Value: %s\n", result);
#ifndef HAS_INT128
	last_word16 = ntohs(result_u128.uint128_a16[7]);
#else
	last_word16 = result_u128 & 0xffff;
#endif
	if (last_word16 != source)
		FAILF("uint16_t_to_uint128_t() failed on source value %u\n", source);

#ifndef HAS_INT128
	if (result_u128.uint128_a16[0] != 0 ||	/* All other words (0 to 6) should remain 0 */
		result_u128.uint128_a16[1] != 0 ||
		result_u128.uint128_a16[2] != 0 ||
		result_u128.uint128_a16[3] != 0 ||
		result_u128.uint128_a16[4] != 0 ||
		result_u128.uint128_a16[5] != 0 ||
		result_u128.uint128_a16[6] != 0) {
#else
	if ((result_u128 & ((uint128_t)0xffffffffffffff << 64)) != 0) {
#endif
		FAILF("uint16_t_to_uint128_t() failed on source value %u: MSW have been altered\n", source);
	}
}

/* Unit test for uint8_t_to_uint128_t()
 */
TEST(uint128_tests, test_uint8_t_to_uint128_t) {
	uint8_t source;
	uint8_t last_byte8;
	uint128_t result_u128;

	for (source = 0; source != (uint8_t)(-1); source++) {
		result_u128 = uint8_t_to_uint128_t(source);
		//uint128_t_to_hexstr(result_u128, 16, result);
		//printf("Value: %s\n", result);
#ifndef HAS_INT128
		last_byte8 = result_u128.uint128_a8[15];
#else
		last_byte8 = result_u128 & 0xff;
#endif
		if (last_byte8 != source) {
			FAILF("uint8_t_to_uint128_t() failed on source value %u\n", source);
		}
#ifndef HAS_INT128
		if (result_u128.uint128_a8[0] != 0 ||	/* All other bytes (0 to 14) should remain 0 */
			result_u128.uint128_a8[1] != 0 ||
			result_u128.uint128_a8[2] != 0 ||
			result_u128.uint128_a8[3] != 0 ||
			result_u128.uint128_a8[4] != 0 ||
			result_u128.uint128_a8[5] != 0 ||
			result_u128.uint128_a8[6] != 0 ||
			result_u128.uint128_a8[7] != 0 ||
			result_u128.uint128_a8[8] != 0 ||
			result_u128.uint128_a8[9] != 0 ||
			result_u128.uint128_a8[10] != 0 ||
			result_u128.uint128_a8[11] != 0 ||
			result_u128.uint128_a8[12] != 0 ||
			result_u128.uint128_a8[13] != 0 ||
			result_u128.uint128_a8[14]) {
#else
		if ( (result_u128 & ((uint128_t)0xffffffffffffff << 64)) != 0 ||	/* High 64 bits should be 0 */
		     (result_u128 & (uint128_t)0xffffffffffff00) != 0) {	/* Next 56 bits should be 0, (ignore low 8 bits) */
#endif
			FAILF("uint8_t_to_uint128_t() failed on source value %u: MSW have been altered\n", source);
		}
	}
	result_u128 = uint8_t_to_uint128_t(source);	/* Do the test again on maximum uint8_t (0xff) */
	//uint128_t_to_hexstr(result_u128, 16, result);
	//printf("Value: %s\n", result);
#ifndef HAS_INT128
	last_byte8 = result_u128.uint128_a8[15];
#else
	last_byte8 = result_u128 & 0xff;
#endif
	if (last_byte8 != source)
		FAILF("uint8_t_to_uint128_t() failed on source value %u\n", source);

#ifndef HAS_INT128
	if (result_u128.uint128_a8[0] != 0 ||	/* All other bytes (0 to 14) should remain 0 */
		result_u128.uint128_a8[1] != 0 ||
		result_u128.uint128_a8[2] != 0 ||
		result_u128.uint128_a8[3] != 0 ||
		result_u128.uint128_a8[4] != 0 ||
		result_u128.uint128_a8[5] != 0 ||
		result_u128.uint128_a8[6] != 0 ||
		result_u128.uint128_a8[7] != 0 ||
		result_u128.uint128_a8[8] != 0 ||
		result_u128.uint128_a8[9] != 0 ||
		result_u128.uint128_a8[10] != 0 ||
		result_u128.uint128_a8[11] != 0 ||
		result_u128.uint128_a8[12] != 0 ||
		result_u128.uint128_a8[13] != 0 ||
		result_u128.uint128_a8[14]) {
#else
		if ( (result_u128 & ((uint128_t)0xffffffffffffff << 64)) != 0 ||	/* High 64 bits should be 0 */
		     (result_u128 & (uint128_t)0xffffffffffff00) != 0) {	/* Next 56 bits should be 0, (ignore low 8 bits) */
#endif
		FAILF("uint8_t_to_uint128_t() failed on source value %u: MSW have been altered\n", source);
	}
}

/* Unit test for uint128_t_sub()
 */
TEST(uint128_tests, test_uint128_t_sub) {
	uint128_t test1_u128;
	uint128_t test2_u128;
	uint128_t result_u128;
	char result[33];
	const char* expected_result;

	uint16_t dec_count;
	uint16_t last_word16;

	for (dec_count = 0; dec_count != 65535; dec_count++) {
		U128_SET_MAX(test1_u128);
		result_u128 = uint128_t_sub(test1_u128, uint16_t_to_uint128_t(dec_count));
		//uint128_t_to_hexstr(result_u128, 16, result);
		//printf("Value: %s\n", result);
#ifndef HAS_INT128
		last_word16 = ntohs(result_u128.uint128_a16[7]);
#else
		last_word16 = result_u128 & 0xffff;
#endif
		if (last_word16 != 65535-dec_count)
			FAILF("uint128_t_sub() failed at iteration %u: got %u, expected %u\n", dec_count, last_word16, 65535-dec_count);

#ifndef HAS_INT128
		if (result_u128.uint128_a16[0] != test1_u128.uint128_a16[0] ||	/* All other words (0 to 6) should remain unchanged */
			result_u128.uint128_a16[1] != test1_u128.uint128_a16[1] ||
			result_u128.uint128_a16[2] != test1_u128.uint128_a16[2] ||
			result_u128.uint128_a16[3] != test1_u128.uint128_a16[3] ||
			result_u128.uint128_a16[4] != test1_u128.uint128_a16[4] ||
			result_u128.uint128_a16[5] != test1_u128.uint128_a16[5] ||
			result_u128.uint128_a16[6] != test1_u128.uint128_a16[6]) {
#else
		if (result_u128 >> 64 != test1_u128 >> 64) {
#endif
			FAILF("uint128_t_sub() failed at iteration %u: MSW have been altered\n", dec_count);
		}
	}
	result_u128 = uint128_t_dec(result_u128);	/* Now result_u128=0xffffffffffffffffffffffffffff0000 */

	/* Now test subtracting on byte index 13 (bit 16) */
	result_u128 = uint128_t_sub(result_u128, uint128_t_inc(uint16_t_to_uint128_t(0xffff)));
	uint128_t_to_hexstr(result_u128, 16, result);
	expected_result = "fffffffffffffffffffffffffffe0000";
	if (strcmp(result, expected_result) != 0)
		FAILF("uint128_t_sub() failed, got:\n\"%s\", expected:\n\"%s\"\n", result, expected_result);

	result_u128 = uint128_t_sub(result_u128, power2_to_uint128_t(23));
	uint128_t_to_hexstr(result_u128, 16, result);
	expected_result = "ffffffffffffffffffffffffff7e0000";
	if (strcmp(result, expected_result) != 0)
		FAILF("uint128_t_sub() failed, got:\n\"%s\", expected:\n\"%s\"\n", result, expected_result);

	/* Subtracting while propagating a carry to bit 63 */
	result_u128 = uint128_t_sub(result_u128, uint128_t_dec(power2_to_uint128_t(64)));
	uint128_t_to_hexstr(result_u128, 16, result);
	expected_result = "fffffffffffffffeffffffffff7e0001";
	if (strcmp(result, expected_result) != 0)
		FAILF("uint128_t_sub() failed, got:\n\"%s\", expected:\n\"%s\"\n", result, expected_result);

	result_u128 = uint128_t_dec(uint128_t_sub(result_u128, uint128_t_dec(power2_to_uint128_t(120))));
	/* (result_u128 - (2^120 - 1)) - 1 = result_u128 - 2^120 (bit 120 set to 0) */
	uint128_t_to_hexstr(result_u128, 16, result);
	expected_result = "fefffffffffffffeffffffffff7e0001";
	if (strcmp(result, expected_result) != 0)
		FAILF("uint128_t_sub() failed, got:\n\"%s\", expected:\n\"%s\"\n", result, expected_result);

	U128_SET_ZERO(test1_u128);
	test1_u128 = uint128_t_dec(test1_u128);	/* test1_u128 = (uint128_t)-1 */

	U128_SET_ZERO(test2_u128);
	result_u128 = uint128_t_dec(uint128_t_sub(test2_u128, test1_u128));	/* Sub -1, thus +1, then decrement, should get 0 */
	uint128_t_to_hexstr(result_u128, 16, result);

	expected_result = "00000000000000000000000000000000";
	if (strcmp(result, expected_result) != 0)
		FAILF("uint128_t_sub() failed, got:\n\"%s\", expected:\n\"%s\"\n", result, expected_result);

#ifndef HAS_INT128
	test1_u128.uint128_a8[0] = 0x12;
	test1_u128.uint128_a8[1] = 0x34;
	test1_u128.uint128_a8[2] = 0x56;
	test1_u128.uint128_a8[3] = 0x78;
	test1_u128.uint128_a8[4] = 0x9a;
	test1_u128.uint128_a8[5] = 0xbc;
	test1_u128.uint128_a8[6] = 0xde;
	test1_u128.uint128_a8[7] = 0xf0;
	test1_u128.uint128_a8[8] = 0x12;
	test1_u128.uint128_a8[9] = 0x34;
	test1_u128.uint128_a8[10] = 0x56;
	test1_u128.uint128_a8[11] = 0x78;
	test1_u128.uint128_a8[12] = 0x9a;
	test1_u128.uint128_a8[13] = 0xbc;
	test1_u128.uint128_a8[14] = 0xde;
	test1_u128.uint128_a8[15] = 0xf0;

	test2_u128.uint128_a8[0] = 0xf0;
	test2_u128.uint128_a8[1] = 0x1e;
	test2_u128.uint128_a8[2] = 0x2d;
	test2_u128.uint128_a8[3] = 0x3c;
	test2_u128.uint128_a8[4] = 0x4b;
	test2_u128.uint128_a8[5] = 0x5a;
	test2_u128.uint128_a8[6] = 0x69;
	test2_u128.uint128_a8[7] = 0x78;
	test2_u128.uint128_a8[8] = 0x87;
	test2_u128.uint128_a8[9] = 0x96;
	test2_u128.uint128_a8[10] = 0xa5;
	test2_u128.uint128_a8[11] = 0xb4;
	test2_u128.uint128_a8[12] = 0xc3;
	test2_u128.uint128_a8[13] = 0xd2;
	test2_u128.uint128_a8[14] = 0xe1;
	test2_u128.uint128_a8[15] = 0x0f;
#else
	test1_u128 = ((uint128_t)0x123456789abcdef0)<<64 | (uint128_t)0x123456789abcdef0;
	test2_u128 = ((uint128_t)0xf01e2d3c4b5a6978)<<64 | (uint128_t)0x8796a5b4c3d2e10f;
#endif

	expected_result = "2216293c4f6275778a9db0c3d6e9fde1";	/* echo 'print "%x" % (0xff123456789abcdef0123456789abcdef0 - 0xf01e2d3c4b5a69788796a5b4c3d2e10f)' | python */
	result_u128 = uint128_t_sub(test1_u128, test2_u128);
	uint128_t_to_hexstr(result_u128, 16, result);

	if (strcmp(result, expected_result) != 0)
		FAILF("uint128_t_sub() failed, got:\n\"%s\", expected:\n\"%s\"\n", result, expected_result);

	U128_SET_ZERO(test1_u128);
	result_u128 = uint128_t_sub(result_u128, test1_u128);

	uint128_t_to_hexstr(result_u128, 16, result);	/* substracting 0, should get same value as before */

	if (strcmp(result, expected_result) != 0)
		FAILF("uint128_t_sub() failed, got:\n\"%s\", expected:\n\"%s\"\n", result, expected_result);

	NOTIFYPASS();
}

/* Unit test for uint128_t_add()
 */
TEST(uint128_tests, test_uint128_t_add) {
	uint128_t test1_u128;
	uint128_t test2_u128;
	uint128_t result_u128;
	char result[33];
	const char* expected_result;

	uint16_t inc_count;
	uint16_t last_word16;

	for (inc_count = 0; inc_count != 65535; inc_count++) {
		U128_SET_ZERO(test1_u128);
		result_u128 = uint128_t_add(test1_u128, uint16_t_to_uint128_t(inc_count));
		//uint128_t_to_hexstr(result_u128, 16, result);
		//printf("Value: %s\n", result);
#ifndef HAS_INT128
		last_word16 = ntohs(result_u128.uint128_a16[7]);
#else
		last_word16 = result_u128 & 0xffff;
#endif
		if (last_word16 != inc_count)
			FAILF("uint128_t_add() failed at iteration %u: got %u, expected %u\n", inc_count, last_word16, inc_count);

#ifndef HAS_INT128
		if (result_u128.uint128_a16[0] != test1_u128.uint128_a16[0] ||	/* All other words (0 to 6) should remain unchanged */
			result_u128.uint128_a16[1] != test1_u128.uint128_a16[1] ||
			result_u128.uint128_a16[2] != test1_u128.uint128_a16[2] ||
			result_u128.uint128_a16[3] != test1_u128.uint128_a16[3] ||
			result_u128.uint128_a16[4] != test1_u128.uint128_a16[4] ||
			result_u128.uint128_a16[5] != test1_u128.uint128_a16[5] ||
			result_u128.uint128_a16[6] != test1_u128.uint128_a16[6]) {
#else
	   if (result_u128 >> 64 != test1_u128 >> 64) {
#endif
			FAILF("uint128_t_add() failed at iteration %u: MSW have been altered\n", inc_count);
		}
	}
	result_u128 = uint128_t_inc(result_u128);	/* Now result_u128=0x0000000000000000000000000000ffff */

	/* Now test adding on byte index 13 (bit 16) */
	result_u128 = uint128_t_add(result_u128, uint128_t_inc(uint16_t_to_uint128_t(0xffff)));
	uint128_t_to_hexstr(result_u128, 16, result);
	expected_result = "0000000000000000000000000001ffff";
	if (strcmp(result, expected_result) != 0)
		FAILF("uint128_t_add() failed, got:\n\"%s\", expected:\n\"%s\"\n", result, expected_result);

	result_u128 = uint128_t_add(result_u128, power2_to_uint128_t(23));
	uint128_t_to_hexstr(result_u128, 16, result);
	expected_result = "0000000000000000000000000081ffff";
	if (strcmp(result, expected_result) != 0)
		FAILF("uint128_t_add() failed, got:\n\"%s\", expected:\n\"%s\"\n", result, expected_result);

	/* Adding while propagating a carry to bit 63 */
	result_u128 = uint128_t_add(result_u128, uint128_t_dec(power2_to_uint128_t(64)));
	uint128_t_to_hexstr(result_u128, 16, result);
	expected_result = "0000000000000001000000000081fffe";
	if (strcmp(result, expected_result) != 0)
		FAILF("uint128_t_add() failed, got:\n\"%s\", expected:\n\"%s\"\n", result, expected_result);

	result_u128 = uint128_t_inc(uint128_t_add(result_u128, uint128_t_dec(power2_to_uint128_t(120))));
	/* (result_u128 + (2^120 - 1)) + 1 = result_u128 + 2^120 (bit 120 set to 1) */
	uint128_t_to_hexstr(result_u128, 16, result);
	expected_result = "0100000000000001000000000081fffe";
	if (strcmp(result, expected_result) != 0)
		FAILF("uint128_t_add() failed, got:\n\"%s\", expected:\n\"%s\"\n", result, expected_result);

	U128_SET_ZERO(test1_u128);
	test1_u128 = uint128_t_inc(test1_u128);	/* test1_u128 = (uint128_t)1 */

	U128_SET_ZERO(test2_u128);
	result_u128 = uint128_t_add(uint128_t_dec(test2_u128), test1_u128);	/* Add -1, thus subtract 1, then increment, should get 0 */
	uint128_t_to_hexstr(result_u128, 16, result);

	expected_result = "00000000000000000000000000000000";
	if (strcmp(result, expected_result) != 0)
		FAILF("uint128_t_add() failed, got:\n\"%s\", expected:\n\"%s\"\n", result, expected_result);

#ifndef HAS_INT128
	test1_u128.uint128_a8[0] = 0x12;
	test1_u128.uint128_a8[1] = 0x34;
	test1_u128.uint128_a8[2] = 0x56;
	test1_u128.uint128_a8[3] = 0x78;
	test1_u128.uint128_a8[4] = 0x9a;
	test1_u128.uint128_a8[5] = 0xbc;
	test1_u128.uint128_a8[6] = 0xde;
	test1_u128.uint128_a8[7] = 0xf0;
	test1_u128.uint128_a8[8] = 0x12;
	test1_u128.uint128_a8[9] = 0x34;
	test1_u128.uint128_a8[10] = 0x56;
	test1_u128.uint128_a8[11] = 0x78;
	test1_u128.uint128_a8[12] = 0x9a;
	test1_u128.uint128_a8[13] = 0xbc;
	test1_u128.uint128_a8[14] = 0xde;
	test1_u128.uint128_a8[15] = 0xf0;

	test2_u128.uint128_a8[0] = 0xf0;
	test2_u128.uint128_a8[1] = 0x1e;
	test2_u128.uint128_a8[2] = 0x2d;
	test2_u128.uint128_a8[3] = 0x3c;
	test2_u128.uint128_a8[4] = 0x4b;
	test2_u128.uint128_a8[5] = 0x5a;
	test2_u128.uint128_a8[6] = 0x69;
	test2_u128.uint128_a8[7] = 0x78;
	test2_u128.uint128_a8[8] = 0x87;
	test2_u128.uint128_a8[9] = 0x96;
	test2_u128.uint128_a8[10] = 0xa5;
	test2_u128.uint128_a8[11] = 0xb4;
	test2_u128.uint128_a8[12] = 0xc3;
	test2_u128.uint128_a8[13] = 0xd2;
	test2_u128.uint128_a8[14] = 0xe1;
	test2_u128.uint128_a8[15] = 0x0f;
#else
	test1_u128 = ((uint128_t)0x123456789abcdef0)<<64 | (uint128_t)0x123456789abcdef0;
	test2_u128 = ((uint128_t)0xf01e2d3c4b5a6978)<<64 | (uint128_t)0x8796a5b4c3d2e10f;
#endif

	expected_result = "025283b4e617486899cafc2d5e8fbfff";	/* echo 'print "%x" % (0x123456789abcdef0123456789abcdef0 + 0xf01e2d3c4b5a69788796a5b4c3d2e10f)' | python */

	result_u128 = uint128_t_add(test1_u128, test2_u128);
	uint128_t_to_hexstr(result_u128, 16, result);

	if (strcmp(result, expected_result) != 0)
		FAILF("uint128_t_add() failed, got:\n\"%s\", expected:\n\"%s\"\n", result, expected_result);

	U128_SET_ZERO(test1_u128);
	result_u128 = uint128_t_add(result_u128, test1_u128);

	uint128_t_to_hexstr(result_u128, 16, result);	/* adding 0, should get same value as before */

	if (strcmp(result, expected_result) != 0)
		FAILF("uint128_t_add() failed, got:\n\"%s\", expected:\n\"%s\"\n", result, expected_result);

	NOTIFYPASS();
}

TEST(uint128_tests, test_uint128_t_mixed_add_sub_inc_dec) {
	uint128_t test1_u128;
	uint128_t test2_u128;
	uint16_t i;

	U128_SET_ZERO(test1_u128);
	test2_u128 = test1_u128;
	test1_u128 = uint128_t_dec(test1_u128);	/* test1_u128 = (uint128_t)-1 */
	test2_u128 = uint128_t_sub(test2_u128, uint16_t_to_uint128_t(0xffff));	/* test1_u128 = (uint128_t)-0xffff */
	for (i = 0; i<0xffff; i++) {
		test2_u128 = uint128_t_sub(test2_u128, test1_u128);	/* remove -1 (add 1) 0xffff times */
	}
	if (!U128_IS_ZERO(test2_u128))	/* Expect test2_u128==0 */
		FAILF("test failed\n");

	test1_u128 = power2_to_uint128_t(0);	/* test1_u128 = (uint128_t)1 */
	U128_SET_ZERO(test2_u128);
	test2_u128 = uint128_t_add(test2_u128, uint16_t_to_uint128_t(0xffff));	/* test1_u128 = (uint128_t)0xffff */
	for (i = 0; i<0xffff; i++) {
		test2_u128 = uint128_t_sub(test2_u128, test1_u128);	/* Remove 1 0xffff times */
	}
	if (!U128_IS_ZERO(test2_u128))	/* Expect test2_u128==0 */
		FAILF("test failed\n");

	for (i = 0; i<0xffff; i++) {
#ifndef HAS_INT128
		test1_u128.uint128_a8[0] = rand() & 0xff;
		test1_u128.uint128_a8[1] = rand() & 0xff;
		test1_u128.uint128_a8[2] = rand() & 0xff;
		test1_u128.uint128_a8[3] = rand() & 0xff;
		test1_u128.uint128_a8[4] = rand() & 0xff;
		test1_u128.uint128_a8[5] = rand() & 0xff;
		test1_u128.uint128_a8[6] = rand() & 0xff;
		test1_u128.uint128_a8[7] = rand() & 0xff;
		test1_u128.uint128_a8[8] = rand() & 0xff;
		test1_u128.uint128_a8[9] = rand() & 0xff;
		test1_u128.uint128_a8[10] = rand() & 0xff;
		test1_u128.uint128_a8[11] = rand() & 0xff;
		test1_u128.uint128_a8[12] = rand() & 0xff;
		test1_u128.uint128_a8[13] = rand() & 0xff;
		test1_u128.uint128_a8[14] = rand() & 0xff;
		test1_u128.uint128_a8[15] = rand() & 0xff;
#else
		test1_u128 =  ((uint128_t)(rand() & 0xffff)) << 48 | ((uint128_t)(rand() & 0xffff)) << 32 | ((uint128_t)(rand() & 0xffff)) << 16 | (uint128_t)(rand() & 0xffff);	/* Build 64 random bits */
		test1_u128 <<= 64;	/* Move to 64 most significant bits */
		test1_u128 |= ((uint128_t)(rand() & 0xffff)) << 48 | ((uint128_t)(rand() & 0xffff)) << 32 | ((uint128_t)(rand() & 0xffff)) << 16 | (uint128_t)(rand() & 0xffff);	/* Build 64 random bits */
#endif
		test2_u128 = test1_u128;

		test1_u128 = uint128_t_inc(test1_u128);	/* +1 */
		test1_u128 = uint128_t_add(test1_u128, uint128_t_inc(uint128_t_zero()));	/* +1 */
		test1_u128 = uint128_t_dec(test1_u128);	/* -1 */
		test1_u128 = uint128_t_sub(test1_u128, power2_to_uint128_t(1));	/* -2 */
		test1_u128 = uint128_t_add(test1_u128, uint128_t_inc(uint128_t_zero()));	/* +1 */
		test1_u128 = uint128_t_add(test1_u128, power2_to_uint128_t(127));	/* + 2^127 */
		test1_u128 = uint128_t_sub(test1_u128, uint128_t_dec(power2_to_uint128_t(127)));	/* - (2^127 - 1) */
		test1_u128 = uint128_t_dec(test1_u128);	/* -1 */

		if (!(uint128_t_cmp(test2_u128, test1_u128) == 0))	/* Expect test2_u128==0 */
			FAILF("test failed\n");
	}

	NOTIFYPASS();
}

/* Unit test for uint128_t_or()
 */
TEST(uint128_tests, test_uint128_t_or) {
	uint128_t test1_u128;
	uint128_t test2_u128;
	uint128_t result_u128;

	U128_SET_ZERO(test1_u128);
	test2_u128 = test1_u128;
	result_u128 = uint128_t_or(test1_u128, test2_u128);	/* A or A = A */
	if (!(uint128_t_cmp(result_u128, test1_u128) == 0))
		FAILF("uint128_t_or() failed\n");

	U128_SET_ZERO(test1_u128);
	test1_u128 = uint128_t_dec(test1_u128);
	test2_u128 = test1_u128;
	result_u128 = uint128_t_or(test1_u128, test2_u128);	/* A or A = A */
	if (!(uint128_t_cmp(result_u128, test1_u128) == 0))
		FAILF("uint128_t_or() failed\n");

#ifndef HAS_INT128
	test1_u128.uint128_a8[0] = 0xa5;
	test1_u128.uint128_a8[1] = 0x5a;
	test1_u128.uint128_a8[2] = 0xa5;
	test1_u128.uint128_a8[3] = 0x5a;
	test1_u128.uint128_a8[4] = 0xa5;
	test1_u128.uint128_a8[5] = 0x5a;
	test1_u128.uint128_a8[6] = 0xa5;
	test1_u128.uint128_a8[7] = 0x5a;
	test1_u128.uint128_a8[8] = 0xa5;
	test1_u128.uint128_a8[9] = 0x5a;
	test1_u128.uint128_a8[10] = 0xa5;
	test1_u128.uint128_a8[11] = 0x5a;
	test1_u128.uint128_a8[12] = 0xa5;
	test1_u128.uint128_a8[13] = 0x5a;
	test1_u128.uint128_a8[14] = 0xa5;
	test1_u128.uint128_a8[15] = 0x5a;
#else
	test1_u128 = ((uint128_t)0xa55aa55aa55aa55a)<<64 | (uint128_t)0xa55aa55aa55aa55a;
#endif
	test2_u128 = test1_u128;
	result_u128 = uint128_t_or(test1_u128, test2_u128);	/* A or A = A */
	if (!(uint128_t_cmp(result_u128, test1_u128) == 0))
		FAILF("uint128_t_or() failed\n");

	result_u128 = uint128_t_or(test1_u128, uint128_t_zero());	/* A or 0 = A */
	if (!(uint128_t_cmp(result_u128, test1_u128) == 0))
		FAILF("uint128_t_or() failed\n");

	result_u128 = uint128_t_or(test1_u128, uint128_t_max());	/* A or (-1) = (-1) */
	if (!(uint128_t_cmp(result_u128, uint128_t_max()) == 0))
		FAILF("uint128_t_or() failed\n");

#ifndef HAS_INT128
	test2_u128.uint128_a8[0] = 0x5a;
	test2_u128.uint128_a8[1] = 0xa5;
	test2_u128.uint128_a8[2] = 0x5a;
	test2_u128.uint128_a8[3] = 0xa5;
	test2_u128.uint128_a8[4] = 0x5a;
	test2_u128.uint128_a8[5] = 0xa5;
	test2_u128.uint128_a8[6] = 0x5a;
	test2_u128.uint128_a8[7] = 0xa5;
	test2_u128.uint128_a8[8] = 0x5a;
	test2_u128.uint128_a8[9] = 0xa5;
	test2_u128.uint128_a8[10] = 0x5a;
	test2_u128.uint128_a8[11] = 0xa5;
	test2_u128.uint128_a8[12] = 0x5a;
	test2_u128.uint128_a8[13] = 0xa5;
	test2_u128.uint128_a8[14] = 0x5a;
	test2_u128.uint128_a8[15] = 0xa5;
#else
	test2_u128 = ((uint128_t)0x5aa55aa55aa55aa5)<<64 | (uint128_t)0x5aa55aa55aa55aa5;
#endif

	result_u128 = uint128_t_or(test1_u128, test2_u128);	/* A or !A = (uint128_t)(-1) */

	if (!(uint128_t_cmp(result_u128, uint128_t_max()) == 0))
		FAILF("uint128_t_or() failed\n");

	NOTIFYPASS();
}

/* Unit test for uint128_t_and()
 */
TEST(uint128_tests, test_uint128_t_and) {
	uint128_t test1_u128;
	uint128_t test2_u128;
	uint128_t result_u128;

	U128_SET_ZERO(test1_u128);
	test2_u128 = test1_u128;
	result_u128 = uint128_t_and(test1_u128, test2_u128);	/* A and A = A */
	if (!(uint128_t_cmp(result_u128, test1_u128) == 0))
		FAILF("uint128_t_and() failed\n");

	U128_SET_ZERO(test1_u128);
	test1_u128 = uint128_t_dec(test1_u128);
	test2_u128 = test1_u128;
	result_u128 = uint128_t_and(test1_u128, test2_u128);	/* A and A = A */
	if (!(uint128_t_cmp(result_u128, test1_u128) == 0))
		FAILF("uint128_t_and() failed\n");

#ifndef HAS_INT128
	test1_u128.uint128_a8[0] = 0xa5;
	test1_u128.uint128_a8[1] = 0x5a;
	test1_u128.uint128_a8[2] = 0xa5;
	test1_u128.uint128_a8[3] = 0x5a;
	test1_u128.uint128_a8[4] = 0xa5;
	test1_u128.uint128_a8[5] = 0x5a;
	test1_u128.uint128_a8[6] = 0xa5;
	test1_u128.uint128_a8[7] = 0x5a;
	test1_u128.uint128_a8[8] = 0xa5;
	test1_u128.uint128_a8[9] = 0x5a;
	test1_u128.uint128_a8[10] = 0xa5;
	test1_u128.uint128_a8[11] = 0x5a;
	test1_u128.uint128_a8[12] = 0xa5;
	test1_u128.uint128_a8[13] = 0x5a;
	test1_u128.uint128_a8[14] = 0xa5;
	test1_u128.uint128_a8[15] = 0x5a;
#else
	test1_u128 = ((uint128_t)0xa55aa55aa55aa55a)<<64 | (uint128_t)0xa55aa55aa55aa55a;
#endif
	test2_u128 = test1_u128;
	result_u128 = uint128_t_and(test1_u128, test2_u128);	/* A and A = A */
	if (!(uint128_t_cmp(result_u128, test1_u128) == 0))
		FAILF("uint128_t_and() failed\n");

	result_u128 = uint128_t_and(test1_u128, uint128_t_max());	/* A and (-1) = A */
	if (!(uint128_t_cmp(result_u128, test1_u128) == 0))
		FAILF("uint128_t_and() failed\n");

	result_u128 = uint128_t_and(test1_u128, uint128_t_zero());	/* A and 0 = 0 */
	if (!U128_IS_ZERO(result_u128))
		FAILF("uint128_t_and() failed\n");

#ifndef HAS_INT128
	test2_u128.uint128_a8[0] = 0x5a;
	test2_u128.uint128_a8[1] = 0xa5;
	test2_u128.uint128_a8[2] = 0x5a;
	test2_u128.uint128_a8[3] = 0xa5;
	test2_u128.uint128_a8[4] = 0x5a;
	test2_u128.uint128_a8[5] = 0xa5;
	test2_u128.uint128_a8[6] = 0x5a;
	test2_u128.uint128_a8[7] = 0xa5;
	test2_u128.uint128_a8[8] = 0x5a;
	test2_u128.uint128_a8[9] = 0xa5;
	test2_u128.uint128_a8[10] = 0x5a;
	test2_u128.uint128_a8[11] = 0xa5;
	test2_u128.uint128_a8[12] = 0x5a;
	test2_u128.uint128_a8[13] = 0xa5;
	test2_u128.uint128_a8[14] = 0x5a;
	test2_u128.uint128_a8[15] = 0xa5;
#else
	test2_u128 = ((uint128_t)0x5aa55aa55aa55aa5)<<64 | (uint128_t)0x5aa55aa55aa55aa5;
#endif

	result_u128 = uint128_t_and(test1_u128, test2_u128);	/* A and !A = 0 */

	if (!U128_IS_ZERO(result_u128))
		FAILF("uint128_t_and() failed\n");

	NOTIFYPASS();
}

/* Unit test for uint128_t_right_0bit_count()
 */
TEST(uint128_tests, test_uint128_t_right_0bit_count) {
	uint128_t test_u128;
	uint8_t bit_count;
	uint8_t result;


	U128_SET_ZERO(test_u128);
	test_u128 = uint128_t_inc(test_u128);	/* test_u128 = (uint128_t)1 */

	result = uint128_t_right_0bit_count(test_u128);
	if (result != 0)	/* LSB bit is 1... so we should count 0 */
		FAILF("uint128_t_right_0bit_count() failed, got: %u, expected: %u\n", result, 0);

	for (bit_count = 1; bit_count<=128; bit_count++) {
		test_u128 = uint128_t_left_shift(test_u128);
		result = uint128_t_right_0bit_count(test_u128);
		if (result != bit_count)	/* LSB bit is 1... so we should count 0 */
			FAILF("uint128_t_right_0bit_count() failed, got: %u, expected: %u\n", result, bit_count);
	}

	NOTIFYPASS();
}

/* Unit test for msb_1bits_to_uint128_t()
 */
TEST(uint128_tests, test_msb_1bits_to_uint128_t) {

	uint8_t counted_msb_bits;
	uint128_t result;
	uint8_t requested_msb_bits;

	for (requested_msb_bits = 0; requested_msb_bits<129; requested_msb_bits++) {	/* Perform all combinations */
		result = msb_1bits_to_uint128_t(requested_msb_bits);
		counted_msb_bits = 0;

#ifndef HAS_INT128
		while (uint128_t_get_byte_no(result, 15) & 0x80) {	/* Grab bit 127 of uint128_t */
#else
		while (result & ((uint128_t)1)<<127) {	/* Grab bit 127 of uint128_t */
#endif
			counted_msb_bits++;
			result = uint128_t_left_shift(result);
		}
		if (counted_msb_bits != requested_msb_bits)
			FAILF("msb_1bits_to_uint128_t() failed. Got %u MSB bits set, expected %u\n", counted_msb_bits, requested_msb_bits);

		if (!U128_IS_ZERO(result))
			FAILF("msb_1bits_to_uint128_t() failed. Expected only zero bits under the last bit set for %u MSB\n", requested_msb_bits);
	}

	NOTIFYPASS();
}

/* Unit test for uint128_t_cmp()
 */
TEST(uint128_tests, test_uint128_t_cmp) {
	uint128_t test1_u128;
	uint128_t test2_u128;

	U128_SET_ZERO(test1_u128);
	U128_SET_ZERO(test2_u128);
	if (!(uint128_t_cmp(test1_u128, test2_u128) == 0))	/* Expect 0==0 */
		FAILF("uint128_t_cmp() failed\n");

	test1_u128 = uint128_t_inc(test1_u128);
	if (!(uint128_t_cmp(test1_u128, test2_u128) > 0))	/* Expect 1>0 */
		FAILF("uint128_t_cmp() failed\n");

	test2_u128 = uint128_t_inc(test2_u128);
	if (!(uint128_t_cmp(test1_u128, test2_u128) == 0))	/* Expect 1==1 */
		FAILF("uint128_t_cmp() failed\n");

	test1_u128 = uint128_t_dec(test1_u128);
	if (!(uint128_t_cmp(test1_u128, test2_u128) < 0))	/* Expect 0<1 */
		FAILF("uint128_t_cmp() failed\n");

	U128_SET_ZERO(test1_u128);
	U128_SET_ZERO(test2_u128);
	test1_u128 = uint128_t_dec(test1_u128);
	if (!(uint128_t_cmp(test1_u128, test2_u128) > 0))	/* Expect (uint128_t)(-1)>0 */
		FAILF("uint128_t_cmp() failed\n");

	if (!(uint128_t_cmp(test2_u128, test1_u128) < 0))	/* Expect 0<(uint128_t)(-1) */
		FAILF("uint128_t_cmp() failed\n");

	test2_u128 = uint128_t_inc(test2_u128);
	if (!(uint128_t_cmp(test1_u128, test2_u128) > 0))	/* Expect (uint128_t)(-1)>1 */
		FAILF("uint128_t_cmp() failed\n");

	if (!(uint128_t_cmp(test2_u128, test1_u128) < 0))	/* Expect 1<(uint128_t)(-1) */
		FAILF("uint128_t_cmp() failed\n");

	test1_u128 = power2_to_uint128_t(7);
	test2_u128 = power2_to_uint128_t(8);
	if (!(uint128_t_cmp(test1_u128, test2_u128) < 0))	/* Expect 2^7<2^8 */
		FAILF("uint128_t_cmp() failed on bit 7 to 8 boundary\n");

	if (!(uint128_t_cmp(test2_u128, test1_u128) > 0))	/* Expect 2^8>2^7 */
		FAILF("uint128_t_cmp() failed on bit 7 to 8 boundary\n");

	test1_u128 = power2_to_uint128_t(15);
	test2_u128 = power2_to_uint128_t(16);
	if (!(uint128_t_cmp(test1_u128, test2_u128) < 0))	/* Expect 2^15<2^16 */
		FAILF("uint128_t_cmp() failed on bit 15 to 16 boundary\n");

	if (!(uint128_t_cmp(test2_u128, test1_u128) > 0))	/* Expect 2^16>2^15 */
		FAILF("uint128_t_cmp() failed on bit 15 to 16 boundary\n");

	test1_u128 = power2_to_uint128_t(31);
	test2_u128 = power2_to_uint128_t(32);
	if (!(uint128_t_cmp(test1_u128, test2_u128) < 0))	/* Expect 2^31<2^32 */
		FAILF("uint128_t_cmp() failed on bit 31 to 32 boundary\n");

	if (!(uint128_t_cmp(test2_u128, test1_u128) > 0))	/* Expect 2^32>2^31 */
		FAILF("uint128_t_cmp() failed on bit 31 to 32 boundary\n");

	test1_u128 = power2_to_uint128_t(63);
	test2_u128 = power2_to_uint128_t(64);
	if (!(uint128_t_cmp(test1_u128, test2_u128) < 0))	/* Expect 2^63<2^64 */
		FAILF("uint128_t_cmp() failed on bit 63 to 64 boundary\n");

	if (!(uint128_t_cmp(test2_u128, test1_u128) > 0))	/* Expect 2^64>2^63 */
		FAILF("uint128_t_cmp() failed on bit 63 to 64 boundary\n");

	test1_u128 = power2_to_uint128_t(0);
	test2_u128 = power2_to_uint128_t(127);
	if (!(uint128_t_cmp(test1_u128, test2_u128) < 0))	/* Expect 2^0<2^127 */
		FAILF("uint128_t_cmp() failed\n");

	if (!(uint128_t_cmp(test2_u128, test1_u128) > 0))	/* Expect 2^127>2^0 */
		FAILF("uint128_t_cmp() failed\n");

	NOTIFYPASS();
}

/* Unit test for uint128_t_hton()
 */
TEST(uint128_tests, test_uint128_t_hton) {

	uint128_t test_u128;
	uint8_t *uint128_repr_ptr;

#ifndef HAS_INT128
	test_u128.uint128_a8[0] = 0xf0;
	test_u128.uint128_a8[1] = 0x1e;
	test_u128.uint128_a8[2] = 0x2d;
	test_u128.uint128_a8[3] = 0x3c;
	test_u128.uint128_a8[4] = 0x4b;
	test_u128.uint128_a8[5] = 0x5a;
	test_u128.uint128_a8[6] = 0x69;
	test_u128.uint128_a8[7] = 0x78;
	test_u128.uint128_a8[8] = 0x87;
	test_u128.uint128_a8[9] = 0x96;
	test_u128.uint128_a8[10] = 0xa5;
	test_u128.uint128_a8[11] = 0xb4;
	test_u128.uint128_a8[12] = 0xc3;
	test_u128.uint128_a8[13] = 0xd2;
	test_u128.uint128_a8[14] = 0xe1;
	test_u128.uint128_a8[15] = 0x0f;
#else
	test_u128 = ((uint128_t)0xf01e2d3c4b5a6978)<<64 | (uint128_t)0x8796a5b4c3d2e10f;
#endif

	test_u128 = uint128_t_hton(test_u128);

#ifndef HAS_INT128
	uint128_repr_ptr = test_u128.uint128_a8;
#else
	uint128_repr_ptr = (void*)&test_u128;
#endif
	if ((uint128_repr_ptr[0] == 0xf0) &&
	    (uint128_repr_ptr[1] = 0x1e) &&
	    (uint128_repr_ptr[2] = 0x2d) &&
	    (uint128_repr_ptr[3] = 0x3c) &&
	    (uint128_repr_ptr[4] = 0x4b) &&
	    (uint128_repr_ptr[5] = 0x5a) &&
	    (uint128_repr_ptr[6] = 0x69) &&
	    (uint128_repr_ptr[7] = 0x78) &&
	    (uint128_repr_ptr[8] = 0x87) &&
	    (uint128_repr_ptr[9] = 0x96) &&
	    (uint128_repr_ptr[10] = 0xa5) &&
	    (uint128_repr_ptr[11] = 0xb4) &&
	    (uint128_repr_ptr[12] = 0xc3) &&
	    (uint128_repr_ptr[13] = 0xd2) &&
	    (uint128_repr_ptr[14] = 0xe1) &&
	    (uint128_repr_ptr[15] = 0x0f)) {
	}
	else
		FAILF("uint128_t_hton() failed\n");

	NOTIFYPASS();
}

#ifndef USE_CPPUTEST
void unit_tests_uint128() {
	test_zero_max_uint128_t();
	test_uint128_t_get_byte_no();
	test_uint128_t_to_binstr();
	test_uint128_t_to_hexstr();
	test_power2_to_uint128_t();
	test_uint128_t_right_shift();
	test_uint128_t_left_shift();
	test_uint128_t_mix_shift();
	test_uint128_t_right_shift_n();
	test_uint128_t_left_shift_n();
	test_uint128_t_inc();
	test_uint128_t_dec();
	test_uint16_t_to_uint128_t();
	test_uint8_t_to_uint128_t();
	test_uint128_t_sub();
	test_uint128_t_add();
	test_uint128_t_mixed_add_sub_inc_dec();
	test_uint128_t_or();
	test_uint128_t_and();
	test_uint128_t_right_0bit_count();
	test_msb_1bits_to_uint128_t();
	test_uint128_t_cmp();
	test_uint128_t_hton();
}
#endif	// USE_CPPUTEST
