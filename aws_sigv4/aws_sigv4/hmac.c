/*
 * hmac_sha256.c
 *
 *  Created on: 2017年11月15日
 *      Author: ai-thinker
 */
#include "hmac.h"

#include "crypto.h"
/**
 * Perform HMAC-SHA256
 * NOTE: does not handle keys larger than the block size.
 */
void hmac_sha256(const uint8_t *msg, int length, const uint8_t *key,
        int key_len, uint8_t *digest)
{
    hmac_sha256_v(&msg, &length, 1, key, key_len, digest);
}

void hmac_sha256_v(const uint8_t **msg, int *length, int count, const uint8_t *key,
        int key_len, uint8_t *digest)
{
    SHA256_CTX context;
    uint8_t k_ipad[64];
    uint8_t k_opad[64];
    int i;

    memset(k_ipad, 0, sizeof k_ipad);
    memset(k_opad, 0, sizeof k_opad);
    memcpy(k_ipad, key, key_len);
    memcpy(k_opad, key, key_len);

    for (i = 0; i < 64; i++)
    {
        k_ipad[i] ^= 0x36;
        k_opad[i] ^= 0x5c;
    }

    SHA256_Init(&context);
    SHA256_Update(&context, k_ipad, 64);
    for (i = 0; i < count; ++i)
    {
        SHA256_Update(&context, msg[i], length[i]);
    }
    SHA256_Final(digest, &context);
    SHA256_Init(&context);
    SHA256_Update(&context, k_opad, 64);
    SHA256_Update(&context, digest, SHA256_SIZE);
    SHA256_Final(digest, &context);
}

void hmac(unsigned char out[32], const unsigned char *key, size_t key_len, const unsigned char *data, size_t data_len)
{
	hmac_sha256(data,data_len,key,key_len,out);
}

void hmac_hex(char out[65], const unsigned char *key, size_t key_len, const unsigned char *data, size_t data_len)
{
    unsigned char hash[SHA256_SIZE];
    int i;

    hmac(hash, key, key_len, data, data_len);

    for (i = 0; i < SHA256_SIZE; i++)
        sprintf(out + (i * 2), "%02x", hash[i]);

    out[64] = '\0';
}
