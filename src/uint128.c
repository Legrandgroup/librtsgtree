/**
 * file uint128.c
 */

#include "uint128.h"
#include <assert.h>	// For assert()
#include <stdlib.h>	// For abort()

inline uint128_t uint128_t_zero() {
	uint128_t result_zero;

	U128_SET_ZERO(result_zero);
	return result_zero;
}

inline uint128_t uint128_t_max() {
	uint128_t result_max;

	U128_SET_MAX(result_max);
	return result_max;
}

uint8_t uint128_t_get_byte_no(const uint128_t input, const uint8_t byte_no) {
	assert(byte_no<16);
#ifndef HAS_INT128	// For platforms that do not support native 128-bit integers arithmetic
	return input.uint128_a8[15-byte_no];
#else	// HAS_INT128
	return (input >> (byte_no*8)) & 0xff;
#endif
}

#ifndef HAS_INT128	// For platforms that do not support native 128-bit integers arithmetic
uint128_t power2_to_uint128_t(uint8_t power) {
	uint128_t result;
	uint8_t byte_index;

	assert(power<128);
	U128_SET_ZERO(result);

	byte_index = 15 - power / 8;	/* Calculate which byte to change */
	result.uint128_a8[byte_index] = 1<<(power % 8);	/* Set to 1 the appropriate bit of the matching byte */
	return result;
}
#else	// HAS_INT128
inline uint128_t power2_to_uint128_t(uint8_t power) {
	return ((uint128_t)1)<<power;
}
#endif

#ifndef HAS_INT128	// For platforms that do not support native 128-bit integers arithmetic
uint128_t uint128_t_right_shift(const uint128_t input) {

	  uint128_t output = input;
	  register int8_t byte_index;
	  uint8_t nb_bytes;

	  nb_bytes = sizeof(output.uint128_a8);
	  for (byte_index = (int8_t)(nb_bytes)-1; byte_index>=0; byte_index--) {
		  /* Start from LSB ([15]) to MSB ([0]) */
		  output.uint128_a8[byte_index] >>= 1;
		  if (byte_index != 0)	/* Not on last byte shifted */
			  /* Then propagate lower significant bit from previous byte */
			  if (output.uint128_a8[byte_index-1] & 0x01)
				  output.uint128_a8[byte_index] |= 0x80;
	  }

	  return output;
}
#else	// HAS_INT128
inline uint128_t uint128_t_right_shift(const uint128_t input) {
	return input>>1;
}
#endif

#ifndef HAS_INT128	// For platforms that do not support native 128-bit integers arithmetic
uint128_t uint128_t_right_shift_n(const uint128_t input, uint8_t n) {

	uint128_t result = input;
	while (n--!=0)
		result = uint128_t_right_shift(result);
	return result;
}
#else	// HAS_INT128
inline uint128_t uint128_t_right_shift_n(const uint128_t input, uint8_t n) {
	if (n==128)
		return 0;	/* native shifting 128 bits is broken on some platforms */
	else
		return input>>n;
}
#endif

#ifndef HAS_INT128	// For platforms that do not support native 128-bit integers arithmetic
uint128_t uint128_t_left_shift(const uint128_t input) {

	  uint128_t output = input;
	  register int8_t byte_index;
	  uint8_t nb_bytes;

	  nb_bytes = sizeof(output.uint128_a8);
	  for (byte_index = 0; byte_index<(int8_t)(nb_bytes); byte_index++) {
		  /* Start from MSB ([0]) to LSB ([15]) */
		  output.uint128_a8[byte_index] <<= 1;
		  if (byte_index != nb_bytes-1)	/* Not on last byte shifted */
			  /* Then propagate lower significant bit from previous byte */
			  if (output.uint128_a8[byte_index+1] & 0x80)
				  output.uint128_a8[byte_index] |= 0x01;
	  }

	  return output;
}
#else	// HAS_INT128
inline uint128_t uint128_t_left_shift(const uint128_t input) {
	return input<<1;
}
#endif

#ifndef HAS_INT128	// For platforms that do not support native 128-bit integers arithmetic
uint128_t uint128_t_left_shift_n(const uint128_t input, uint8_t n) {

	uint128_t result = input;
	while (n--!=0)
		result = uint128_t_left_shift(result);
	return result;
}
#else	// HAS_INT128
inline uint128_t uint128_t_left_shift_n(const uint128_t input, uint8_t n) {
	if (n==128)
		return 0;	/* native shifting 128 bits is broken on some platforms */
	else
		return input<<n;
}
#endif

