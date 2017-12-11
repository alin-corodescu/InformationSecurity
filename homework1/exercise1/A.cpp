//
// Created by alin on 12/10/17.
//
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <malloc.h>
#include <cstring>
#include "ex3.h"


#define IN "test.txt"

void send_file(int fd, char *k, unsigned char *iv, char *mode);

void decryptKey(char *k);

void padBuffer(char *buffer, int bytes);

int main(int argc, char* argv[]) {

    int a_b_fd, a_km_fd;
    // 16 bytes for the key
    char * k = (char *) malloc(16);

    mkfifo("tmp/a_b", 0666);
    mkfifo("tmp/a_km", 0666);

    a_b_fd = open("tmp/a_b", O_RDWR);
    a_km_fd = open("tmp/a_km", O_RDWR);

    // Write the first letter of the argument given
    while(!write(a_b_fd, argv[1], 1));
    printf("Sent the mode to B: %c\n", argv[1][0]);
    // to the km as well
    while(!write(a_km_fd, argv[1], 1));
    printf("Sent the mode to KM: %c\n", argv[1][0]);

    // wait for the key
    int read_bytes = 0;
    while(read_bytes < BLOCK_SIZE) {
        read_bytes += read(a_km_fd, k + read_bytes, (size_t) (BLOCK_SIZE - read_bytes));
    }

    printf("Read the key from KM\n");

    // wait for B to confirm that he is ready
    int response_code;
    while(!read(a_b_fd, &response_code, 1));
    printf("Got the response from B\n");

    decryptKey(k);
    send_file(a_b_fd, k, iv, argv[1]);
}

void decryptKey(char *k) {
    applyKey(k,k,key3);
}

void send_file(int fd, char *k, unsigned char *iv, char *mode) {
    bool finished = false;
    void* buffer = malloc(BLOCK_SIZE);
    FILE* in = fopen(IN, "rb");

    void* cipherTextBuffer = malloc(BLOCK_SIZE);
    void* previousBlock = NULL;
    if (mode[0] == 'C') {
        previousBlock = malloc(BLOCK_SIZE);
        memcpy(previousBlock, iv, BLOCK_SIZE);
    }
    while (!feof(in))
    {
        int bytes_read = 0;
        while (bytes_read < BLOCK_SIZE && !feof(in)) {
            bytes_read += (int) fread(buffer+bytes_read, (size_t) (BLOCK_SIZE - bytes_read), 1, in);
        }

        if (bytes_read < BLOCK_SIZE)
        {
            // we have reached the end of file
            padBuffer((char *) buffer, bytes_read);
            finished = true;
        }

        if (previousBlock != NULL) {

            // XOR the buffer with the previous block
            xorBlocks((char *) buffer, (char *) previousBlock);

        }
        applyKey(buffer, cipherTextBuffer, (unsigned char *) k);

        if (previousBlock != NULL)
            memcpy(previousBlock, cipherTextBuffer, BLOCK_SIZE);

        int written_bytes = 0;
        while(written_bytes < BLOCK_SIZE) {
            written_bytes += write(fd, cipherTextBuffer + written_bytes, (size_t) (BLOCK_SIZE - written_bytes));
        }

        if (finished) {
            fclose(in);
            close(fd);
            break;
        }

    }
}

void padBuffer(char *buffer, int bytes) {
    int toPad = BLOCK_SIZE - bytes;
    for (int i = bytes; i < BLOCK_SIZE -1; i++) {
        buffer[i] = 0;
    }
    buffer[BLOCK_SIZE - 1] = (char) toPad;
}
