#include "uint128.h"
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>	// For inet_ntop() and htons()
#include <stdint.h>
#include <string.h>	// For memcpy()

#define zero_word_16bits "00000000" "00000000"

/* Unit test for set_zero_uint128_t()
 */
void test_zero_uint18_t() {
	uint128_t test_u128;

	test_u128.uint128_a16[0] = 0xffff;
	test_u128.uint128_a16[1] = 0xffff;
	test_u128.uint128_a16[2] = 0xffff;
	test_u128.uint128_a16[3] = 0xffff;
	test_u128.uint128_a16[4] = 0xffff;
	test_u128.uint128_a16[5] = 0xffff;
	test_u128.uint128_a16[6] = 0xffff;
	test_u128.uint128_a16[7] = 0xffff;
	set_zero_uint128_t(test_u128);
	if (test_u128.uint128_a16[0] == 0 &&
	    test_u128.uint128_a16[1] == 0 &&
	    test_u128.uint128_a16[2] == 0 &&
	    test_u128.uint128_a16[3] == 0 &&
	    test_u128.uint128_a16[4] == 0 &&
	    test_u128.uint128_a16[5] == 0 &&
	    test_u128.uint128_a16[6] == 0 &&
	    test_u128.uint128_a16[7] == 0) {
	}
	else {
		fprintf(stderr, "set_zero_uint128_t() Failed\n");
		//FAIL();
		exit(1);
	}

	test_u128 = uint128t_zero();
	if (test_u128.uint128_a16[0] == 0 &&
	    test_u128.uint128_a16[1] == 0 &&
	    test_u128.uint128_a16[2] == 0 &&
	    test_u128.uint128_a16[3] == 0 &&
	    test_u128.uint128_a16[4] == 0 &&
	    test_u128.uint128_a16[5] == 0 &&
	    test_u128.uint128_a16[6] == 0 &&
	    test_u128.uint128_a16[7] == 0) {
	}
	else {
		fprintf(stderr, "uint128t_zero() Failed\n");
		//FAIL();
		exit(1);
	}

	printf("%s: tests passed\n", __func__);
}

/* Unit test for uint128_t_to_binstr()
 */
void test_uint128_t_to_binstr() {
	uint128_t test_u128;
	char result[129];
	char* expected_result;
	uint8_t nb_bits_missing;

	set_zero_uint128_t(test_u128);
	uint128_t_to_binstr(test_u128, 128, result);
	expected_result = zero_word_16bits /* Word 0 */ \
	                  zero_word_16bits /* Word 1 */ \
	                  zero_word_16bits /* Word 2 */ \
	                  zero_word_16bits /* Word 3 */ \
	                  zero_word_16bits /* Word 4 */ \
	                  zero_word_16bits /* Word 5 */ \
	                  zero_word_16bits /* Word 6 */ \
	                  zero_word_16bits;/* Word 7 */
	if (strcmp(result, expected_result) != 0) {
		fprintf(stderr, "%d: uint128_t_to_binstr() failed, got:\n\"%s\", expected:\n\"%s\"\n", __LINE__, result, expected_result);
		//FAIL();
		exit(1);
	}

	set_zero_uint128_t(test_u128);
	test_u128.uint128_a16[0] = htons(0xfe80);
	test_u128.uint128_a16[6] = htons(0x8000);
	test_u128.uint128_a16[7] = htons(0xa5a5);
	uint128_t_to_binstr(test_u128, 128, result);
	expected_result = "1111111010000000"    /* Word 0 */ \
                      zero_word_16bits      /* Word 1 */ \
                      zero_word_16bits      /* Word 2 */ \
                      zero_word_16bits      /* Word 3 */ \
                      zero_word_16bits      /* Word 4 */ \
                      zero_word_16bits      /* Word 5 */ \
                      "10000000" "00000000" /* Word 6 */ \
                      "10100101" "10100101";/* Word 7 */
	if (strcmp(result, expected_result) != 0) {
		fprintf(stderr, "%d: uint128_t_to_binstr() failed, got:\n\"%s\", expected:\n\"%s\"\n", __LINE__, result, expected_result);
		//FAIL();
		exit(1);
	}

	/* Test printing ipv6.google.com (2a00:1450:4007:80e::200e) IPv6 address as a unit128_t */
	set_zero_uint128_t(test_u128);
	test_u128.uint128_a16[0] = htons(0x2a00);
	test_u128.uint128_a16[1] = htons(0x1450);
	test_u128.uint128_a16[2] = htons(0x4007);
	test_u128.uint128_a16[3] = htons(0x080e);
	test_u128.uint128_a16[7] = htons(0x200e);
	uint128_t_to_binstr(test_u128, 128, result);
	expected_result = "0010101000000000" /* Word 0 */ \
	                  "0001010001010000" /* Word 1 */ \
	                  "0100000000000111" /* Word 2 */ \
	                  "0000100000001110" /* Word 3 */ \
	                  zero_word_16bits   /* Word 4 */ \
	                  zero_word_16bits   /* Word 5 */ \
	                  zero_word_16bits   /* Word 6 */ \
	                  "0010000000001110";/* Word 7 */
	if (strcmp(result, expected_result) != 0) {
		fprintf(stderr, "%d: uint128_t_to_binstr() failed, got:\n\"%s\", expected:\n\"%s\"\n", __LINE__, result, expected_result);
		//FAIL();
		exit(1);
	}
	/* Same 128-bit value (2a00:1450:4007:80e::200e), but assigned byte-per-byte */
	set_zero_uint128_t(test_u128);
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
	if (strcmp(result, expected_result) != 0) {
		fprintf(stderr, "%d: uint128_t_to_binstr() failed, got:\n\"%s\", expected:\n\"%s\"\n", __LINE__, result, expected_result);
		//FAIL();
		exit(1);
	}

	set_zero_uint128_t(test_u128);
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
	if (strcmp(result, expected_result) != 0) {
		fprintf(stderr, "%d: uint128_t_to_binstr() failed, got:\n\"%s\", expected:\n\"%s\"\n", __LINE__, result, expected_result);
		//FAIL();
		exit(1);
	}
	/* Now dump the same value with less that 128 bits (from 127 LSB to 0 LSB), and check that the string reduces progressively to empty */
	for (nb_bits_missing=1; nb_bits_missing<=128; nb_bits_missing++) {
		uint128_t_to_binstr(test_u128, 128-nb_bits_missing, result);
		if (strcmp(result, expected_result+nb_bits_missing) != 0) {
			fprintf(stderr, "%d: uint128_t_to_binstr() failed, got:\n\"%s\", expected:\n\"%s\"\n", __LINE__, result, expected_result+nb_bits_missing);
			//FAIL();
			exit(1);
		}
	}
	printf("%s: tests passed\n", __func__);
}

