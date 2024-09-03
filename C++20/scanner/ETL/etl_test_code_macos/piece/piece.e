#include<libetl/etl.e>
#include"sdl.e"

proc set_block_color(color) {
    if(color == 0) {
        sdl_setcolor(0,0,0,255);
        return 0;
    } 
    if(color == 1) {
        sdl_setcolor(255,0,0,255);
        return 0;
    }
    if(color == 2) {
        sdl_setcolor(0,255,0,255);
        return 0;
    }
    if(color == 3)  {
        sdl_setcolor(0,0,255,255);
        return 0;
    }
    if(color == 4) {
        sdl_setcolor(255,0,255,25);
        return 0;
    }
    return 0;
}

proc set_block_at(@block, index) {
    let bx = mematb(block, index+1);
    let by = mematb(block, index+2);
    let x = 32 * 4;
    let y = 16 * 4;
    set_block_color(mematb(block, index));
    sdl_fillrect(x*bx+20, y*by+30, 32 * 4, 16 * 4);
    return 0;
}

proc draw_grid(@grid, @block) {
    sdl_setcolor(0,0,0, 255);
    sdl_fillrect(0, 0, 1440, 1080); 
    let block_size_w = 32 * 4;
    let block_size_h = 16 * 4;
    let offset_left = 20;
    let offset_top = 30;
    
    for(let x = 0; x < 11; x = x + 1) {
        for(let y = 0; y < 16; y = y + 1) {
            let grid_x = (x*block_size_w)+offset_left;
            let grid_y = (y*block_size_h)+offset_top;
            let color = mematb(grid, x * 16 + y);
            set_block_color(color);
            sdl_fillrect(grid_x, grid_y, block_size_w, block_size_h);
        }
    }
    set_block_at(block,0);
    set_block_at(block,3);
    set_block_at(block,6);
    return 0;
}


proc @allocate_grid(w, h) {
    let grid = malloc (w * h);
    memclr(grid, w * h);
    return grid;    
}

proc @allocate_block() {
    let block = malloc (11);
    memclr(block, 10);
    return block;
}

proc setup_block(@block) {
    memstoreb(block,0, 1+(rand()%3)); // color
    memstoreb(block,1, 11/2 - 1 ); // x
    memstoreb(block,2, 0); // y
    memstoreb(block,3, 1+(rand()%3)); // color
    memstoreb(block,4, 11/2 - 1); // x
    memstoreb(block,5, 1); // y
    memstoreb(block,6, 1+(rand()%3)); // color
    memstoreb(block,7, 11/2 - 1); // x
    memstoreb(block,8, 2); // y
    return 0;
}

proc set_block_in_grid(@grid, @block) {
    let val = mematb(block, 0);
    let bx =  mematb(block, 1); 
    let by =  mematb(block, 2);
    memstoreb(grid, bx * 16 + by, val);
    val = mematb(block, 3);
    bx = mematb(block, 4);
    by = mematb(block, 5);
    memstoreb(grid, bx * 16 + by, val);
    val = mematb(block,6);
    bx = mematb(block, 7);
    by = mematb(block, 8);
    memstoreb(grid, bx * 16 + by, val);
    return 0;
}

proc move_block_down(@grid, @block) {
    if(mematb(block, 8) < 15) {
        memstoreb(block, 2, mematb(block, 2) + 1);
        memstoreb(block, 5, mematb(block, 5) + 1);
        memstoreb(block, 8, mematb(block, 8) + 1);
    } else {
        // store in grid
        // release new block
       set_block_in_grid(grid, block);
       setup_block(block); 
    }

    return 0;
}

proc init() {
    sdl_init();
    sdl_create("MasterPiece", 1440, 1080);
    srand(time(0));
    let grid = allocate_grid(11, 16);
    let block = allocate_block();
    setup_block(block);
    let prev_time = sdl_getticks();
    while (sdl_pump()) {
        sdl_clear(); // clear screen
        draw_grid(grid, block);
        sdl_flip(); // flip
        let current_time = sdl_getticks();
        if ((current_time - prev_time) >= 100) {
            prev_time = current_time; 
            if (sdl_keydown(79) && mematb(block, 1) < (1440 / (32 * 4) - 1))
                    memstoreb(block, 1, mematb(block, 1) + 1);
                    memstoreb(block, 4, mematb(block, 4) + 1);
                    memstoreb(block, 7, mematb(block, 7) + 1);
            } else {
                if (sdl_keydown(80) && mematb(block, 1) > 0) {
                    memstoreb(block, 1, mematb(block, 1) - 1);
                    memstoreb(block, 4, mematb(block, 4) - 1);
                    memstoreb(block, 7, mematb(block, 7) - 1);
                } else {
                    if(sdl_keydown(81)) {
                         move_block_down(grid, block);
                    }
                }
            }
        }
    }
    free(grid);
    free(block);
    sdl_release();
    sdl_quit();
    return 0;
}