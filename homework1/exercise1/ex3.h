//
// Created by alin on 12/10/17.
//
#pragma once
#ifndef EXERCISE1_EX3_H
#define EXERCISE1_EX3_H

#include <openssl/aes.h>

const int BLOCK_SIZE = 16;
// Initialization vector to be used
unsigned char *iv = (unsigned char *)"\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f";

/* median word key*/
unsigned char *key3 = (unsigned char *)"\x6D\x65\x64\x69\x61\x6E\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20";

void applyKey(void *in, void *out, unsigned char *key);

void xorBlocks(char* b1, char* b2);

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
#endif //EXERCISE1_EX3_H
