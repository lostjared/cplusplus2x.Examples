#include<libetl/etl.e>
#include"sdl.e"

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
                if(color == 3)  {
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
        sdl_clear(); // clear screen
        draw_grid(grid, mode);
        sdl_flip(); // flip
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