#ifndef GENRATOR_H
#define GENERATOR_H "generator.h"

#include <stdlib.h>
#include <stdio.h>
#include "labyrinth.h"

#define DEFAULT_LABYRINTH_HEIGHT 11
#define DEFAULT_LABYRINTH_WIDTH 25

/**
 * Permet de générer un labyrinth en choisissant la difficulté, la hauteur et la largeur.
 * 
 * Cette fonction retourne un pointeur alloué dynamiquement vers le labyrinth généré. 
 *  Il faut donc penser à libérer cette mémoire en utilisant free_labyrinth().
 * 
 * Si la hauteur ou la largeur sont égales ou inférieur à 0, alors DEFAULT_LABYRINTH_HEIGHT et 
 *   DEFAULT_LABYRINTH_WIDTH seront utilisés. 
 * 
 * @param Difficulty la difficulté
 * @param int la hauteur du labyrinth
 * @param int la largeur du labyrinth
 * 
 * @return Labyrinth*
 */
Labyrinth* generate_labyrinth(Difficulty difficulty, int height, int width);


#endif