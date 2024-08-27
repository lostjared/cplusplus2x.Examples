#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

long intToString(long value, char *buffer) {
    sprintf(buffer, "%ld", value); 
    return strlen(buffer);
}

char *str(long value) {
    char *buf = malloc(24);
    intToString(value, buf);
    return buf;
}

void print_number(unsigned long value) {
    printf("value is: %ld\n", value);
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
    printf("%ld\n", value);
}

long test1() { return 1; }

long scan_integer() {
    char buffer[25];
    long value = 0;
    scanf("%s", buffer);
    return atol(buffer);
}