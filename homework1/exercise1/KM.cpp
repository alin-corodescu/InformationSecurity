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
#include "ex3.h"


#define IN "test.txt"

const char* k1 = "\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f";
const char* k2 = "\x0f\x0f\x0f\x0f\x0f\x0f\x0f\x0f\x0f\x0f\x0f\x0f\x0f\x0f\x0f";
int main(int argc, char* argv[]) {

    int b_km_fd, a_km_fd;
    // 16 bytes for the key
    char * k = (char *) malloc(16);


    b_km_fd = open("tmp/b_km", O_RDWR);
    a_km_fd = open("tmp/a_km", O_RDWR);

    char mode;
    read(a_km_fd, &mode, 1);
    printf("Read the mode from A: %c\n", mode);
    if (mode == 'E')
        applyKey((void *) k1, k, key3);
    else
        applyKey((void *) k2, k, key3);

    int bytes_written = 0;
    while (bytes_written < BLOCK_SIZE) {
        bytes_written += write(a_km_fd, k, BLOCK_SIZE);
    }
    printf("Sent the key to A\n");


    read(b_km_fd, &mode, 1);
    printf("Read the mode from B: %c\n", mode);

    bytes_written = 0;
    while (bytes_written < BLOCK_SIZE) {
        bytes_written += write(b_km_fd, k, BLOCK_SIZE);
    }
    printf("Sent the key to B\n");
}

