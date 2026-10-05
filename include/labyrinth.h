#ifndef LABYRINTH_H
#define LABYRINTH_H "labyrinth.h"

/**
 * Enum permettant de représenter une cellule du labyrinth
 * Une cellule peut être vide ou contenir un mur, une porte, une clé ou un piège
 */
typedef enum Cell {
    EMPTY,
    WALL,
    DOOR,
    KEY,
    TREASURE,
    TRAP,
    PLAYER
} Cell;

/**
 * Enum permettant de représenter la difficulté du labyrinth
 */
typedef enum Difficulty {
    EASY,
    HARD
} Difficulty;

/**
 * Structure représentant un labyrinth
 * @var int height la hauteur du labyrinth
 * @var int width la largeur du labyrinth
 * @var Difficulty la difficulté du labyrinth
 * @var cell** cells un tableau 2D contenant les cellules du labyrinth                
 */
typedef struct Labyrinth {
    int height;
    int width;
    Difficulty difficulty;
    Cell** cells;
} Labyrinth;

#endif