/* Unit test for uint128_t_to_hexstr()
 */
void test_uint128_t_to_hexstr() {
	uint128_t test_u128;
	char result[33];
	char* expected_result;
	uint8_t nb_bytes_missing;

	set_zero_uint128_t(test_u128);
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
	uint128_t_to_hexstr(test_u128, 16, result);
	expected_result = "a5a2" "1502" "45a8" "87c4" "e504" "1afe" "899c" "470f";
	if (strcmp(result, expected_result) != 0) {
		fprintf(stderr, "%d: uint128_t_to_hexstr() failed, got:\n\"%s\", expected:\n\"%s\"\n", __LINE__, result, expected_result);
		//FAIL();
		exit(1);
	}
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
	uint128_t_to_hexstr(test_u128, 16, result);
	expected_result = "0f1e2d3c4b5a69788796a5b4c3d2e1f0";
	if (strcmp(result, expected_result) != 0) {
		fprintf(stderr, "%d: uint128_t_to_hexstr() failed, got:\n\"%s\", expected:\n\"%s\"\n", __LINE__, result, expected_result);
		//FAIL();
		exit(1);
	}

	/* Now dump the same value with less that 16 bytes (from 15 LSB to 0 LSB), and check that the string reduces progressively to empty */
	for (nb_bytes_missing=1; nb_bytes_missing<=16; nb_bytes_missing++) {
		uint128_t_to_hexstr(test_u128, 16-nb_bytes_missing, result);
		if (strcmp(result, expected_result+nb_bytes_missing*2) != 0) {	// nb_bytes_missing*2 because there are two hex digits output per byte
			fprintf(stderr, "%d: uint128_t_to_hexstr() failed, got:\n\"%s\", expected:\n\"%s\"\n", __LINE__, result, expected_result+nb_bytes_missing*2);
			//FAIL();
			exit(1);
		}
	}
	printf("%s: tests passed\n", __func__);
}

/* Unit test for power2_to_uint128_t()
 * Note: uint8_t_to_binstr() will be covered while testing this */
void test_power2_to_uint128_t() {
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
			if (result[string_pos] != expected) {
				fprintf(stderr, "%d: power2_to_uint128_t() failed at pos %d, got:\n\"%c\", expected:\n\"%c\"\n", __LINE__, string_pos, result[string_pos], expected);
				//FAIL();
				exit(1);
			}
		}
	}
	if (result[128] != '\0') {
		fprintf(stderr, "%d: power2_to_uint128_t() did not get a NULL termination at pos 128\n", __LINE__);
		//FAIL();
		exit(1);
	}
	printf("%s: tests passed\n", __func__);
}

