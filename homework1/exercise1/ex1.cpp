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

EVP_CIPHER* cipher;

void handleErrors(void)
{
    ERR_print_errors_fp(stderr);
    abort();
}

int encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *key,
            unsigned char *iv, unsigned char *ciphertext)
{
    EVP_CIPHER_CTX *ctx;

    int len;

    int ciphertext_len;

    /* Create and initialise the context */
    if(!(ctx = EVP_CIPHER_CTX_new())) handleErrors();

    /* Initialise the encryption operation. IMPORTANT - ensure you use a key
     * and IV size appropriate for your cipher
     * In this example we are using 256 bit AES (i.e. a 256 bit key). The
     * IV size for *most* modes is the same as the block size. For AES this
     * is 128 bits */
    if(1 != EVP_EncryptInit_ex(ctx, cipher, NULL, key, iv))
        handleErrors();

    /* Provide the message to be encrypted, and obtain the encrypted output.
     * EVP_EncryptUpdate can be called multiple times if necessary
     */
    if(1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len))
        handleErrors();
    ciphertext_len = len;

    /* Finalise the encryption. Further ciphertext bytes may be written at
     * this stage.
     */
    if(1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len)) handleErrors();
    ciphertext_len += len;

    /* Clean up */
    EVP_CIPHER_CTX_free(ctx);

    return ciphertext_len;
}

int decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key,
            unsigned char *iv, unsigned char *plaintext)
{
    EVP_CIPHER_CTX *ctx;

    int len;

    int plaintext_len;

    /* Create and initialise the context */
    if(!(ctx = EVP_CIPHER_CTX_new())) handleErrors();

    /* Initialise the decryption operation. IMPORTANT - ensure you use a key
     * and IV size appropriate for your cipher
     * In this example we are using 256 bit AES (i.e. a 256 bit key). The
     * IV size for *most* modes is the same as the block size. For AES this
     * is 128 bits */
    if(1 != EVP_DecryptInit_ex(ctx, cipher, NULL, key, iv))
        handleErrors();

    /* Provide the message to be decrypted, and obtain the plaintext output.
     * EVP_DecryptUpdate can be called multiple times if necessary
     */
    if(1 != EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len))
        return -1;
    plaintext_len = len;

    /* Finalise the decryption. Further plaintext bytes may be written at
     * this stage.
     */
    if(1 != EVP_DecryptFinal_ex(ctx, plaintext + len, &len))
        return -1;
    plaintext_len += len;

    /* Clean up */
    EVP_CIPHER_CTX_free(ctx);

    return plaintext_len;
}

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
    /* Set up the key and iv. Do I need to say to not hard code these in a
 * real application? :-)
 */

    FILE* pt = fopen(pt_path, "rb");
    fseek(pt, 0, SEEK_END);
    long file_size = ftell(pt);
    fseek(pt, 0, SEEK_SET);

    unsigned char* plaintext = (unsigned char*) malloc(file_size + 1);
    fread(plaintext, 1, file_size, pt);
    fclose(pt);
    // Mark the end of the text
    plaintext[file_size] = '\0';



    FILE* ct = fopen(ct_path, "rb");
    fseek(ct, 0, SEEK_END);
    long crypt_file_size = ftell(ct);
    fseek(ct, 0, SEEK_SET);

    unsigned char* ciphertext = (unsigned char*) malloc(file_size);
    fread(ciphertext, 1, crypt_file_size, ct);
    fclose(ct);


    int output_size = (int) ((file_size / BLOCK_SIZE + 1) * BLOCK_SIZE);

    unsigned char *decryptedtext;
    decryptedtext = (unsigned char*) malloc((size_t) output_size);


    /* A 256 bit key */
    unsigned char *key = (unsigned char *)"0123456789012345";

/* A 128 bit IV */
    unsigned char *iv = (unsigned char *)"\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f";

    char word[100];
    FILE* dict = fopen("word_dict.txt", "r");
    int tries = 0;
    while (true) {
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

        int decryptedtext_len, ciphertext_len = crypt_file_size;

        /* Decrypt the ciphertext */
        decryptedtext_len = decrypt(ciphertext, ciphertext_len, key, iv,
                                    decryptedtext);

        if (decryptedtext_len == -1)
            continue;

        if (!memcmp(decryptedtext, plaintext, file_size)) {
            word[word_len] = 0;
            printf("The word is %s\n",word);

            break;
        }

    }
    printf("It took the program %d tries\n", tries);
    return 0;
}
