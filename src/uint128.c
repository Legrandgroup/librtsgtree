/**
 * file uint128.c
 */

#include "uint128.h"
#include "utils.h"
#include <stdlib.h>	// For abort()

uint128_t uint128t_zero() {
	uint128_t result_zero;

	set_zero_uint128_t(result_zero);
	return result_zero;
}

uint128_t power2_to_uint128_t(uint8_t power) {
	uint128_t result;
	uint8_t byte_index;

	assert(power<128);
	set_zero_uint128_t(result);

	byte_index = 15 - power / 8;	/* Calculate which byte to change */
	result.uint128_a8[byte_index] = 1<<(power % 8);	/* Set to 1 the appropriate bit of the matching byte */
	return result;
}

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

uint128_t uint8_t_to_uint128_t(uint8_t from) {

	uint128_t result;

	set_zero_uint128_t(result);
	result.uint128_a8[sizeof(result.uint128_a8)-1] = from;
	return result;
}

uint128_t uint16_t_to_uint128_t(uint16_t from) {

	uint128_t result;

	set_zero_uint128_t(result);
	result.uint128_a8[sizeof(result.uint128_a16)-1] = (uint8_t)(from & 0xff);
	result.uint128_a8[sizeof(result.uint128_a16)-2] = (uint8_t)(from >> 8);
	return result;
}

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
		uint8_t_to_binstr(input.uint128_a8[15-nb_full_bytes_total],	/* nb_full_bytes_total can be up to 15 (16 would mean nb_bits_in_additional_MSB==0) */
		                  nb_bits_in_additional_MSB,
		                  output);

	//output[nb_bits_in_additional_MSB]='\0'; // For debug only
	already_parsed_full_bytes = 0;
	while (already_parsed_full_bytes<nb_full_bytes_total) {	/* Parse all bytes inside uint128_t... starting from LSB down to byte 7 (appended at end of the string) */
		uint8_t_to_binstr(input.uint128_a8[15-nb_full_bytes_total + already_parsed_full_bytes + 1],	/* Get the next byte out of the uint128_t... we parse from MSB (left most) to LSB (right most, or byte index 15) */
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

	for (current_index = sizeof(input.uint128_a8)-nb_bytes; current_index<sizeof(input.uint128_a8); current_index++) {
		current_byte = input.uint128_a8[current_index];

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
