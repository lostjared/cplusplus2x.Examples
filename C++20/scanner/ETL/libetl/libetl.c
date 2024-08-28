#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <stdbool.h>
#include <string.h>

size_t intToString(long value, char *buffer, size_t buffer_size) {
    char temp[21]; 
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

    size_t length = i;
    for (int j = 0; j < i; j++) {
        buffer[j] = temp[i - j - 1];
    }
    buffer[length] = '\0';
    return length;
}


char *str(long value) {
    size_t buffer_size = 21;
    char *buf = (char *)malloc(buffer_size+1);
    if (buf == NULL) {
        fprintf(stderr, "Error: Memory allocation failed in str\n");
        exit(EXIT_FAILURE);
    }
    intToString(value, buf, buffer_size);
    return buf;
}

void print_number(unsigned long value) {
    printf("value is: %lu\n", value);
}

void print_ptr(void *value) {
    printf("ptr is: %p\n", value);
    unsigned long value1 = *(unsigned long *)value;
    printf("%lu\n", value1);
}

void output(const char *src) {
    printf("%s\n", src);
}

void output_int(long value) {
    printf("%ld\n", value);
}

long test1() {
    return 1;
}

long scan_integer() {
    long value;
    if (scanf("%ld", &value) != 1) {
        fprintf(stderr, "Error: Failed to read integer in scan_integer\n");
        exit(EXIT_FAILURE);
    }
    return value;
}