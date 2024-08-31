# 0 "length.e"
# 0 "<built-in>"
# 0 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 0 "<command-line>" 2
# 1 "length.e"

proc testLen($what) {
    let n_s = "hey: " + what;
    puts(n_s);
    return len(n_s);
}

proc init() {
    puts("Len value:" + str(testLen("value is here")));
    return 0;
}
