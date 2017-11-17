/*
 * crypto.h
 *
 *  Created on: 2017年11月15日
 *      Author: ai-thinker
 */

#ifndef CRYPTO_H_
#define CRYPTO_H_
#include <stdint.h>
#define CONFIG_SSL_FULL_MODE
#ifndef PROGMEM
#define PROGMEM __attribute__((aligned(4))) __attribute__((section(".irom.text")))
#endif

#define SHA256_SIZE   32

typedef struct
{
    uint32_t total[2];
    uint32_t state[8];
    uint8_t buffer[64];
} SHA256_CTX;

void SHA256_Init(SHA256_CTX *c);
void SHA256_Update(SHA256_CTX *, const uint8_t *input, int len);
void SHA256_Final(uint8_t *digest, SHA256_CTX *);
void sha256(unsigned char out[32], const char *data);
void sha256_hex(char out[65], const char *data);
/**************************************************************************
 * HMAC declarations
 **************************************************************************/
void hmac_sha256(const uint8_t *msg, int length, const uint8_t *key,
        int key_len, uint8_t *digest);

#endif /* CRYPTO_H_ */
