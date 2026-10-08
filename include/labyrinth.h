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

/**
 * Permet de vérifier si les paramètres d'un labyrinthe sont valides.
 * 
 * @param difficulty la difficulté d'un labyrinthe
 * @param height la hauteur du labyrinthe
 * @param width la largeur du labyrinthe
 * 
 * @return 1 si les paramètres sont valides, 0 sinon.
 */
int is_valid_labyrinth_params(Difficulty difficulty, int height, int width);

/**
 * Permet d'obtenir le caractère représentant le contenu d'une cellule.
 * 
 * @param content le contenu de la cellule.
 * 
 * @return le caractère représentant graphiquement cette cellule.
 */
char get_cell_representation(Cell content);

#endif
