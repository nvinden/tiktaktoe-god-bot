#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ttt.h"

int main(int argc, char * argv[]){

    init_boards();
    init_board(START_BOARD);
    join_graph(START_BOARD);
    compute_score();

    for(int i = 1; i < argc; i++){
        //printf("%d", i);
        int hash;
        hash = atoi(argv[i]);
        print_node(htable[hash]);
    }

    return 0;
}