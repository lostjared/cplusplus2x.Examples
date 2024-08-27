#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

unsigned int intToString(unsigned long value, char *buffer) {
    unsigned int i = 0;
    if (value == 0) {
        buffer[i++] = '0';
        buffer[i] = 0;
        return 1;
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

char *str(long value) {
    char *buf = malloc(24);
    intToString(value, buf);
    return buf;
}

void print_number(unsigned long value) {
    printf("value is: %d\n", value);
}
void print_ptr(void *value) {
    printf("ptr is: %p\n", value);
    unsigned long value1 = *(unsigned long *)value;
    printf("%d\n", value1);
}

void output(const char *src) {
    printf("%s\n", src);
}
void output_int(long value) {
    printf("%d\n", value);
}

long test1() { return 1; }

long scan_integer() {
    long value = 0;
    scanf("%d", &value);
    return value;
}