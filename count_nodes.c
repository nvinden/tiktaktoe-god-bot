#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ttt.h"

int main(int argc, char * argv[]){

    init_boards();
    init_board(START_BOARD);
    join_graph(START_BOARD);


    int count = 0;
    for(int i = 0; i < HSIZE; i++){
        if(htable[i].init == 1) count++;
    }

    printf("Number of playable boards: %d\n", count);

    return 0;
}