/* Unit test for uint128_t_right_shift()
 */
void test_uint128_t_right_shift() {
	uint128_t test_u128;
	char result[129];
	char expected;
	char* expected_result;

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
			if (result[string_pos] != expected) {
				fprintf(stderr, "%d: uint128_t_right_shift() failed at pos %d, got:\n\"%c\", expected:\n\"%c\"\n", __LINE__, string_pos, result[string_pos], expected);
				//FAIL();
				exit(1);
			}
		}
	}
	if (result[128] != '\0') {
		fprintf(stderr, "%d: uint128_t_right_shift() did not get a NULL termination at pos 128\n", __LINE__);
		//FAIL();
		exit(1);
	}
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
			if (result[string_pos] != expected) {
				fprintf(stderr, "%d: uint128_t_right_shift() failed at pos %d, got:\n\"%c\", expected:\n\"%c\"\n", __LINE__, string_pos, result[string_pos], expected);
				//FAIL();
				exit(1);
			}
		}
	}
	printf("%s: tests passed\n", __func__);
}

/* Unit test for uint128_t_left_shift()
 */
void test_uint128_t_left_shift() {
	uint128_t test_u128;
	char result[129];
	char expected;
	char* expected_result;

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
			if (result[string_pos] != expected) {
				fprintf(stderr, "%d: uint128_t_left_shift() failed at pos %d, got:\n\"%c\", expected:\n\"%c\"\n", __LINE__, string_pos, result[string_pos], expected);
				//FAIL();
				exit(1);
			}
		}
	}
	if (result[128] != '\0') {
		fprintf(stderr, "%d: uint128_t_left_shift() did not get a NULL termination at pos 128\n", __LINE__);
		//FAIL();
		exit(1);
	}
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
			if (result[string_pos] != expected) {
				fprintf(stderr, "%d: uint128_t_right_shift() failed at pos %d, got:\n\"%c\", expected:\n\"%c\"\n", __LINE__, string_pos, result[string_pos], expected);
				//FAIL();
				exit(1);
			}
		}
	}
	printf("%s: tests passed\n", __func__);
}

/* Unit test for mixed uint128_t_left_shift() and uint128_t_right_shift()
 */
void test_uint128_t_mix_shift() {
	uint128_t test1_u128;
	uint128_t test2_u128;
	char result1[129];
	char result2[129];
	char expected;
	char expected_result[129];

	int i;
	uint8_t shift;
	uint8_t string_pos;

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
			if (result1[string_pos] != expected) {
				fprintf(stderr, "%d: uint128_t_right_shift() failed on result1 at pos %d, got:\n\"%c\", expected:\n\"%c\"\n", __LINE__, string_pos, result1[string_pos], expected);
				//FAIL();
				exit(1);
			}
			if (string_pos > 127-shift) {
				expected = '0';	/* All right bits that we taken off test2_u128 by shift overflow should be set to '0' */
				//fprintf(stderr, "expecting result2[%d]='0' (removed)\n", string_pos);
			}
			else {
				expected = expected_result[string_pos];
				//fprintf(stderr, "expecting result2[%d]='%c'\n", string_pos, expected);
			}
			if (result2[string_pos] != expected) {
				fprintf(stderr, "%d: uint128_t_right_shift() failed on result2 at pos %d, got:\n\"%c\", expected:\n\"%c\"\n", __LINE__, string_pos, result1[string_pos], expected);
				//FAIL();
				exit(1);
			}
		}
	}
	printf("%s: tests passed\n", __func__);
}

/* Unit test for uint128_t_inc()
 */