#ifndef HAS_INT128	// For platforms that do not support native 128-bit integers arithmetic
uint128_t uint128_t_inc(const uint128_t input) {

	  uint128_t output = input;
	  register int8_t byte_index;
	  uint8_t index_of_last_byte = sizeof(output.uint128_a8) - 1;
	  register int carry = 0;

	  carry = 0;
	  for (byte_index = index_of_last_byte; byte_index>=0; byte_index--) {
		  if ((byte_index == index_of_last_byte) || carry) {	/* We are on LSB... so we should start the increment here, or we are propagating a carry up */
			  carry = (output.uint128_a8[byte_index] == 0xff);	/* Should carry if overflow occurs on LSB */
			  output.uint128_a8[byte_index]++;
		  }
		  else {
			  break;
		  }
	  }
	  return output;
}
#else	// HAS_INT128
inline uint128_t uint128_t_inc(const uint128_t input) {
	return input+1;
}
#endif

#ifndef HAS_INT128	// For platforms that do not support native 128-bit integers arithmetic
uint128_t uint128_t_dec(const uint128_t input) {

	  uint128_t output = input;
	  register int8_t byte_index;
	  uint8_t index_of_last_byte = sizeof(output.uint128_a8) - 1;
	  register int carry = 0;

	  for (byte_index = index_of_last_byte; byte_index>=0; byte_index--) {
		  if ((byte_index == index_of_last_byte) || carry) {	/* We are on LSB... so we should start decrementing here, or we are propagating a carry up */
			  carry = (output.uint128_a8[byte_index] == 0x00);	/* Should carry if underflow occurs on LSB */
			  output.uint128_a8[byte_index]--;
		  }
		  else {
			  break;
		  }
	  }
	  return output;
}
#else	// HAS_INT128
inline uint128_t uint128_t_dec(const uint128_t input) {
	return input-1;
}
#endif

#ifndef HAS_INT128	// For platforms that do not support native 128-bit integers arithmetic
uint128_t uint128_t_sub(const uint128_t from, uint128_t subtraction) {
	  uint128_t output;
	  register int8_t byte_index;
	  uint8_t index_of_last_byte = sizeof(output.uint128_a8) - 1;
	  register int carry = 0;

	  for (byte_index = index_of_last_byte; byte_index>=0; byte_index--) {
		  if (carry) {	/* We are on LSB... so we should start subtracting here, or we are propagating a carry up */
			  if (++(subtraction.uint128_a8[byte_index]) == 0) {
				  /* If a carry needs to be propagated, we add it to the value to subtract */
				  /* Overflow occurred (we get 0), then keep carry for next byte */
			  }
			  else
				  carry = 0;	/* No carry anymore, we have propagated the previous byte's carry */
		  }
		  if (from.uint128_a8[byte_index] < subtraction.uint128_a8[byte_index]) {	/* We should propagate a carry */
			  carry++;
		  }
		  output.uint128_a8[byte_index] = from.uint128_a8[byte_index] - subtraction.uint128_a8[byte_index];
	  }
	  return output;
}
#else	// HAS_INT128
inline uint128_t uint128_t_sub(const uint128_t from, uint128_t subtraction) {
	return from-subtraction;
}
#endif

#ifndef HAS_INT128	// For platforms that do not support native 128-bit integers arithmetic
uint128_t uint128_t_add(const uint128_t first, uint128_t second) {
	  uint128_t output;
	  register int8_t byte_index;
	  uint8_t index_of_last_byte = sizeof(output.uint128_a8) - 1;
	  register int carry = 0;

	  for (byte_index = index_of_last_byte; byte_index>=0; byte_index--) {
		  if (carry) {	/* We are on LSB... so we should start adding  here, or we are propagating a carry up */
			  if (++(second.uint128_a8[byte_index]) == 0) {
				  /* If a carry needs to be propagated, we add it to the value to add */
				  /* Overflow occurred (we get 0), then keep carry for next byte */
			  }
			  else
				  carry = 0;	/* No carry anymore, we have propagated the previous byte's carry */
		  }
		  output.uint128_a8[byte_index] = first.uint128_a8[byte_index] + second.uint128_a8[byte_index];
		  if (output.uint128_a8[byte_index] < first.uint128_a8[byte_index]) {	/* Result of addition is lower than the initial value... this is the symptom of overflow, so we should propagate a carry */
			  carry++;
		  }
	  }
	  return output;
}
#else	// HAS_INT128
uint128_t uint128_t_add(const uint128_t first, uint128_t second) {
	return first+second;
}
#endif

