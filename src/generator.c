#include "generator.h"

#include <stdio.h>
#include <stdlib.h>

/**
 * Construit le labyrinthe dont la mémoire doit être déjà allouée par l'appellant.
 * 
 * @param labyrinth le pointeur vers le labyrinthe à construire.
 * 
 * @return 0 si la construction échoue, 1 sinon.
 */
static int construct_labyrinth(Labyrinth* labyrinth);

/**
 * Permet de libérer la mémoire allouée pour un tableau 2D des identifiants des cellules du labyrinthe. 
 * 
 * @param cells_valuesle tableau 2D des identifiants des cellules du labyrinthe
 * @param labyrinth_height la hauteur du labyrinthe.
 * 
 * @return void
 */
static void free_cells_values(int** cells_values, int labyrinth_height);

/**
 * Permet de vérifier si toutes les cellules du labyrinthe sont reliées.
 * 
 * @param cells_values le tableau 2D des identifiants des cellules du labyrinthe.
 * @param labyrinth_height la hauteur du labyrinthe.
 * @param labyrinth_width la largeur du labyrinthe.
 * 
 * @return 1 si toutes les cellules sont reliées, 0 sinon.
 */
static int all_cells_linked(int** cells_values, int labyrinth_height, int labyrinth_width);

Labyrinth* generate_labyrinth(Difficulty difficulty, int height, int width){

    // Vérifier s'il faut utiliser les valeurs par défaut
    height = height <= 0 ? DEFAULT_LABYRINTH_HEIGHT : height;
    width = width <= 0 ? DEFAULT_LABYRINTH_WIDTH : width;

    if(height < 3 || width < 3){
        fprintf(stderr, "La taille minimale du labyrinthe est de 3x3\n");
        return NULL;
    }

    //vérifier que la hauteur et la largeur sont impaires
    if(height%2 == 0 || width%2 == 0){
        fprintf(stderr, "La hauteur et la largeur du labyrinthe doivent être impaires\n");
        return NULL;
    }

    //Vérifier que la difficulté choisie est reconnue
    if(difficulty != EASY && difficulty != HARD){
        fprintf(stderr, "La difficulté %d choisie n'est pas reconnue\n", difficulty);
        return NULL;  
    }

    Labyrinth* labyrinth = malloc(sizeof(Labyrinth));
    if(labyrinth == NULL){
        fprintf(stderr, "Impossible d'allouer la mémoire nécessaire pour la génération du labyrinthe\n");
        return NULL;
    }

    //Allocation des lignes du tableau 2D des cellules
    labyrinth->cells = malloc(height * sizeof(Cell*));
    if(labyrinth->cells == NULL){
        fprintf(stderr, "Impossible d'allouer la mémoire nécessaire pour la génération du labyrinthe\n");
        free(labyrinth);
        return NULL;
    }

    //Allocation des colonnes de chaque ligne du tableau 2D des cellules
    for(int line = 0 ; height > line ; line++){
        labyrinth->cells[line] = malloc(width * sizeof(Cell));

        if(labyrinth->cells[line] == NULL){
            fprintf(stderr, "Impossible d'allouer la mémoire nécessaire pour la génération du labyrinthe\n");
            for(int i = 0 ; line > i ; i++){
                free(labyrinth->cells[i]);
            }

            free(labyrinth->cells);
            free(labyrinth);

            return NULL;
        }
    }

    labyrinth->height = height;
    labyrinth->width = width;
    labyrinth->difficulty = difficulty;

    if(construct_labyrinth(labyrinth) == 0){
        fprintf(stderr, "Impossible de construire le labyrinthe\n");
        free_labyrinth(labyrinth);
        return NULL; 
    }

    return labyrinth;
}

void free_labyrinth(Labyrinth* labyrinth){
    if(labyrinth != NULL){
        for(int line = 0 ; labyrinth->height > line ; line++){
            free(labyrinth->cells[line]);
        }

        free(labyrinth->cells);
        free(labyrinth);
    }
}

