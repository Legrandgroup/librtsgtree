/**
 * \file uint128.h
 * \brief Manipulation of 128-bit integers on platforms that do not support native 128-bit arithmetic
 */

#ifndef __UINT128_H__
#define __UINT128_H__

#include <stdint.h>	// For uint*_t
#include <netinet/in.h>	// For struct in6_addr

#ifdef HAS_INT128	// For platforms that do not support native 128-bit integers arithmetic

#ifndef IS_BIG_ENDIAN
#ifndef IS_LITTLE_ENDIAN
#warning IS_BIG_ENDIAN or IS_LITTLE_ENDIAN compiler directives should be set in order to optimise this library with native 128-bit integers
#endif
#endif
#ifdef IS_BIG_ENDIAN
#ifdef IS_LITTLE_ENDIAN
#error IS_BIG_ENDIAN and IS_LITTLE_ENDIAN compiler directives cannot be simultaneously
#endif
#endif

#endif // HAS_INT128

#ifndef HAS_INT128	// For platforms that do not support native 128-bit integers arithmetic
#define HAS_UINT8	//!< directive used for platforms that have uint8_t types (hopefully most)
// #define HAS_UINT16
// #define HAS_UINT16BE
// #define HAS_UINT32BE
/**
 * \brief A type container for 128-bit wide unsigned integers
**/
typedef struct {
	union {
		uint16_t __split_16bits[8];	//!< The value stored by this 128-bit integer accessed as an array of 16-bit elements
		uint8_t  __split_8bits[16];	//!< The value stored by this 128-bit integer accessed as an array of bytes
	} __uint128_u;	//!< A union to select the splitting the content of the 128-bits when accessing its internal parts
} uint128_t;

/** \def uint128_a8
 * Allows access to the 128 bits of a uint128_t
 * The access is done via an array, byte per byte, in network order (index 0 is the MSB, index 15 is the LSB)
 */
#define uint128_a8  __uint128_u.__split_8bits
/** \def uint128_a16
 * Allows access to the 128 bits of a uint128_t
 * The access is done via an array 16-bit word per 16-bit word, in network order (index 0 is the MSB, index 15 is the LSB).
 * 16-bit words are stored in network order in order (use htons) for code to be platform independent
 */
#define uint128_a16 __uint128_u.__split_16bits
/* Lionel: on 32, 64bits archs, we could also perform operations on larger words for optimization */

/**
 * \def U128_SET_ZERO(n)
 *
 * \brief Assign the value zero to an uint128_t variable \p n
 *
 * \param n The uint128_t variable to set
 */
#define U128_SET_ZERO(n) \
	do { \
		/*assert(typeof(n) == uint128_t);*/ \
		n.uint128_a16[0] = 0; \
		n.uint128_a16[1] = 0; \
		n.uint128_a16[2] = 0; \
		n.uint128_a16[3] = 0; \
		n.uint128_a16[4] = 0; \
		n.uint128_a16[5] = 0; \
		n.uint128_a16[6] = 0; \
		n.uint128_a16[7] = 0; \
	} while(0)

/**
 * \def U128_SET_MAX(n)
 *
 * \brief Assign the value -1 to an uint128_t variable \p n
 *
 * \param n The uint128_t variable to set
 */
#define U128_SET_MAX(n) \
	do { \
		/*assert(typeof(n) == uint128_t);*/ \
		n.uint128_a16[0] = 0xffff; \
		n.uint128_a16[1] = 0xffff; \
		n.uint128_a16[2] = 0xffff; \
		n.uint128_a16[3] = 0xffff; \
		n.uint128_a16[4] = 0xffff; \
		n.uint128_a16[5] = 0xffff; \
		n.uint128_a16[6] = 0xffff; \
		n.uint128_a16[7] = 0xffff; \
	} while(0)

/**
 * \def U128_IS_ZERO(n)
 *
 * \brief Check if an uint128_t variable \p n equals 0
 *
 * \param n The uint128_t variable to check
 */
#define U128_IS_ZERO(n) \
	(	n.uint128_a16[0] == 0 && \
		n.uint128_a16[1] == 0 && \
		n.uint128_a16[2] == 0 && \
		n.uint128_a16[3] == 0 && \
		n.uint128_a16[4] == 0 && \
		n.uint128_a16[5] == 0 && \
		n.uint128_a16[6] == 0 && \
		n.uint128_a16[7] == 0 )

/**
 * \def U128_IS_MAX(n)
 *
 * \brief Check if an uint128_t variable \p n equals -1
 *
 * \param n The uint128_t variable to check
 */
#define U128_IS_MAX(n) \
	(	n.uint128_a16[0] == 0xffff && \
		n.uint128_a16[1] == 0xffff && \
		n.uint128_a16[2] == 0xffff && \
		n.uint128_a16[3] == 0xffff && \
		n.uint128_a16[4] == 0xffff && \
		n.uint128_a16[5] == 0xffff && \
		n.uint128_a16[6] == 0xffff && \
		n.uint128_a16[7] == 0xffff )

