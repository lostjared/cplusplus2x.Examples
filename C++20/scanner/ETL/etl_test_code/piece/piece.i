# 1 "piece.e"
# 1 "<built-in>" 1
# 1 "<built-in>" 3
# 370 "<built-in>" 3
# 1 "<command line>" 1
# 1 "<built-in>" 2
# 1 "piece.e" 2
# 1 "/usr/local/include/libetl/etl.e" 1


define scan_integer();
define len($value);
define $str(value);
define exit(code);
define puts($output);
define srand(seed);
define rand();
define time(value);
define strlen($value);
define strcmp($value1, $value2);
define at($value, index);
define @malloc(bytes);
define @calloc(bytes, size);
define free(@memory);
define mematl(@memory, index);
define mematb(@memory, index);
define memclr(@memory, size);
define memstorel(@memory, index, value);
define memstoreb(@memory, index, value);
# 2 "piece.e" 2
# 1 "./sdl.e" 1



define sdl_init();
define sdl_quit();
define sdl_pump();
define sdl_release();
define sdl_create($name, width, height);
define sdl_setcolor(r, g, b, a);
define sdl_fillrect(x, y, w, h);
define sdl_flip();
define sdl_clear();
# 3 "piece.e" 2


proc draw_grid() {

    sdl_setcolor(0,0,0, 255);
    sdl_fillrect(0, 0, 1440, 1080);


    let grid_w = 8;
    let grid_h = 17;
    let block_size_w = 32 * 4;
    let block_size_h = 16 * 4;

    for(let x = 0; x < grid_w; x = x + 1) {
        for(let y = 0; y < grid_h; y = y + 1) {
            let grid_x = x*block_size_w;
            let grid_y = y*block_size_h;
            sdl_setcolor(rand()%255, rand()%255, rand()%255, 255);
            sdl_fillrect(grid_x, grid_y, block_size_w, block_size_h);
        }
    }


    return 0;
}


proc init() {
    sdl_init();
    sdl_create("MasterPiece", 1440, 1080);
    while (sdl_pump()) {
        sdl_clear();
        draw_grid();
        sdl_flip();
    }
    sdl_release();
    sdl_quit();
    return 0;
}
