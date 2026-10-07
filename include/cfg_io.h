#ifndef CFG_IO_H
#define CFG_IO_H "cfg_io.h"

#include "labyrinth.h"

#define SAVE_PATH "../data/labyrinths"

/**
 * Enum représentant l'état de retour des opérations d'enregistrement/chargement de labyrinthes
 */
typedef enum LabyrinthIOStatus{
    LABYRINTH_IO_SUCCESS,
    LABYRINTH_IO_PERMISSION_ERROR,
    LABYRINTH_IO_NOT_FOUND,
    LABYRINTH_IO_CORRUPTED,
    LABYRINTH_IO_INVALID_NAME,
    LABYRINTH_IO_INVALID_FORMAT
}LabyrinthIOStatus;

/**
 * Enregistre un labyrinthe dans un fichier .cfg.
 * 
 * @param name le nom du labyrinthe. Ce nom sera aussi le nom du fichier dans lequel le labyrinthe est enregistré.
 * @param labyrinth pointeur vers le labyrinthe à enregistrer. 
 */
int save_labyrinth(char** name, Labyrinth* labyrinth);

/**
 * Permet de charger un labyrinthe depuis un fichier cfg.
 * 
 * @param name le nom du labyrinth à recharger. Celui-ci doit être le même que celui du fichier cfg dans lequel le labyrinth est enregisré.
 */
Labyrinth* load_labyrinth(char** name, int load_state);

#endif