#else	// HAS_INT128
/* Note: gcc or clang, on 64 bit platforms they have a builtin 128 bit types that come for free, __uint128_t and __int128_t */
typedef unsigned __int128 uint128_t;

/**
 * \def U128_SET_ZERO(n)
 *
 * \brief Assign the value zero to an uint128_t variable \p n
 *
 * \param n The uint128_t variable to set
 */
#define U128_SET_ZERO(n) do { n=0; } while(0)

/**
 * \def U128_SET_MAX(n)
 *
 * \brief Assign the value -1 to an uint128_t variable \p n
 *
 * \param n The uint128_t variable to set
 */
#define U128_SET_MAX(n) do { n=(uint128_t)-1; } while(0)

/**
 * \def U128_IS_ZERO(n)
 *
 * \brief Check if an uint128_t variable \p n equals 0
 *
 * \param n The uint128_t variable to check
 */
#define U128_IS_ZERO(n) (n == 0)

/**
 * \def U128_IS_MAX(n)
 *
 * \brief Check if an uint128_t variable \p n equals -1
 *
 * \param n The uint128_t variable to check
 */
#define U128_IS_MAX(n) (n == (uint128_t)-1)
#endif	// HAS_INT128

/**
 * \brief Extract a specific byte from a uint128_t integer
 *
 * \param input The uint128_t variable from which we perform the extraction
 * \param byte_no The byte number we want to extract (value should be between 15 for the most significant byte, and 0 for the least significant byte)
 *
 * \return The byte extracted from \p input
 */
uint8_t uint128_t_get_byte_no(const uint128_t input, const uint8_t byte_no);

/**
 * \brief Return 0 as an uint128_t
 *
 * \return (uint128_t)0
 */
static inline uint128_t uint128_t_zero() {
	uint128_t result_zero;

	U128_SET_ZERO(result_zero);
	return result_zero;
}

/**
 * \brief Return -1 as an uint128_t
 *
 * \return (uint128_t)-1
 */
static inline uint128_t uint128_t_max() {
	uint128_t result_max;

	U128_SET_MAX(result_max);
	return result_max;
}

/**
 * \brief Calculate 2 ^ power for a 128-bit wide unsigned int
 *
 * \param power The power of 2 to use
 * \return 2 ^ \p power as a 128-bit wide unsigned int
**/
#ifdef HAS_INT128
static
#endif
uint128_t power2_to_uint128_t(uint8_t power);

// For native int128, we inline this function so it is defined here and not in the .c file
#ifdef HAS_INT128
static inline uint128_t power2_to_uint128_t(uint8_t power) {
	return ((uint128_t)1)<<power;
}
#endif

/**
 * \brief Right shift a uint128_t of 1 bit
 *
 * \param input The uint128_t to shift
 *
 * \return The resulting uint128_t once bit-shifted
**/
#ifdef HAS_INT128
static
#endif
uint128_t uint128_t_right_shift(const uint128_t input);

// For native int128, we inline this function so it is defined here and not in the .c file
#ifdef HAS_INT128
static inline uint128_t uint128_t_right_shift(const uint128_t input) {
	return input>>1;
}
#endif

/**
 * \brief Right shift a uint128_t of \p n bits
 *
 * \param input The uint128_t to shift
  * \param n How many bits to shift
*
 * \return The resulting uint128_t once bit-shifted
**/
#ifdef HAS_INT128
static
#endif
uint128_t uint128_t_right_shift_n(const uint128_t input, uint8_t n);

// For native int128, we inline this function so it is defined here and not in the .c file
#ifdef HAS_INT128
static inline uint128_t uint128_t_right_shift_n(const uint128_t input, uint8_t n) {
	if (n==128)
		return 0;	/* native shifting 128 bits is broken on some platforms */
	else
		return input>>n;
}
#endif

/**
 * \brief Left shift a uint128_t of 1 bit
 *
 * \param input The uint128_t to shift
 *
 * \return The resulting uint128_t once bit-shifted
**/
#ifdef HAS_INT128
static
#endif
uint128_t uint128_t_left_shift(const uint128_t input);

// For native int128, we inline this function so it is defined here and not in the .c file
#ifdef HAS_INT128
static inline uint128_t uint128_t_left_shift(const uint128_t input) {
	return input<<1;
}
#endif

/**
 * \brief Left shift a uint128_t of \p n bits
 *
 * \param input The uint128_t to shift
 * \param n How many bits to shift
 *
 * \return The resulting uint128_t once bit-shifted
**/
#ifdef HAS_INT128
static
#endif
uint128_t uint128_t_left_shift_n(const uint128_t input, uint8_t n);