#ifndef HAS_INT128	// For platforms that do not support native 128-bit integers arithmetic
uint128_t uint128_t_or(const uint128_t first, uint128_t second) {
	  register int8_t byte_index;
	  uint8_t index_of_last_byte = sizeof(first.uint128_a8) - 1;

	  for (byte_index = index_of_last_byte; byte_index>=0; byte_index--)
		  second.uint128_a8[byte_index] |= first.uint128_a8[byte_index];
	  return second;
}
#else	// HAS_INT128
inline uint128_t uint128_t_or(const uint128_t first, uint128_t second) {
	return first|second;
}
#endif

#ifndef HAS_INT128	// For platforms that do not support native 128-bit integers arithmetic
uint128_t uint128_t_and(const uint128_t first, uint128_t second) {
	  register int8_t byte_index;
	  uint8_t index_of_last_byte = sizeof(first.uint128_a8) - 1;

	  for (byte_index = index_of_last_byte; byte_index>=0; byte_index--)
		  second.uint128_a8[byte_index] &= first.uint128_a8[byte_index];
	  return second;
}
#else	// HAS_INT128
uint128_t uint128_t_and(const uint128_t first, uint128_t second) {
	return first&second;
}
#endif

#ifndef HAS_INT128	// For platforms that do not support native 128-bit integers arithmetic
uint128_t uint8_t_to_uint128_t(uint8_t from) {

	uint128_t result;

	U128_SET_ZERO(result);
	result.uint128_a8[sizeof(result.uint128_a8)-1] = from;
	return result;
}
#else	// HAS_INT128
uint128_t uint8_t_to_uint128_t(uint8_t from) {
	return (uint128_t)from;
}
#endif

#ifndef HAS_INT128	// For platforms that do not support native 128-bit integers arithmetic
uint128_t uint16_t_to_uint128_t(uint16_t from) {

	uint128_t result;

	U128_SET_ZERO(result);
	result.uint128_a8[sizeof(result.uint128_a16)-1] = (uint8_t)(from & 0xff);
	result.uint128_a8[sizeof(result.uint128_a16)-2] = (uint8_t)(from >> 8);
	return result;
}
#else	// HAS_INT128
uint128_t uint16_t_to_uint128_t(uint16_t from) {
	return (uint128_t)from;
}
#endif

void uint8_t_to_binstr(uint8_t input, const uint8_t bits_no, char output[9]) {

	register uint8_t pos;

	assert(output);
	//assert(bits_no>=0); // Not required, we are working with an unsigned int
	assert(bits_no<=8);

	input <<= (8-bits_no);	/* Set the MSB directly to the first bit we want to output */
	for (pos=0; pos<bits_no; pos++) {
		if (input & 0x80)	/* Check the MSB (bit 7 = 0x80) */
			output[pos] = '1';
		else
			output[pos] = '0';
		input <<= 1;	/* Work on the next MSB as bit 16 */
	}
	output[pos] = '\0';	/* Terminate string */
}

