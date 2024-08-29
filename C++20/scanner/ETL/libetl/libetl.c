#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>

size_t intToString(long value, char *buffer, size_t buffer_size) {
    char temp[22]; 
    memset(temp, 0, 22);
    int i = 0;
    bool isNegative = false;

    if (value < 0) {
        isNegative = true;
        value = -value;
    }

    do {
        temp[i++] = (value % 10) + '0';
        value /= 10;
    } while (value > 0);

    if (isNegative) {
        temp[i++] = '-';
    }

    if (i >= buffer_size) {
        return 0;
    }
    memset(buffer, 0, buffer_size);
    size_t length = i;
    for (int j = 0; j < i; j++) {
        buffer[j] = temp[i - j - 1];
    }
    buffer[length] = '\0';
    return length;
}


void *str(long value) {
    size_t buffer_size = 21;
    char *buf = (char *)malloc(buffer_size+1);
    if (buf == NULL) {
        fprintf(stderr, "Error: Memory allocation failed in str\n");
        exit(EXIT_FAILURE);
    }
    intToString(value, buf, buffer_size);
    return (void*)buf;
}

long scan_integer() {
    long value;
    if (scanf("%ld", &value) != 1) {
        fprintf(stderr, "Error: Failed to read integer in scan_integer\n");
        exit(EXIT_FAILURE);
    }
    return value;
}