void test_uint128_t_inc() {
	uint128_t test_u128;
	char result[33];
	char* expected_result;

	uint16_t inc_count;
	uint16_t last_word16;

	set_zero_uint128_t(test_u128);
	for (inc_count = 0; inc_count != 65535; inc_count++) {
		test_u128 = uint128_t_inc(test_u128);
		//uint128_t_to_hexstr(test_u128, 16, result);
		//printf("Value: %s\n", result);
		last_word16 = ntohs(test_u128.uint128_a16[7]);
		if (last_word16-1 != inc_count) {
			fprintf(stderr, "%d: uint128_t_inc() failed at value %u: got %u\n", __LINE__, inc_count, last_word16);
			//FAIL();
			//exit(1);
		}
	}
	test_u128 = uint128_t_inc(test_u128);	/* Test next increment... will overflow last 16 bits, and also increment bit 16 */
	uint128_t_to_hexstr(test_u128, 16, result);
	expected_result = "00000000000000000000000000010000";
	if (strcmp(result, expected_result) != 0) {
		fprintf(stderr, "%d: uint128_t_inc() failed, got:\n\"%s\", expected:\n\"%s\"\n", __LINE__, result, expected_result);
		//FAIL();
		exit(1);
	}

	test_u128.uint128_a8[0] = 0xff;
	test_u128.uint128_a8[1] = 0xff;
	test_u128.uint128_a8[2] = 0xff;
	test_u128.uint128_a8[3] = 0xff;
	test_u128.uint128_a8[4] = 0xff;
	test_u128.uint128_a8[5] = 0xff;
	test_u128.uint128_a8[6] = 0xff;
	test_u128.uint128_a8[7] = 0xff;
	test_u128.uint128_a8[8] = 0xff;
	test_u128.uint128_a8[9] = 0xff;
	test_u128.uint128_a8[10] = 0xff;
	test_u128.uint128_a8[11] = 0xff;
	test_u128.uint128_a8[12] = 0xff;
	test_u128.uint128_a8[13] = 0xff;
	test_u128.uint128_a8[14] = 0xff;
	test_u128.uint128_a8[15] = 0xff;

	expected_result = "00000000000000000000000000000000";
	test_u128 = uint128_t_inc(test_u128);
	uint128_t_to_hexstr(test_u128, 16, result);
	//printf("Overflow lead to \"%s\"\n", result);
	//printf("Expecting \"%s\"\n", expected_result);

	if (strcmp(result, expected_result) != 0) {
		fprintf(stderr, "%d: uint128_t_inc() failed, got:\n\"%s\", expected:\n\"%s\"\n", __LINE__, result, expected_result);
		//FAIL();
		exit(1);
	}
	printf("%s: tests passed\n", __func__);
}

/* Unit test for uint128_t_dec()
 */
void test_uint128_t_dec() {
	uint128_t test_u128;
	char result[33];
	char* expected_result;

	uint16_t dec_count;
	uint16_t last_word16;

	test_u128.uint128_a8[0] = 0xff;
	test_u128.uint128_a8[1] = 0xff;
	test_u128.uint128_a8[2] = 0xff;
	test_u128.uint128_a8[3] = 0xff;
	test_u128.uint128_a8[4] = 0xff;
	test_u128.uint128_a8[5] = 0xff;
	test_u128.uint128_a8[6] = 0xff;
	test_u128.uint128_a8[7] = 0xff;
	test_u128.uint128_a8[8] = 0xff;
	test_u128.uint128_a8[9] = 0xff;
	test_u128.uint128_a8[10] = 0xff;
	test_u128.uint128_a8[11] = 0xff;
	test_u128.uint128_a8[12] = 0xff;
	test_u128.uint128_a8[13] = 0xff;
	test_u128.uint128_a8[14] = 0xff;
	test_u128.uint128_a8[15] = 0xff;
	for (dec_count = 0; dec_count != 65535; dec_count++) {
		test_u128 = uint128_t_dec(test_u128);
		//uint128_t_to_hexstr(test_u128, 16, result);
		//printf("Value: %s\n", result);
		last_word16 = ntohs(test_u128.uint128_a16[7]);
		if (last_word16+1 != 65535-dec_count) {
			fprintf(stderr, "%d: uint128_t_dec() failed at value %u: got %u\n", __LINE__, dec_count, last_word16);
			//FAIL();
			//exit(1);
		}
	}
	test_u128 = uint128_t_dec(test_u128);	/* Test next decrement... will overflow last 16 bits, and also decrement bit 16 */
	uint128_t_to_hexstr(test_u128, 16, result);
	expected_result = "fffffffffffffffffffffffffffeffff";
	if (strcmp(result, expected_result) != 0) {
		fprintf(stderr, "%d: uint128_t_dec() failed, got:\n\"%s\", expected:\n\"%s\"\n", __LINE__, result, expected_result);
		//FAIL();
		exit(1);
	}

	set_zero_uint128_t(test_u128);

	expected_result = "ffffffffffffffffffffffffffffffff";
	test_u128 = uint128_t_dec(test_u128);
	uint128_t_to_hexstr(test_u128, 16, result);
	//printf("Overflow lead to \"%s\"\n", result);
	//printf("Expecting \"%s\"\n", expected_result);

	if (strcmp(result, expected_result) != 0) {
		fprintf(stderr, "%d: uint128_t_dec() failed, got:\n\"%s\", expected:\n\"%s\"\n", __LINE__, result, expected_result);
		//FAIL();
		exit(1);
	}
	printf("%s: tests passed\n", __func__);
}