void uint128_t_to_binstr(const uint128_t input, const uint8_t nb_bits, char* output) {

	register uint8_t already_parsed_full_bytes;
	uint8_t nb_full_bytes_total;
	uint8_t nb_bits_in_additional_MSB;

	assert(output);
	//assert(nb_bits>=0); // Not required, we are working with an unsigned int
	assert(nb_bits<=128);

	nb_full_bytes_total = nb_bits / 8;
	nb_bits_in_additional_MSB = nb_bits % 8;

	if (nb_bits_in_additional_MSB)	/* If the highest MSB is incomplete... first dump the few MSbits at the head of the string */
		uint8_t_to_binstr(uint128_t_get_byte_no(input, nb_full_bytes_total),	/* nb_full_bytes_total can be up to 15 (16 would mean nb_bits_in_additional_MSB==0) */
		                  nb_bits_in_additional_MSB,
		                  output);

	//output[nb_bits_in_additional_MSB]='\0'; // For debug only
	already_parsed_full_bytes = 0;
	while (already_parsed_full_bytes<nb_full_bytes_total) {	/* Parse all bytes inside uint128_t... starting from LSB down to byte 7 (appended at end of the string) */
		uint8_t_to_binstr(uint128_t_get_byte_no(input, nb_full_bytes_total - already_parsed_full_bytes - 1),	/* Get the next byte out of the uint128_t... we parse from MSB (left most) to LSB (right most, or byte 0) */
		                  8,
		                  &(output[nb_bits_in_additional_MSB + already_parsed_full_bytes*8]));
		//The following is for debug
		//output[nb_bits_in_additional_MSB + (already_parsed_full_bytes+1)*8]='\0'; printf("Current string constructed after alpfw=%d is \"%s\"\n", already_parsed_full_bytes, output);
		already_parsed_full_bytes++;
	}
	output[nb_bits_in_additional_MSB + already_parsed_full_bytes*8] = '\0';	/* Terminate C-string */
}

void uint128_t_to_hexstr(const uint128_t input, const uint8_t nb_bytes, char* output) {

	uint8_t current_index;
	uint8_t current_byte;
	uint8_t hi_nibble;
	uint8_t lo_nibble;

	assert(output);
	//assert(nb_bytes>=0); // Not required, we are working with an unsigned int
	assert(nb_bytes<=16);

	for (current_index = 16-nb_bytes; current_index<16; current_index++) {
		current_byte = uint128_t_get_byte_no(input, 15-current_index);

		hi_nibble = current_byte >> 4;
		lo_nibble = current_byte & 0x0f;

		if (hi_nibble < 0x0a)
			*output++ = hi_nibble + '0';	/* Output numeric digit */
		else
			*output++ = hi_nibble - 0x0a + 'a';	/* Output hex digit */
		if (lo_nibble < 0x0a)
			*output++ = lo_nibble + '0';	/* Output numeric digit */
		else
			*output++ = lo_nibble - 0x0a + 'a';	/* Output hex digit */
	}
	*output = '\0';	/* Terminate C-string */
}

#ifndef HAS_INT128	// For platforms that do not support native 128-bit integers arithmetic
uint8_t uint128_t_right_0bit_count(const uint128_t input) {

	uint8_t result = 0;
	uint8_t byte_index;
	uint8_t mask;
	uint8_t index_of_last_byte = sizeof(input.uint128_a8) - 1;

	byte_index = index_of_last_byte;
	while (1) {
		if (input.uint128_a8[byte_index] == 0) {
			/* We have a full byte set to 0... continue on higher significant byte... count 8 bits */
			result+=8;
		}
		else {	/* The current byte is not full... count the right 0 bits */
			for (mask = 1; mask != 0; mask<<=1) {	/* Start from bit 0 to bit 7 */
				if ((input.uint128_a8[byte_index] & mask) == 0) {
					result++;
				}
				else
					return result;
			}
		}
		if (byte_index ==0)
			break;
		else
			byte_index--;
	}

	return result;
}
#else	// HAS_INT128
uint8_t uint128_t_right_0bit_count(const uint128_t input) {

	uint128_t input_copy = input;
	uint8_t result = 0;

	if (input == 0)
		return 128;

	/* If we get here, we know there is at least one bit set */
	while ((input_copy & 0xffffffff) == 0) {
		result+=32;
		input_copy>>=32;
	}
	while ((input_copy & 0xff) == 0) {
		result+=8;
		input_copy>>=8;
	}
	while ((input_copy & 0x1) == 0) {
		result++;
		input_copy>>=1;
	}
	return result;
}
#endif

#ifndef HAS_INT128	// For platforms that do not support native 128-bit integers arithmetic
uint128_t msb_1bits_to_uint128_t(uint8_t n) {
	uint8_t byte_pos;
	uint128_t result;

	assert(n<=128);

	for (byte_pos = 0; n >= 8; byte_pos++, n-=8) {
		result.uint128_a8[byte_pos] = 0xff;	/* Fill all full bytes */
	}
	if (n!=0) {	/* If there are remaining bits */
		assert(n<=8);
		assert(byte_pos<sizeof(result.uint128_a8));	/* Make sure we don't overflow... should never occur */
		result.uint128_a8[byte_pos] = 0xff << (8-n);
		byte_pos++;
	}
	for (; byte_pos<sizeof(result.uint128_a8); byte_pos++) {
		result.uint128_a8[byte_pos] = 0;	/* Fill the remaining bytes with 0 */
	}
	return result;
}
#else	// HAS_INT128
uint128_t msb_1bits_to_uint128_t(uint8_t n) {
	return ((uint128_t)1<<n)-1;
}
#endif

