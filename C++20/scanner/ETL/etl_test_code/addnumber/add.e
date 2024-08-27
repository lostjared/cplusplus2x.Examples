proc add_numbers() {
    puts("Enter firsT number: ");
    let x = scan_integer();
    puts("Enter second number: ");
    let y = scan_integer();
    puts(str(x) + "+" + str(y) + "=" + str(x+y));
     return 0;
}

proc init() {
    add_numbers();
    return 0;
}


