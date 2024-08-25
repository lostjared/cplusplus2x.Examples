#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

unsigned int intToString(unsigned long value, char *buffer) {
    unsigned int i = 0;
    if (value == 0) {
        buffer[i++] = '0';
    } else {
        while (value > 0) {
            int remainder = value % 10;
            buffer[i++] = '0' + remainder;
            value /= 10;
        }
    }
    buffer[i] = '\0';
    int len = strlen(buffer);
    for (int j = 0; j < len / 2; ++j) {
        char temp = buffer[j];
        buffer[j] = buffer[len - j - 1];
        buffer[len - j - 1] = temp;
    }   
    return i;
}

void print_number(unsigned long value) {
    printf("value is: %d\n", value);
}