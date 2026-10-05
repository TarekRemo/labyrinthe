#ifndef GENERATOR_H
#define GENERATOR_H "generator.h"

#include "labyrinth.h"

#define DEFAULT_LABYRINTH_HEIGHT 11
#define DEFAULT_LABYRINTH_WIDTH 25

/**
 * Permet de générer un labyrinthe en choisissant la difficulté, la hauteur et la largeur.
 * 
 * Cette fonction retourne un pointeur alloué dynamiquement vers le labyrinthe généré. 
 *  Il faut donc penser à libérer cette mémoire en utilisant free_labyrinth().
 * 
 * Si la hauteur ou la largeur sont égales ou inférieur à 0, alors DEFAULT_LABYRINTH_HEIGHT et 
 *   DEFAULT_LABYRINTH_WIDTH seront utilisés.
 * 
 * Une hauteur et une largeur minimales de 3 sont exigées.
 * 
 * @param difficulty la difficulté
 * @param height la hauteur du labyrinthe
 * @param width la largeur du labyrinthe
 * 
 * @return le pointeur vers le labyrinthe généré
 */
Labyrinth* generate_labyrinth(Difficulty difficulty, int height, int width);

/**
 * Permet de libérer la mémoire alloué pour un labyrinthe
 * 
 * @param labyrinth pointeur vers le labyrinthe à libérer
 * 
 * @return void
 */
void free_labyrinth(Labyrinth* labyrinth);

#endif