// For native int128, we inline this function so it is defined here and not in the .c file
#ifdef HAS_INT128
static inline uint128_t uint128_t_left_shift_n(const uint128_t input, uint8_t n) {
	if (n==128)
		return 0;	/* native shifting 128 bits is broken on some platforms */
	else
		return input<<n;
}
#endif

/**
 * \brief Increment a uint128_t
 *
 * \param input The uint128_t to increment
 *
 * \return The result of \p input + 1 in a uint128_t
**/
#ifdef HAS_INT128
static
#endif
uint128_t uint128_t_inc(const uint128_t input);

// For native int128, we inline this function so it is defined here and not in the .c file
#ifdef HAS_INT128
static inline uint128_t uint128_t_inc(const uint128_t input) {
	return input+1;
}
#endif

/**
 * \brief Decrement a uint128_t
 *
 * \param input The uint128_t to decrement
 *
 * \return The result of \p input - 1 in a uint128_t
**/
#ifdef HAS_INT128
static
#endif
uint128_t uint128_t_dec(const uint128_t input);

// For native int128, we inline this function so it is defined here and not in the .c file
#ifdef HAS_INT128
static inline uint128_t uint128_t_dec(const uint128_t input) {
	return input-1;
}
#endif

/**
 * \brief Calculate the subtraction between two uint128_t
 *
 * Note: underflow may occur... we handle positive integers only, so it is up to the caller to handle this
 *
 * \param from The uint128_t to subtract from
 * \param subtraction The uint128_t to subtract
 *
 * \return The result of (from - subtraction) as an uint128_t
**/
#ifdef HAS_INT128
static
#endif
uint128_t uint128_t_sub(const uint128_t from, uint128_t subtraction);

// For native int128, we inline this function so it is defined here and not in the .c file
#ifdef HAS_INT128
static inline uint128_t uint128_t_sub(const uint128_t from, uint128_t subtraction) {
	return from-subtraction;
}
#endif

/**
 * \brief Calculate the addition between two uint128_t
 *
 * Note: overflow may occur... we handle positive integers only, so it is up to the caller to handle this
 *
 * \param first The first operand of the addition
 * \param second The second operand of the addition
 *
 * \return The result of (first + second) as an uint128_t
**/
#ifdef HAS_INT128
static
#endif
uint128_t uint128_t_add(const uint128_t first, uint128_t second);

// For native int128, we inline this function so it is defined here and not in the .c file
#ifdef HAS_INT128
static inline uint128_t uint128_t_add(const uint128_t first, uint128_t second) {
	return first+second;
}
#endif

/**
 * \brief Calculate the bitwise or operation between two uint128_t
 *
 * \param first The first operand of the bitwise or
 * \param second The second operand of the bitwise or
 *
 * \return The result of (first | second) as an uint128_t
**/
#ifdef HAS_INT128
static
#endif
uint128_t uint128_t_or(const uint128_t first, uint128_t second);

// For native int128, we inline this function so it is defined here and not in the .c file
#ifdef HAS_INT128
static inline uint128_t uint128_t_or(const uint128_t first, uint128_t second) {
	return first|second;
}
#endif

/**
 * \brief Calculate the bitwise and operation between two uint128_t
 *
 * \param first The first operand of the bitwise and
 * \param second The second operand of the bitwise and
 *
 * \return The result of (first & second) as an uint128_t
**/
#ifdef HAS_INT128
static
#endif
uint128_t uint128_t_and(const uint128_t first, uint128_t second);

// For native int128, we inline this function so it is defined here and not in the .c file
#ifdef HAS_INT128
static inline uint128_t uint128_t_and(const uint128_t first, uint128_t second) {
	return first&second;
}
#endif

/**
 * \brief Create a uint128_t from a uint8_t
 *
 * \param from The uint8_t value to store into a uint128_t
 *
 * \return the resulting uint128_t
**/
#ifdef HAS_INT128
static
#endif
uint128_t uint8_t_to_uint128_t(uint8_t from);

// For native int128, we inline this function so it is defined here and not in the .c file
#ifdef HAS_INT128
static inline uint128_t uint8_t_to_uint128_t(uint8_t from) {
	return (uint128_t)from;
}
#endif

/**
 * \brief Create a uint128_t from a uint16_t
 *
 * \param from The uint16_t value to store into a uint128_t
 *
 * \return the resulting uint128_t
**/
#ifdef HAS_INT128
static
#endif
uint128_t uint16_t_to_uint128_t(uint16_t from);

// For native int128, we inline this function so it is defined here and not in the .c file
#ifdef HAS_INT128
static inline uint128_t uint16_t_to_uint128_t(uint16_t from) {
	return (uint128_t)from;
}
#endif

