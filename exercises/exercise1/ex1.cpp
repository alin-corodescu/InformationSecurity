//
// Created by alin on 12/10/17.
//
#include <iostream>
#include <cstring>
#include "openssl/evp.h"
#include <openssl/err.h>
#include <fcntl.h>
#include "openssl/aes.h"
#include "unistd.h"


#define BLOCK_SIZE 128

#define BUFFER_SIZE 1024

EVP_CIPHER* cipher;
EVP_CIPHER_CTX *ctx;


int main(int argc, char *argv[]) {
    char *pt_path, *ct_path;
    if (argc != 4) {
        printf("Wrong arguments");
        exit(1);
    }
    else {
        pt_path = argv[1];
        ct_path = argv[2];
        if (!strcmp(argv[3], "CBC"))
            cipher = (EVP_CIPHER *) EVP_aes_128_cbc();
        else
            cipher = (EVP_CIPHER *) EVP_aes_128_ecb();

    }

    ctx = EVP_CIPHER_CTX_new();


    // Open the plaintext file
    FILE* pt = fopen(pt_path, "rb");

    // Allocate a buffer to read parts of the plain text
    unsigned char* plaintext = (unsigned char*) malloc(2* BUFFER_SIZE);



    // Do the same for the ciphertext
    FILE* ct = fopen(ct_path, "rb");
    unsigned char* ciphertext = (unsigned char*) malloc(BUFFER_SIZE);

    // Allocate a buffer for the decrypted text
    unsigned char *decryptedtext;
    decryptedtext = static_cast<unsigned char *> (malloc(2 * BUFFER_SIZE));


    unsigned char *key = (unsigned char *)"0123456789012345";

    unsigned char *iv = (unsigned char *)"\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f";

    char word[100];
    FILE* dict = fopen("word_dict.txt", "r");
    int tries = 0;
    bool finished = false;
    while (!finished) {
        tries++;
        // read the next word in the dictionary
        fscanf(dict, "%s", word);
        int word_len = strlen(word);
        if (word_len >= 16)
            continue;

        for (int i = word_len; i < 16; i++)
            word[i] = ' ';

        // the implementation will take into account only the first 16 bytes anyway
        key = (unsigned char*) word;

        // Here begins the decryption
        // Reset the files
        fseek(ct, 0, SEEK_SET);
        fseek(pt, 0, SEEK_SET);
        clearerr(ct);
        clearerr(pt);
        // Reinitialize the decryption context
        EVP_DecryptInit_ex(ctx, cipher, NULL, key, iv);
        while (!feof(ct)) {
            int dt_len;
            int ct_len = fread(ciphertext, 1, BUFFER_SIZE, ct);
            // Decrypt the next part
            int status = EVP_DecryptUpdate(ctx, decryptedtext, &dt_len, ciphertext, ct_len);
            if (status != 1)
                break;
            if (ct_len < BUFFER_SIZE) {
                int final;
                status = EVP_DecryptFinal_ex(ctx, decryptedtext + dt_len, &final);
                if (status != 1)
                    break;
                dt_len += final;
            }
            // Read the next block of plain text
            int pt_len = fread(plaintext, 1, dt_len, pt);
            if (pt_len != dt_len) {
                // The codes don't match
                break;
            }

            if (!memcmp(decryptedtext, plaintext, dt_len)) {
                word[word_len] = 0;
                printf("The word is %s\n",word);
                finished = true;
                break;
            }
        }
    }
    printf("It took the program %d tries\n", tries);

    printf("The key is: ");
    for (int i = 0; i < 16; i++) {
        if (word[i] == 0)
            word[i] = ' ';
        printf("%X",word[i]);
    }
    return 0;
}
