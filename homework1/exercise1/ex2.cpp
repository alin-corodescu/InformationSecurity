//
// Created by alin on 12/10/17.
//
#include <iostream>
#include <cstring>
#include "openssl/evp.h"

#define BUFFER_SIZE 1024

const EVP_MD *md5 = EVP_md5();
const EVP_MD *sha = EVP_sha256();
EVP_MD_CTX *md5_context = EVP_MD_CTX_create();;
EVP_MD_CTX *sha_context = EVP_MD_CTX_create();

int file = 1;
void produce_digests(const char*);
int main(int argc, char *argv[]) {

    for (int i = 1; i<= 2; i++) {
        EVP_DigestInit_ex(md5_context, md5, NULL);
        EVP_DigestInit_ex(sha_context, sha, NULL);

        produce_digests(argv[i]);
    }

    int identical_bytes = 0;

    FILE* md5_1 = fopen("h1_md5", "rb");
    FILE* md5_2 = fopen("h2_md5", "rb");

    char* buffer1 = (char *) malloc(32);
    char* buffer2 = (char *) malloc(32);

    int bytes_read = (int) fread(buffer1, 1, 32, md5_1);
    fread(buffer2, 1, 32, md5_2);
    for (int i = 0; i < bytes_read; i++) {
        if (!memcmp(buffer1 + i, buffer2 + i, 1)) {
            identical_bytes++;
        }
    }
    printf("Pentru md5, cele doua hash-uri au %d bytes identici\n", identical_bytes);


    identical_bytes = 0;

    FILE* sha_1 = fopen("h1_sha256", "rb");
    FILE* sha_2 = fopen("h2_sha256", "rb");

    bytes_read = (int) fread(buffer1, 1, 32, sha_1);
    fread(buffer2, 1, 32, sha_2);
    for (int i = 0; i < bytes_read; i++) {
        if (!memcmp(buffer1 + i, buffer2 + i, 1)) {
            identical_bytes++;
        }
    }
    printf("Pentru sha256, cele doua hash-uri au %d bytes identici\n", identical_bytes);

    free(buffer1);
    free(buffer2);

    fclose(md5_1);
    fclose(md5_2);
    fclose(sha_1);
    fclose(sha_2);
}

void produce_digests(const char* filename) {
    FILE* f1 = fopen(filename, "r");

    char* f1_buffer = (char *) malloc((size_t) BUFFER_SIZE);
    while(!feof(f1)) {
        int bytes_read = (int) fread(f1_buffer, 1, BUFFER_SIZE, f1);
        EVP_DigestUpdate(md5_context, f1_buffer, bytes_read);
        EVP_DigestUpdate(sha_context, f1_buffer, bytes_read);
    }

    fclose(f1);

    unsigned char* md5_digest = (unsigned char *) malloc(32);
    unsigned int md5_digest_len;
    EVP_DigestFinal(md5_context, md5_digest, &md5_digest_len);

    unsigned char* sha_digest = (unsigned char *) malloc(32);
    unsigned int sha_digest_len;
    EVP_DigestFinal(sha_context, sha_digest, &sha_digest_len);

    FILE* md5_out;
    FILE* sha_out;
    if (file == 1) {
        md5_out = fopen("h1_md5","w");
        sha_out = fopen("h1_sha256","w");
    }
    else
    {
        md5_out = fopen("h2_md5","w");
        sha_out = fopen("h2_sha256","w");
    }

    fwrite(md5_digest, 1, md5_digest_len, md5_out);
    fwrite(sha_digest, 1, sha_digest_len, sha_out);

    fclose(md5_out);
    fclose(sha_out);


    free(f1_buffer);
    free(sha_digest);
    free(md5_digest);

    file++;

}
