/*
 * hmac_sha256.h
 *
 *  Created on: 2017年11月15日
 *      Author: ai-thinker
 */

#ifndef HMAC_H_
#define HMAC_H_
#include <stdio.h>
void hmac(unsigned char out[32], const unsigned char *key, size_t key_len, const unsigned char *data, size_t data_len);
void hmac_hex(char out[65], const unsigned char *key, size_t key_len, const unsigned char *data, size_t data_len);


#endif /* HMAC_H_ */
