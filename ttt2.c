#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include "ttt.h"

void init_boards(){
    for(int i = 0; i < HSIZE; i++){
        htable[i].init = 0;
    }
}

int depth(Board board){
    int out = 0;
    for(int i = 0; i < 9; i++){
        char curr = board[pos2idx[i]];
        if(curr == 'X' || curr == 'O') out++;
    }
    return out;
}

char winner(Board board){
    if(depth(board) >= 9) return '-';
    for(int i = 0; i < 8; i++){
        int * winning_idx = WINS[i];
        if(board[pos2idx[winning_idx[0]]] == 'O' && board[pos2idx[winning_idx[1]]] == 'O' && board[pos2idx[winning_idx[2]]] == 'O'){
            return 'O';
        }
        else if(board[pos2idx[winning_idx[0]]] == 'X' && board[pos2idx[winning_idx[1]]] == 'X' && board[pos2idx[winning_idx[2]]] == 'X'){
            return 'X';
        }
    }
    return '?';
}

char turn(Board board){
    int board_depth = depth(board);
    if(board_depth == 9) return '-';
    else if(winner(board) !=  '?') return '-';
    else if(board_depth % 2 == 0) return 'X';
    else if(board_depth % 2 == 1) return 'O';
    else return '-';
}

void init_board(Board board){
    int idx = board_hash(board);
    struct BoardNode * curr = &htable[idx];
    curr->init = 1;
    curr->turn = turn(board);
    curr->depth = depth(board);
    strcpy(curr->board, board);
    curr->winner = winner(board);
    //print_node(curr);
}

void join_graph(Board board){
    //printf("%s", board);
    int hash = board_hash(board);
    struct BoardNode * curr_node = &htable[hash];
    //printf("%d\n", hash);
    //print_node(curr_board);
    for(int i = 0; i < 9; i++){
        if(curr_node->winner != '?') return;
        else if(board[pos2idx[i]] == 'X' ||  board[pos2idx[i]] == 'O'){
            curr_node->move[i] = -1;
        }
        else{
            Board board_copy;
            strcpy(board_copy, board);
            board_copy[pos2idx[i]] = turn(board_copy);
            int new_hash = board_hash(board_copy);
            curr_node->move[i] = new_hash;
            //printf("%d\n", new_hash);
            struct BoardNode * next = &htable[new_hash];
            //print_node(next);
            if(next->init == 0){
                //exit(-1);
                init_board(board_copy);
                //print_node(next);
                join_graph(board_copy);
            }
        }
    }
}

void compute_score_node(struct BoardNode * node){
    if(node->winner == 'X') node->score = 1;
    else if(node->winner == 'O') node->score = -1;
    else if(node->winner == '-') node->score = 0;
    else if(node->turn == 'X'){
        int max = INT_MIN;
        for(int i = 0; i < 9; i++){
            if(node->move[i] != -1){
                struct BoardNode * next_node = &htable[node->move[i]];
                compute_score_node(next_node);
                if(next_node->score > max){
                    max = next_node->score;
                }
            }
        }
        node->score = max;
    }
    else if(node->turn == 'O'){
        int min = INT_MAX;
        for(int i = 0; i < 9; i++){
            if(node->move[i] != -1){
                struct BoardNode * next_node = &htable[node->move[i]];
                compute_score_node(next_node);
                if(next_node->score < min){
                    min = next_node->score;
                }
            }
        }
        node->score = min;
    }
}

void compute_score(){
    int hash = board_hash(START_BOARD);
    compute_score_node(&htable[hash]);
}

int best_move(int board){
    struct BoardNode * node = &htable[board];
    if(node->turn == 'X'){
        //printf("RETURN: X\n");
        int ret = -1;
        int max = INT_MIN;
        for(int i = 0; i < 9; i++){
            if(node->move[i] != -1){
                struct BoardNode * next_node = &htable[node->move[i]];
                if(next_node->score > max){
                    max = next_node->score;
                    ret = i;
                }
            }
        }
        return ret;
    }
    else if(node->turn == 'O'){
        int ret = -1;
        int min = INT_MAX;
        for(int i = 0; i < 9; i++){
            if(node->move[i] != -1){
                struct BoardNode * next_node = &htable[node->move[i]];
                if(next_node->score < min){
                    min = next_node->score;
                    ret = i;
                }
            }
        }
        return ret;
    }
    return -1;
}

/*
int main(int argc, char * argv[]){
    init_boards();
    join_graph(START_BOARD);
    return 0;
}
*/