/* Unit test for uint16_t_to_uint128_t()
 */
void test_uint16_t_to_uint128_t() {
	uint16_t source;
	uint16_t last_word16;
	uint128_t result_u128;

	for (source = 0; source != 65535; source++) {
		result_u128 = uint16_t_to_uint128_t(source);
		//uint128_t_to_hexstr(result_u128, 16, result);
		//printf("Value: %s\n", result);
		last_word16 = ntohs(result_u128.uint128_a16[7]);
		if (last_word16 != source) {
			fprintf(stderr, "%d: uint16_t_to_uint128_t() failed on source value %u\n", __LINE__, source);
			//FAIL();
			//exit(1);
		}
		if (result_u128.uint128_a16[0] != 0 ||	/* All other words (0 to 6) should remain 0 */
			result_u128.uint128_a16[1] != 0 ||
			result_u128.uint128_a16[2] != 0 ||
			result_u128.uint128_a16[3] != 0 ||
			result_u128.uint128_a16[4] != 0 ||
			result_u128.uint128_a16[5] != 0 ||
			result_u128.uint128_a16[6] != 0) {
			fprintf(stderr, "%d: uint16_t_to_uint128_t() failed on source value %u: MSW have been altered\n", __LINE__, source);
			//FAIL();
			//exit(1);
		}
	}
	result_u128 = uint16_t_to_uint128_t(source);	/* Do the test again on maximum uint16_t (0xffff) */
	//uint128_t_to_hexstr(result_u128, 16, result);
	//printf("Value: %s\n", result);
	last_word16 = ntohs(result_u128.uint128_a16[7]);
	if (last_word16 != source) {
		fprintf(stderr, "%d: uint16_t_to_uint128_t() failed on source value %u\n", __LINE__, source);
		//FAIL();
		//exit(1);
	}
	if (result_u128.uint128_a16[0] != 0 ||	/* All other words (0 to 6) should remain 0 */
		result_u128.uint128_a16[1] != 0 ||
		result_u128.uint128_a16[2] != 0 ||
		result_u128.uint128_a16[3] != 0 ||
		result_u128.uint128_a16[4] != 0 ||
		result_u128.uint128_a16[5] != 0 ||
		result_u128.uint128_a16[6] != 0) {
		fprintf(stderr, "%d: uint16_t_to_uint128_t() failed on source value %u: MSW have been altered\n", __LINE__, source);
		//FAIL();
		//exit(1);
	}
}

/* Unit test for uint8_t_to_uint128_t()
 */
void test_uint8_t_to_uint128_t() {
	uint8_t source;
	uint8_t last_byte8;
	uint128_t result_u128;

	for (source = 0; source != (uint8_t)(-1); source++) {
		result_u128 = uint8_t_to_uint128_t(source);
		//uint128_t_to_hexstr(result_u128, 16, result);
		//printf("Value: %s\n", result);
		last_byte8 = result_u128.uint128_a8[15];
		if (last_byte8 != source) {
			fprintf(stderr, "%d: uint8_t_to_uint128_t() failed on source value %u\n", __LINE__, source);
			//FAIL();
			//exit(1);
		}
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
			fprintf(stderr, "%d: uint8_t_to_uint128_t() failed on source value %u: MSW have been altered\n", __LINE__, source);
			//FAIL();
			//exit(1);
		}
	}
	result_u128 = uint8_t_to_uint128_t(source);	/* Do the test again on maximum uint8_t (0xff) */
	//uint128_t_to_hexstr(result_u128, 16, result);
	//printf("Value: %s\n", result);
	last_byte8 = result_u128.uint128_a8[15];
	if (last_byte8 != source) {
		fprintf(stderr, "%d: uint8_t_to_uint128_t() failed on source value %u\n", __LINE__, source);
		//FAIL();
		//exit(1);
	}
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
		fprintf(stderr, "%d: uint8_t_to_uint128_t() failed on source value %u: MSW have been altered\n", __LINE__, source);
		//FAIL();
		//exit(1);
	}
}

/* Unit test for uint128_t_sub()
 */