int construct_labyrinth(Labyrinth* labyrinth){
    if(labyrinth == NULL || labyrinth->cells == NULL){
        return 0;
    }

    int height = labyrinth->height;
    int width = labyrinth->width;

    //Allocation du tableau 2D des identifiants (une ligne à la fois)
    int** cells_values = malloc(height * sizeof(int*));
    if(cells_values == NULL){
        return 0;
    }
    for(int line = 0 ; height > line ; line++){
        cells_values[line] = malloc(width * sizeof(int));
        if(cells_values[line] == NULL){
            free_cells_values(cells_values, line);
            return 0;
        }
    }

    //Initialisation des cellules (cellules impaires = vides ; autres = murs)
    int current_cell_value = 1;
    for(int line = 0 ; height > line ; line++){
        for(int col = 0 ; width > col ; col++){
            if(line%2 == 1 && col%2 == 1){
                labyrinth->cells[line][col] = EMPTY;
                cells_values[line][col] = current_cell_value++;
            }
            else{
                labyrinth->cells[line][col] = WALL;
                cells_values[line][col] = 0;
            }
        }
    }

    //application de l'algorithme de fusion aléatoire de chemins
    //Il faut exactement (nombre de cellules - 1) fusions pour relier toutes les cellules
    int remaining_merges = (current_cell_value - 1) - 1;
    int random_line;
    int random_col;
    while(remaining_merges > 0){
        //Choisir un mur intérieur au hasard
        random_line = 1 + rand() % (height - 2);
        random_col = 1 + rand() % (width - 2);

        //Seuls les murs séparant deux cellules (une coordonnée paire, l'autre impaire) peuvent être ouverts
        if((random_line + random_col) % 2 == 0){
            continue;
        }

        //Récupérer les identifiants des deux cellules séparées par ce mur
        int first_value, second_value;
        if(random_line%2 == 1){
            //Mur vertical : cellules à gauche et à droite
            first_value = cells_values[random_line][random_col - 1];
            second_value = cells_values[random_line][random_col + 1];
        }
        else{
            //Mur horizontal : cellules au-dessus et en dessous
            first_value = cells_values[random_line - 1][random_col];
            second_value = cells_values[random_line + 1][random_col];
        }

        //Si les deux cellules sont déjà reliées, ouvrir ce mur créerait une boucle
        if(first_value == second_value){
            continue;
        }

        //Ouvrir le mur et propager l'identifiant de la première cellule à tout le second chemin
        labyrinth->cells[random_line][random_col] = EMPTY;
        cells_values[random_line][random_col] = first_value;
        for(int line = 0 ; height > line ; line++){
            for(int col = 0 ; width > col ; col++){
                if(cells_values[line][col] == second_value){
                    cells_values[line][col] = first_value;
                }
            }
        }
        remaining_merges--;
    }

    int success = all_cells_linked(cells_values, height, width);

    if(success){
        //Placer le joueur en haut à gauche et la porte en bas à droite
        labyrinth->cells[0][1] = PLAYER;
        labyrinth->cells[height-1][width-2] = DOOR;
    }

    free_cells_values(cells_values, height);

    return success;
}

static void free_cells_values(int** cells_values, int labyrinth_height){
    for(int line = 0 ; labyrinth_height > line ; line++){
        free(cells_values[line]);
    }
    free(cells_values);
}

static int all_cells_linked(int** cells_values, int labyrinth_height, int labyrinth_width){
    int unique_id = -1;

    for(int line = 0 ; labyrinth_height > line ; line++){
        for(int col = 0 ; labyrinth_width > col ; col++){
            //ignorer les murs
            if(cells_values[line][col] == 0){
                continue;
            }
            
            //Si l'identifiant de la première cellule n'a pas encore été trouvé
            if(unique_id == -1){
                unique_id = cells_values[line][col];
                continue;
            }

            if(cells_values[line][col] != unique_id){
                return 0;
            }
        }
    }

    return 1;
}
