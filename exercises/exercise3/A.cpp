//
// Created by alin on 12/10/17.
//
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <malloc.h>
#include <cstring>
#include "ex3.h"


#define IN "text.txt"

void send_file(int fd, char *k, unsigned char *iv, char *mode);

void decryptKey(char *k);

void padBuffer(char *buffer, int bytes);

int main(int argc, char* argv[]) {

    int a_b_fd, a_km_fd, b_a_fd, km_a_fd;
    // 16 bytes for the key
    char * k = (char *) malloc(16);

    mkfifo("tmp/a_b", 0777);
    mkfifo("tmp/b_a", 0777);
    mkfifo("tmp/km_a", 0777);
    mkfifo("tmp/a_km", 0777);
    mkfifo("tmp/km_b", 0777);
    mkfifo("tmp/b_km", 0777);

    a_b_fd = open("tmp/a_b", O_WRONLY);
    a_km_fd = open("tmp/a_km", O_WRONLY);
    b_a_fd = open("tmp/b_a", O_RDONLY);
    km_a_fd = open("tmp/km_a", O_RDONLY);


    int result;
    // Write the first letter of the argument given
    while(!(result = write(a_b_fd, argv[1], 1)));
    printf("%d Sent the mode to B: %c\n", result, argv[1][0]);
    // to the km as well
    while(!(result = write(a_km_fd, argv[1], 1)));
    printf("%d Sent the mode to KM: %c\n", result, argv[1][0]);

    // wait for the key
    int read_bytes = 0;
    while(read_bytes < BLOCK_SIZE) {
        read_bytes += read(km_a_fd, k + read_bytes, (size_t) (BLOCK_SIZE - read_bytes));
    }

    printf("Read the key from KM\n");

    decryptKey(k);
    printKey(k);

    // wait for B to confirm that he is ready
    int response_code;
    while(!read(b_a_fd, &response_code, 1));
    printf("Got the response from B\n");



    send_file(a_b_fd, k, iv, argv[1]);
    printf("Sent the file to B\n");
}

void decryptKey(char *k) {
    decrypt(k, k, key3);
}

void send_file(int fd, char *k, unsigned char *iv, char *mode) {
    bool finished = false;
    char* buffer = (char *) malloc(BLOCK_SIZE);
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
        bytes_read += (int) fread(buffer + bytes_read, 1, (size_t) (BLOCK_SIZE - bytes_read), in);
        printf("Read a block of %d bytes\n", bytes_read);
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
        encrypt(buffer, cipherTextBuffer, (unsigned char *) k);

        if (previousBlock != NULL)
            memcpy(previousBlock, cipherTextBuffer, BLOCK_SIZE);

        int written_bytes = 0;
        while(written_bytes < BLOCK_SIZE) {
            written_bytes += write(fd, cipherTextBuffer + written_bytes, (size_t) (BLOCK_SIZE - written_bytes));
        }
        printf("Written %d bytes to B\n", written_bytes);
        if (finished) {
            free(buffer);
            free(cipherTextBuffer);
            if (previousBlock != NULL)
                free(previousBlock);
            fclose(in);
            close(fd);
            break;
        }
    }
}

void padBuffer(char *buffer, int bytes) {
    int toPad = BLOCK_SIZE - bytes - 1;
    for (int i = bytes; i < BLOCK_SIZE -1; i++) {
        buffer[i] = 0;
    }
    buffer[BLOCK_SIZE - 1] = (char) toPad;
    printf("Padded with %d bytes\n", toPad + 1);
}