void test_uint128_t_sub() {
	uint128_t test1_u128;
	uint128_t test2_u128;
	uint128_t result_u128;
	char result[33];
	char* expected_result;

	uint16_t dec_count;
	uint16_t last_word16;

	for (dec_count = 0; dec_count != 65535; dec_count++) {
		test1_u128.uint128_a8[0] = 0xff;
		test1_u128.uint128_a8[1] = 0xff;
		test1_u128.uint128_a8[2] = 0xff;
		test1_u128.uint128_a8[3] = 0xff;
		test1_u128.uint128_a8[4] = 0xff;
		test1_u128.uint128_a8[5] = 0xff;
		test1_u128.uint128_a8[6] = 0xff;
		test1_u128.uint128_a8[7] = 0xff;
		test1_u128.uint128_a8[8] = 0xff;
		test1_u128.uint128_a8[9] = 0xff;
		test1_u128.uint128_a8[10] = 0xff;
		test1_u128.uint128_a8[11] = 0xff;
		test1_u128.uint128_a8[12] = 0xff;
		test1_u128.uint128_a8[13] = 0xff;
		test1_u128.uint128_a8[14] = 0xff;
		test1_u128.uint128_a8[15] = 0xff;
		result_u128 = uint128_t_sub(test1_u128, uint16_t_to_uint128_t(dec_count));
		//uint128_t_to_hexstr(result_u128, 16, result);
		//printf("Value: %s\n", result);
		last_word16 = ntohs(result_u128.uint128_a16[7]);
		if (last_word16 != 65535-dec_count) {
			fprintf(stderr, "%d: uint128_t_sub() failed at iteration %u: got %u, expected %u\n", __LINE__, dec_count, last_word16, 65535-dec_count);
			//FAIL();
			//exit(1);
		}
		if (result_u128.uint128_a16[0] != test1_u128.uint128_a16[0] ||	/* All other words (0 to 6) should remain unchanged */
			result_u128.uint128_a16[1] != test1_u128.uint128_a16[1] ||
			result_u128.uint128_a16[2] != test1_u128.uint128_a16[2] ||
			result_u128.uint128_a16[3] != test1_u128.uint128_a16[3] ||
			result_u128.uint128_a16[4] != test1_u128.uint128_a16[4] ||
			result_u128.uint128_a16[5] != test1_u128.uint128_a16[5] ||
			result_u128.uint128_a16[6] != test1_u128.uint128_a16[6]) {
			fprintf(stderr, "%d: uint128_t_sub() failed at iteration %u: MSW have been altered\n", __LINE__, dec_count);
			//FAIL();
			//exit(1);
		}
	}
	result_u128 = uint128_t_dec(result_u128);	/* Now result_u128=0xffffffffffffffffffffffffffff0000 */

	/* Now test subtracting on byte index 13 (bit 16) */
	result_u128 = uint128_t_sub(result_u128, uint128_t_inc(uint16_t_to_uint128_t(0xffff)));
	uint128_t_to_hexstr(result_u128, 16, result);
	expected_result = "fffffffffffffffffffffffffffe0000";
	if (strcmp(result, expected_result) != 0) {
		fprintf(stderr, "%d: uint128_t_sub() failed, got:\n\"%s\", expected:\n\"%s\"\n", __LINE__, result, expected_result);
		//FAIL();
		exit(1);
	}

	result_u128 = uint128_t_sub(result_u128, power2_to_uint128_t(23));
	uint128_t_to_hexstr(result_u128, 16, result);
	expected_result = "ffffffffffffffffffffffffff7e0000";
	if (strcmp(result, expected_result) != 0) {
		fprintf(stderr, "%d: uint128_t_sub() failed, got:\n\"%s\", expected:\n\"%s\"\n", __LINE__, result, expected_result);
		//FAIL();
		exit(1);
	}

	/* Subtracting while propagating a carry to bit 63 */
	result_u128 = uint128_t_sub(result_u128, uint128_t_dec(power2_to_uint128_t(64)));
	uint128_t_to_hexstr(result_u128, 16, result);
	expected_result = "fffffffffffffffeffffffffff7e0001";
	if (strcmp(result, expected_result) != 0) {
		fprintf(stderr, "%d: uint128_t_sub() failed, got:\n\"%s\", expected:\n\"%s\"\n", __LINE__, result, expected_result);
		//FAIL();
		exit(1);
	}

	result_u128 = uint128_t_dec(uint128_t_sub(result_u128, uint128_t_dec(power2_to_uint128_t(120))));
	/* (result_u128 - (2^120 - 1)) - 1 = result_u128 - 2^120 (bit 120 set to 0) */
	uint128_t_to_hexstr(result_u128, 16, result);
	expected_result = "fefffffffffffffeffffffffff7e0001";
	if (strcmp(result, expected_result) != 0) {
		fprintf(stderr, "%d: uint128_t_sub() failed, got:\n\"%s\", expected:\n\"%s\"\n", __LINE__, result, expected_result);
		//FAIL();
		exit(1);
	}

	set_zero_uint128_t(test1_u128);
	test1_u128 = uint128_t_dec(test1_u128);	/* test1_u128 = (uint128_t)-1 */

	set_zero_uint128_t(test2_u128);
	result_u128 = uint128_t_dec(uint128_t_sub(test2_u128, test1_u128));	/* Sub -1, thus +1, then decrement, should get 0 */
	uint128_t_to_hexstr(result_u128, 16, result);

	expected_result = "00000000000000000000000000000000";
	if (strcmp(result, expected_result) != 0) {
		fprintf(stderr, "%d: uint128_t_sub() failed, got:\n\"%s\", expected:\n\"%s\"\n", __LINE__, result, expected_result);
		//FAIL();
		exit(1);
	}

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

	expected_result = "2216293c4f6275778a9db0c3d6e9fde1";	/* echo 'print "%x" % (0xff123456789abcdef0123456789abcdef0 - 0xf01e2d3c4b5a69788796a5b4c3d2e10f)' | python */
	result_u128 = uint128_t_sub(test1_u128, test2_u128);
	uint128_t_to_hexstr(result_u128, 16, result);

	if (strcmp(result, expected_result) != 0) {
		fprintf(stderr, "%d: uint128_t_sub() failed, got:\n\"%s\", expected:\n\"%s\"\n", __LINE__, result, expected_result);
		//FAIL();
		exit(1);
	}

	set_zero_uint128_t(test1_u128);
	result_u128 = uint128_t_sub(result_u128, test1_u128);

	uint128_t_to_hexstr(result_u128, 16, result);	/* substracting 0, should get same value as before */

	if (strcmp(result, expected_result) != 0) {
		fprintf(stderr, "%d: uint128_t_sub() failed, got:\n\"%s\", expected:\n\"%s\"\n", __LINE__, result, expected_result);
		//FAIL();
		exit(1);
	}

	printf("%s: tests passed\n", __func__);
}

