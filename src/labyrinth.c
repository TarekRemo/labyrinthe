#include "labyrinth.h"

int is_valid_labyrinth_params(Difficulty difficulty, int height, int width){
    return height >= 3 && 
           width >= 3  &&
           height%2 == 1 && 
           width%2 == 1 &&
           (difficulty == EASY || difficulty == HARD);
}

char get_cell_representation(Cell content){
    switch (content){
        case EMPTY:
            return ' ';

        case WALL:
            return '#';

        case DOOR:
            return '_';

        case KEY:
            return '^';

        case TREASURE:
            return '$';

        case TRAP:
            return 'X';

        case PLAYER:
            return 'O';

        default:
            return 'E'; //erreur
    }
}