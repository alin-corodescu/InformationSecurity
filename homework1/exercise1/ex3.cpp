//
// Created by alin on 12/10/17.
//

#include <cstdio>
#include "ex3.h"
#include <openssl/evp.h>
#include <openssl/aes.h>


void applyKey(void *in, void *out, unsigned char *key) {
    AES_KEY encrypt;
    AES_set_encrypt_key(key, 128, &encrypt);
    AES_encrypt((const unsigned char *) in, (unsigned char *) out, &encrypt);
}

void xorBlocks(char *b1, char *b2) {

    for (int i = 0; i <BLOCK_SIZE; i++) {
        b1[i] ^= b2[i];
    }

}
