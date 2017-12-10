//
// Created by alin on 12/10/17.
//

#include <cstdio>
#include "ex3.h"
#include <openssl/evp.h>
#include <openssl/aes.h>


int applyKey(void *buffer, void *plainText, unsigned char *key, int plain_len) {
    int len;
    int fin;
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    EVP_EncryptInit(ctx, EVP_aes_128_ecb(), NULL, key);
    EVP_EncryptUpdate(ctx, (unsigned char *) buffer, &len, (const unsigned char *) plainText, plain_len);
    EVP_EncryptFinal_ex(ctx, (unsigned char *) (buffer + len), &fin);

    return len + fin;
}
