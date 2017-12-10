//
// Created by alin on 12/10/17.
//

#ifndef EXERCISE1_EX3_H
#define EXERCISE1_EX3_H
const int BLOCK_SIZE = 16;
// Initialization vector to be used
unsigned char *iv = (unsigned char *)"\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f";

/* median word key*/
unsigned char *key3 = (unsigned char *)"\x6D\x65\x64\x69\x61\x6E\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20";

void applyKey(void *in, void *out, unsigned char *key);
#endif //EXERCISE1_EX3_H