/* Unit test for uint128_t_add()
 */
void test_uint128_t_add() {
	uint128_t test1_u128;
	uint128_t test2_u128;
	uint128_t result_u128;
	char result[33];
	char* expected_result;

	uint16_t inc_count;
	uint16_t last_word16;

	for (inc_count = 0; inc_count != 65535; inc_count++) {
		set_zero_uint128_t(test1_u128);
		result_u128 = uint128_t_add(test1_u128, uint16_t_to_uint128_t(inc_count));
		//uint128_t_to_hexstr(result_u128, 16, result);
		//printf("Value: %s\n", result);
		last_word16 = ntohs(result_u128.uint128_a16[7]);
		if (last_word16 != inc_count) {
			fprintf(stderr, "%d: uint128_t_add() failed at iteration %u: got %u, expected %u\n", __LINE__, inc_count, last_word16, inc_count);
			//FAIL();
			//exit(1);
		}
		if (result_u128.uint128_a16[0] != test1_u128.uint128_a16[0] ||	/* All other words (0 to 6) should remain unchanged */
			result_u128.uint128_a16[1] != test1_u128.uint128_a16[1] ||
			result_u128.uint128_a16[2] != test1_u128.uint128_a16[2] ||
			result_u128.uint128_a16[3] != test1_u128.uint128_a16[3] ||
			result_u128.uint128_a16[4] != test1_u128.uint128_a16[4] ||
			result_u128.uint128_a16[5] != test1_u128.uint128_a16[5] ||
			result_u128.uint128_a16[6] != test1_u128.uint128_a16[6]) {
			fprintf(stderr, "%d: uint128_t_add() failed at iteration %u: MSW have been altered\n", __LINE__, inc_count);
			//FAIL();
			//exit(1);
		}
	}
	result_u128 = uint128_t_inc(result_u128);	/* Now result_u128=0x0000000000000000000000000000ffff */

	/* Now test adding on byte index 13 (bit 16) */
	result_u128 = uint128_t_add(result_u128, uint128_t_inc(uint16_t_to_uint128_t(0xffff)));
	uint128_t_to_hexstr(result_u128, 16, result);
	expected_result = "0000000000000000000000000001ffff";
	if (strcmp(result, expected_result) != 0) {
		fprintf(stderr, "%d: uint128_t_add() failed, got:\n\"%s\", expected:\n\"%s\"\n", __LINE__, result, expected_result);
		//FAIL();
		exit(1);
	}

	result_u128 = uint128_t_add(result_u128, power2_to_uint128_t(23));
	uint128_t_to_hexstr(result_u128, 16, result);
	expected_result = "0000000000000000000000000081ffff";
	if (strcmp(result, expected_result) != 0) {
		fprintf(stderr, "%d: uint128_t_add() failed, got:\n\"%s\", expected:\n\"%s\"\n", __LINE__, result, expected_result);
		//FAIL();
		exit(1);
	}

	/* Adding while propagating a carry to bit 63 */
	result_u128 = uint128_t_add(result_u128, uint128_t_dec(power2_to_uint128_t(64)));
	uint128_t_to_hexstr(result_u128, 16, result);
	expected_result = "0000000000000001000000000081fffe";
	if (strcmp(result, expected_result) != 0) {
		fprintf(stderr, "%d: uint128_t_add() failed, got:\n\"%s\", expected:\n\"%s\"\n", __LINE__, result, expected_result);
		//FAIL();
		exit(1);
	}

	result_u128 = uint128_t_inc(uint128_t_add(result_u128, uint128_t_dec(power2_to_uint128_t(120))));
	/* (result_u128 + (2^120 - 1)) + 1 = result_u128 + 2^120 (bit 120 set to 1) */
	uint128_t_to_hexstr(result_u128, 16, result);
	expected_result = "0100000000000001000000000081fffe";
	if (strcmp(result, expected_result) != 0) {
		fprintf(stderr, "%d: uint128_t_add() failed, got:\n\"%s\", expected:\n\"%s\"\n", __LINE__, result, expected_result);
		//FAIL();
		exit(1);
	}

	set_zero_uint128_t(test1_u128);
	test1_u128 = uint128_t_inc(test1_u128);	/* test1_u128 = (uint128_t)1 */

	set_zero_uint128_t(test2_u128);
	result_u128 = uint128_t_add(uint128_t_dec(test2_u128), test1_u128);	/* Add -1, thus subtract 1, then increment, should get 0 */
	uint128_t_to_hexstr(result_u128, 16, result);

	expected_result = "00000000000000000000000000000000";
	if (strcmp(result, expected_result) != 0) {
		fprintf(stderr, "%d: uint128_t_add() failed, got:\n\"%s\", expected:\n\"%s\"\n", __LINE__, result, expected_result);
		//FAIL();
		exit(1);
	}

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

	expected_result = "025283b4e617486899cafc2d5e8fbfff";	/* echo 'print "%x" % (0x123456789abcdef0123456789abcdef0 + 0xf01e2d3c4b5a69788796a5b4c3d2e10f)' | python */

	result_u128 = uint128_t_add(test1_u128, test2_u128);
	uint128_t_to_hexstr(result_u128, 16, result);

	if (strcmp(result, expected_result) != 0) {
		fprintf(stderr, "%d: uint128_t_add() failed, got:\n\"%s\", expected:\n\"%s\"\n", __LINE__, result, expected_result);
		//FAIL();
		exit(1);
	}

	set_zero_uint128_t(test1_u128);
	result_u128 = uint128_t_add(result_u128, test1_u128);

	uint128_t_to_hexstr(result_u128, 16, result);	/* adding 0, should get same value as before */

	if (strcmp(result, expected_result) != 0) {
		fprintf(stderr, "%d: uint128_t_add() failed, got:\n\"%s\", expected:\n\"%s\"\n", __LINE__, result, expected_result);
		//FAIL();
		exit(1);
	}

	printf("%s: tests passed\n", __func__);
}

