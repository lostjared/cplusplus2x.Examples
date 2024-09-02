#include<libetl/etl.e>
#include"sdl.e"


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
        sdl_clear(); // clear screen
        draw_grid();
        sdl_flip(); // flip
    }
    sdl_release();
    sdl_quit();
    return 0;
}