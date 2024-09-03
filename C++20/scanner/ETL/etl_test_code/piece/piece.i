# 0 "piece.e"
# 0 "<built-in>"
# 0 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 0 "<command-line>" 2
# 1 "piece.e"
# 1 "/usr/local/include/libetl/etl.e" 1 3



# 3 "/usr/local/include/libetl/etl.e" 3
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
# 1 "sdl.e" 1




# 4 "sdl.e"
define sdl_init();
define sdl_quit();
define sdl_pump();
define sdl_release();
define sdl_create($name, width, height);
define sdl_setcolor(r, g, b, a);
define sdl_fillrect(x, y, w, h);
define sdl_flip();
define sdl_clear();
define sdl_keydown(key);
# 3 "piece.e" 2

proc draw_grid(@grid, mode) {
    sdl_setcolor(0,0,0, 255);
    sdl_fillrect(0, 0, 1440, 1080);
    let block_size_w = 32 * 4;
    let block_size_h = 16 * 4;
    let offset_left = 20;
    let offset_top = 30;
    let grid_w = 1440 / block_size_w;
    let grid_h = 1080 / block_size_h;
    for(let x = 0; x < grid_w; x = x + 1) {
        for(let y = 0; y < grid_h; y = y + 1) {
            let grid_x = (x*block_size_w)+offset_left;
            let grid_y = (y*block_size_h)+offset_top;
            let color = mematb(grid, x * grid_w + y);
            if(mode == 1) { 
                if(color == 0) {
                    sdl_setcolor(0,0,0,255);
                }
                if(color == 1) {
                    sdl_setcolor(255,0,0,255);
                }
                if(color == 2) {
                    sdl_setcolor(0,255,0,255);
                }
                if(color == 3) {
                    sdl_setcolor(0,0,255,255);
                }
                if(color == 4) {
                    sdl_setcolor(255,0,255,25);
                }
            } else {
                sdl_setcolor(rand()%255,rand()%255,rand()%255,rand()%255);
            }
            sdl_fillrect(grid_x, grid_y, block_size_w, block_size_h);
        }
    }
    return 0;
}

proc @allocate_grid(w, h) {
    let grid = malloc (w * h);
    memclr(grid, w * h);
    for(let x = 0; x < w; x = x + 1) {
        for(let y = 0; y < h; y = y + 1) {
            memstoreb(grid, x * w + y, 1+(rand()%4));
        }
    }
    return grid;
}

proc init() {
    sdl_init();
    sdl_create("MasterPiece", 1440, 1080);
    let grid = allocate_grid(1440 / (32 * 4), 1080 / (16 * 4));
    let mode = 1;
    while (sdl_pump()) {
        sdl_clear();
        draw_grid(grid, mode);
        sdl_flip();
        let s = "test";
        if(sdl_keydown(40)) {
            mode = 1;
        } else {
            if(sdl_keydown(44)) {
                mode = 0;
            }
        }
    }
    free(grid);
    sdl_release();
    sdl_quit();
    return 0;
}
