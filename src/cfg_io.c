#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "cfg_io.h"

int save_labyrinth(char** name, Labyrinth* labyrinth){
    if(labyrinth == NULL || labyrinth->cells == NULL){
        return LABYRINTH_IO_CORRUPTED;
    }

    if(!is_valid_labyrinth_params(labyrinth->difficulty, labyrinth->height, labyrinth->width)){
        return LABYRINTH_IO_CORRUPTED;
    }

    if(name == NULL){
        return LABYRINTH_IO_INVALID_NAME;
    }

    //Vérifier que le dossier existe, le créer sinon 
    struct stat st = {0};   
    if (stat(SAVE_PATH, &st) == -1) {
        mkdir(SAVE_PATH, 0700);
    }
}