#ifndef HAS_INT128	// For platforms that do not support native 128-bit integers arithmetic
int uint128_t_cmp(const uint128_t first, uint128_t second) {
	  register int8_t byte_index;
	  uint8_t index_of_last_byte = sizeof(first.uint128_a8) - 1;

	  for (byte_index = index_of_last_byte; byte_index>=0; byte_index--) {
		  if (first.uint128_a8[byte_index] > second.uint128_a8[byte_index])
			  return 1;
		  if (first.uint128_a8[byte_index] < second.uint128_a8[byte_index])
			  return -1;
	  }
	  return 0;
}
#else	// HAS_INT128
int uint128_t_cmp(const uint128_t first, uint128_t second) {
	if (first==second) return 0;
	if (first>second) return 1;
	return -1;
}
#endif

#ifndef HAS_INT128	// For platforms that do not support native 128-bit integers arithmetic
inline uint128_t uint128_t_hton(const uint128_t input) {
	return input;	/* Built-in uint128_t struct (not native) stores the 128 bits in network order */
}
#else // HAS_INT128
inline uint128_t uint128_t_hton(const uint128_t input) {
#ifdef IS_BIG_ENDIAN
	return input;
#elif IS_LITTLE_ENDIAN
	uint128_t output;
	((uint8_t *)&output)[0] = ((uint8_t *)&input)[15];
	((uint8_t *)&output)[1] = ((uint8_t *)&input)[14];
	((uint8_t *)&output)[2] = ((uint8_t *)&input)[13];
	((uint8_t *)&output)[3] = ((uint8_t *)&input)[12];
	((uint8_t *)&output)[4] = ((uint8_t *)&input)[11];
	((uint8_t *)&output)[5] = ((uint8_t *)&input)[10];
	((uint8_t *)&output)[6] = ((uint8_t *)&input)[9];
	((uint8_t *)&output)[7] = ((uint8_t *)&input)[8];
	((uint8_t *)&output)[8] = ((uint8_t *)&input)[7];
	((uint8_t *)&output)[9] = ((uint8_t *)&input)[6];
	((uint8_t *)&output)[10] = ((uint8_t *)&input)[5];
	((uint8_t *)&output)[11] = ((uint8_t *)&input)[4];
	((uint8_t *)&output)[12] = ((uint8_t *)&input)[3];
	((uint8_t *)&output)[13] = ((uint8_t *)&input)[2];
	((uint8_t *)&output)[14] = ((uint8_t *)&input)[1];
	((uint8_t *)&output)[15] = ((uint8_t *)&input)[0];
	return output;
#else // unspecified endianness... use an endianness-agnostic version
	uint128_t output;
	((uint8_t *)&output)[0] = input & 0xff;
	((uint8_t *)&output)[1] = (input>>8) & 0xff;
	((uint8_t *)&output)[2] = (input>>16) & 0xff;
	((uint8_t *)&output)[3] = (input>>24) & 0xff;
	((uint8_t *)&output)[4] = (input>>32) & 0xff;
	((uint8_t *)&output)[5] = (input>>40) & 0xff;
	((uint8_t *)&output)[6] = (input>>48) & 0xff;
	((uint8_t *)&output)[7] = (input>>56) & 0xff;
	((uint8_t *)&output)[8] = (input>>64) & 0xff;
	((uint8_t *)&output)[9] = (input>>72) & 0xff;
	((uint8_t *)&output)[10] = (input>>80) & 0xff;
	((uint8_t *)&output)[11] = (input>>88) & 0xff;
	((uint8_t *)&output)[12] = (input>>96) & 0xff;
	((uint8_t *)&output)[13] = (input>>104) & 0xff;
	((uint8_t *)&output)[14] = (input>>112) & 0xff;
	((uint8_t *)&output)[15] = (input>>120) & 0xff;
	return output;
#endif
}
#endif // HAS_INT128
