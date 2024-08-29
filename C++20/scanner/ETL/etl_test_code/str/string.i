# 0 "string.e"
# 0 "<built-in>"
# 0 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 0 "<command-line>" 2
# 1 "string.e"


proc init() {
    let x = 200;
    let y = 300;
    puts(str(x) + "+" + str(y) + "=" + str(x+y));
    puts(str(x * x));
    return 0;
}
