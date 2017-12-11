//
// Created by alin on 12/10/17.
//

//
// Created by alin on 12/10/17.
//
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <malloc.h>
#include <cstring>
#include "ex3.h"


void decryptKey(char *k);


void receive_file(int fd, char *k, unsigned char *iv, char *mode);

void unpad_bytes(char *buffer);

int main(int argc, char* argv[]) {

    int a_b_fd, b_km_fd;
    // 16 bytes for the key
    char * k = (char *) malloc(16);

    mkfifo("tmp/a_b", 0666);
    mkfifo("tmp/b_km", 0666);

    a_b_fd = open("tmp/a_b", O_RDWR);
    b_km_fd = open("tmp/b_km", O_RDWR);

    // Write the first letter of the argument given
    char mode;
    read(a_b_fd, &mode, 1);
    printf("Read the mode %c\n",mode);

    // to the km as well
    write(b_km_fd, &mode, 1);
    printf("Sent the mode to KM: %c\n", mode);

    // wait for the key
    int read_bytes = 0;
    while(read_bytes < BLOCK_SIZE) {
        read_bytes += read(b_km_fd, k + read_bytes, (size_t) (BLOCK_SIZE - read_bytes));
    }
    printf("Read the key from KM\n");

    decryptKey(k);

    char signal = 0;
    write(a_b_fd, &signal, 1);

    receive_file(a_b_fd, k, iv, argv[1]);
    printf("\n\nReceived the file from A\n");
}

void receive_file(int fd, char *k, unsigned char *iv, char *mode) {

    bool finished = false;
    void* cipherTextBuffer = malloc(BLOCK_SIZE);
    void* previousBlock = NULL;
    char* plainTextBuffer = (char *) malloc(BLOCK_SIZE);
    // +1 for the \0
    char* printBuffer = (char *) malloc(BLOCK_SIZE + 1);
    printBuffer[BLOCK_SIZE] = 0;
    if (mode[0] == 'C') {
        previousBlock = malloc(BLOCK_SIZE);
        memcpy(previousBlock, iv, BLOCK_SIZE);
    }
    FILE* in = fdopen(fd, "rb");
    while (!feof(in)) {
        int bytes_read = 0;
        while (bytes_read < BLOCK_SIZE && !feof(in)) {
            bytes_read += fread(cipherTextBuffer + bytes_read, 1, BLOCK_SIZE - bytes_read, in);
        }

        // Decrypt the block
        applyKey(cipherTextBuffer, plainTextBuffer, (unsigned char *) k);
        printf("Applied the key!");
        // if operating in cbc mode also apply the xor
        if (previousBlock != NULL) {
            xorBlocks(plainTextBuffer, (char *) previousBlock);
            printf("xor'ed blocks");
            mempcpy(previousBlock, cipherTextBuffer, BLOCK_SIZE);
        }

        // We have reached the end
        if (bytes_read < BLOCK_SIZE) {
            finished = true;

            // This is unnecessary, it's just for show here, in case we will deal with files other than text sometime
            unpad_bytes(plainTextBuffer);
        }

        mempcpy(printBuffer, plainTextBuffer, BLOCK_SIZE);
        printf("%s", printBuffer);

        if (finished) {
            free(printBuffer);
            free(plainTextBuffer);
            if (previousBlock != NULL)
                free(previousBlock);
            fclose(in);
            break;
        }
    }
}

void unpad_bytes(char *buffer) {
    int end = BLOCK_SIZE - 1;
    int padded_bytes = (int) buffer[end];
    printf("The block is padded with %d bytes\n", padded_bytes + 1);
    int i = 0;
    while(padded_bytes) {
        buffer[end - i] = 0;
        i++;
        padded_bytes--;
    }
}

void decryptKey(char *k) {
    applyKey(k,k,key3);
}
