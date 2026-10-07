#include "labyrinth.h"

int is_valid_labyrinth_params(Difficulty difficulty, int height, int width){
    return height >= 3 && 
           width >= 3  &&
           height%2 == 1 && 
           width%2 == 1 &&
           (difficulty == EASY || difficulty == HARD);
}