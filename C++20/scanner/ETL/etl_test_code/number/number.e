
proc print_numbers() {
    let x = 25;
    let y = 100;
    let z = 50;
    let value = x*y/z+(2+4);
    printf("The Value is: %d\n", value);
    return 0;
}


proc init() {
    print_numbers();
    return 0;
}