/**
 * \brief Dump a uint16_t word to a binary representation in a char*
 *
 * The buffer \p output will be filled with the 16-character long representation of the 16-bit word
 *
 * \param[in] input The 16-bit word to dump
 * \param[in] bits_no The number of LSB to dump (value accepted 0 to 16)
 * \param[out] output A pointer to a 33-byte array where the resulting string will be stored
**/
void uint8_t_to_binstr(uint8_t input, const uint8_t bits_no, char output[9]);

/**
 * \brief Dump a uint128_t into a binary representation in a char*
 *
 * \param[in] input The uint128_t to dump
 * \param[in] nb_bits The number of LSB to dump (value accepted 0 to 128)
 * \param[out] output A pointer to an array allocated with nb_bits+1 bytes where the resulting string will be stored
**/
void uint128_t_to_binstr(const uint128_t input, const uint8_t nb_bits, char* output);

/**
 * \brief Dump a uint128_t into a hexadecimal representation in a char*
 *
 * The \p output will be in lowercase and without any 0x prefix
 *
 * \param[in] input The uint128_t to dump
 * \param[in] nb_bytes The number of LSB to dump (value accepted 0 to 16)
 * \param[out] output A pointer to an array allocated with (nb_bytes*2)+1 bytes where the resulting string will be stored
**/
void uint128_t_to_hexstr(const uint128_t input, const uint8_t nb_bytes, char* output);

/**
 * \brief Count the number of lower significant bits set to 0 in \p input
 *
 * \param input The uint128_t on which to perform the calculation
 *
 * \return The number of bits set to 0 (will be >=0 and <=128)
**/
uint8_t uint128_t_right_0bit_count(const uint128_t input);

/**
 * \brief Create a uint128_t value with the \n high significant bits to 1 and all other to 0
 *
 * \param n The number of bits to set to 1
 *
 * \return The resulting uint128_t
**/
uint128_t msb_1bits_to_uint128_t(uint8_t n);

/**
 * \brief Compare \p first uint128_t and \p second uint128_t
 *
 * Note: We work on unsigned integers only, so be careful: possible "negative" results passed as \p first or \p second will not lead to expected result
 *
 * \param first The first operand of the comparison
 * \param second The second operand of the comparison
 *
 * \return -1 if \p first is strictly lower than \p second, 0 is they are equal, and 1 if \p first is strictly higher than \p second
**/
int uint128_t_cmp(const uint128_t first, const uint128_t second);

/**
 * \brief Convert a uint128_t from host order (with associated host-specific endianness) to network order (big endian)
 *
 * \param input The uint128_t to convert to network order
 *
 * \result The corresponding network order
 */
static inline uint128_t uint128_t_hton(const uint128_t input) {
#if !defined(HAS_INT128) || defined(IS_BIG_ENDIAN)
/* For platforms that do not support native 128-bit integers arithmetic, we are using the library to handle uint128_t, and it stores data in network order
 * For big endian arch, we already store data in network order
 * In both cases, do nothing... output is input */
	return input;	/* Built-in uint128_t struct (not native) stores the 128 bits in network order */
#else // HAS_INT128 is defined and we IS_BIG_ENDIAN is false
	uint128_t output;	/* We will need an intermediate variable to store results */
#if defined(IS_LITTLE_ENDIAN)
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
#else // unspecified endianness... use an endianness-agnostic version... less efficient
	((uint8_t *)&output)[0] = (input>>120) & 0xff;
	((uint8_t *)&output)[1] = (input>>112) & 0xff;
	((uint8_t *)&output)[2] = (input>>104) & 0xff;
	((uint8_t *)&output)[3] = (input>>96) & 0xff;
	((uint8_t *)&output)[4] = (input>>88) & 0xff;
	((uint8_t *)&output)[5] = (input>>80) & 0xff;
	((uint8_t *)&output)[6] = (input>>72) & 0xff;
	((uint8_t *)&output)[7] = (input>>64) & 0xff;
	((uint8_t *)&output)[8] = (input>>56) & 0xff;
	((uint8_t *)&output)[9] = (input>>48) & 0xff;
	((uint8_t *)&output)[10] = (input>>40) & 0xff;
	((uint8_t *)&output)[11] = (input>>32) & 0xff;
	((uint8_t *)&output)[12] = (input>>24) & 0xff;
	((uint8_t *)&output)[13] = (input>>16) & 0xff;
	((uint8_t *)&output)[14] = (input>>8) & 0xff;
	((uint8_t *)&output)[15] = input & 0xff;
#endif
	return output;
#endif
}

/**
 * \brief Convert a uint128_t from network order (big endian) to host order (with associated host-specific endianness)
 *
 * \param input The uint128_t to convert to host order
 *
 * \result The corresponding host order
 */
//uint128_t uint128_t_ntoh(const uint128_t input);

#endif	// __UINT128_H__
