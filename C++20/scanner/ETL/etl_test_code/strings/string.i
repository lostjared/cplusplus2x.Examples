# 0 "string.e"
# 0 "<built-in>"
# 0 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 0 "<command-line>" 2
# 1 "string.e"
proc $format($hello, x, y) {
    return hello + ": " + str(x) + "+" + str(y) + "=" + str(x+y);
}

proc init() {
    let x = 25;
    let y = 25;
    puts(format("Hey two numers: ", x, y));
    let s = "Jared Says: ";
    puts(format(s, 10, 10));
    return 0;
}