void test_uint128_t_mixed_add_sub_inc_dec() {
	/* Mix inc/dec and add/sub */
	/* Take random value
	 * Add random, sub random, make sure equal, repeat n times */
	/* test sub -1 is inc */
	/* test add -1 is dec */
	printf("%s: tests passed\n", __func__);
}

void test_uint128_t_right_0bit_count() {
	uint128_t test_u128;
	uint8_t bit_count;
	uint8_t result;


	set_zero_uint128_t(test_u128);
	test_u128 = uint128_t_inc(test_u128);	/* test_u128 = (uint128_t)1 */

	result = uint128_t_right_0bit_count(test_u128);
	if (result != 0) {	/* LSB bit is 1... so we should count 0 */
		fprintf(stderr, "%d: uint128_t_right_0bit_count() failed, got: %u, expected: %u\n", __LINE__, result, 0);
		//FAIL();
		exit(1);
	}

	for (bit_count = 1; bit_count<=128; bit_count++) {
		test_u128 = uint128_t_left_shift(test_u128);
		result = uint128_t_right_0bit_count(test_u128);
		if (result != bit_count) {	/* LSB bit is 1... so we should count 0 */
			fprintf(stderr, "%d: uint128_t_right_0bit_count() failed, got: %u, expected: %u\n", __LINE__, result, bit_count);
			//FAIL();
			exit(1);
		}
	}

	printf("%s: tests passed\n", __func__);
}
