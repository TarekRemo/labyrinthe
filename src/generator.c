#include "../include/generator.h" 

Labyrinth* generate_labyrinth(Difficulty difficulty, int height, int width){

    // Vérifier s'il faut utiliser les valeurs par défaut
    height = height <= 0 ? DEFAULT_LABYRINTH_HEIGHT : height;
    width = width <= 0 ? DEFAULT_LABYRINTH_WIDTH : width;

    //vérifier que la hauteur et la largeur sont impaires
    if(height%2 == 0 || width%2 == 0){
        fprintf(stderr, "La hauteur et la largeur du labyrinth doivent être impaires\n");
        return NULL;
    }

    Labyrinth* labyrinth = malloc(sizeof(Labyrinth));
    labyrinth->cells = malloc(height*width*sizeof(Cell));

    int** cells_values = malloc(height*width*sizeof(int));

    //Initialisation des cellules (cellules impaires = vides ; autres = murs)
    int current_cell_value = 0;
    for(int line = 0 ; height > line ; line++){
        for(int col = 0 ; width > col ; col++){
            if(line%2 == 1 && col%2 == 1){
                labyrinth->cells[line][col] = EMPTY;
                cells_values[line][col] = current_cell_value++;
            }
            else{
                labyrinth->cells[line][col] = WALL;
            }
        }
    }
}
