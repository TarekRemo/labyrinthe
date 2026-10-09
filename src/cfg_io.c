#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include "cfg_io.h"
#include "utils.h"

/**
 * Permet de vérifier que le dossier de sauvegarde existe et est accessible, le crée sinon.
 * 
 * @return LABYRINTH_IO_SUCCESS si le dossier existe ou si sa création est réussie. 
 */
static LabyrinthIOStatus check_save_folder(void);

/**
 * Permet de vérifier si un fichier existe.
 * 
 * @param file_path le chemin du fichier à chercher.
 * 
 * @return 1 si le fichier existe, 0 sinon.
 */
static int file_exists(char* file_path);

/**
 * Permet de vérifier qu'un nom de labyrinthe est valide.
 * 
 * Un nom valide est non vide, ne dépasse pas MAX_LABYRINTH_NAME_LENGTH caractères et ne contient
 *  que des lettres non accentuées, des chiffres, '_' ou '-'.
 * 
 * @param name le nom à vérifier.
 * 
 * @return 1 si le nom est valide, 0 sinon.
 */
static int is_valid_labyrinth_name(char* name);

LabyrinthIOStatus save_labyrinth(char* name, Labyrinth* labyrinth){
    if(labyrinth == NULL || labyrinth->cells == NULL){
        return LABYRINTH_IO_CORRUPTED;
    }

    if(!is_valid_labyrinth_params(labyrinth->difficulty, labyrinth->height, labyrinth->width)){
        return LABYRINTH_IO_CORRUPTED;
    }

    if(!is_valid_labyrinth_name(name)){
        return LABYRINTH_IO_INVALID_NAME;
    }

    //Vérifier que le dossier de sauvegarde existe
    LabyrinthIOStatus save_folder_status = check_save_folder();
    if(save_folder_status != LABYRINTH_IO_SUCCESS){
        return save_folder_status;
    }

    char* file_path = malloc(strlen(SAVE_PATH)+strlen(name)+5);
    if(file_path == NULL){
        return LABYRINTH_IO_UNKNOWN;
    }
    strcpy(file_path, SAVE_PATH);
    strcat(file_path, name);
    strcat(file_path, ".cfg");

    //Vérifier qu'un labyrinthe avec ce nom n'existe pas déjà
    if(file_exists(file_path)){
        free(file_path);
        return LABYRINTH_IO_ALREADY_EXISTS;
    }

    FILE* save_file = fopen(file_path, "w");
    if(save_file == NULL){
        free(file_path);
        return LABYRINTH_IO_UNKNOWN;
    }

    /**
     * Format : 
     * 1- Nom du labyrinthe 
     * 2- difficulté (0=easy, 1=hard)
     * 3- Hauteur
     * 4- Largeur
     */
    fprintf(
        save_file,
        "%s\n%d\n%d\n%d\n", 
        name,
        labyrinth->difficulty,
        labyrinth->height,
        labyrinth->width
    );

    for(int line = 0 ; labyrinth->height > line ; line++){
        for(int col = 0 ; labyrinth->width > col ; col++){
            fprintf(save_file, "%c", get_cell_representation(labyrinth->cells[line][col]));
        }
        fprintf(save_file, "\n");
    }

    free(file_path);
    fclose(save_file);

    return LABYRINTH_IO_SUCCESS;
}

LabyrinthIOStatus load_labyrinth(char* name, Labyrinth* labyrinth){
    labyrinth = NULL;

    if(!is_valid_labyrinth_name(name)){
        return LABYRINTH_IO_INVALID_NAME;
    }

    char* file_path = malloc(strlen(SAVE_PATH) + strlen(name) + 5);
    strcpy(file_path, SAVE_PATH); 
    strcat(file_path, name);
    strcat(file_path, ".cfg");

    if(check_save_folder() != LABYRINTH_IO_SUCCESS || !file_exists(file_path)){
        free(file_path);
        return LABYRINTH_IO_NOT_FOUND;
    }

    FILE* save_file = fopen(file_path, "r");
    free(file_path); //on n'en a plus besoin
    if(save_file == NULL){
        fclose(save_file);
        return LABYRINTH_IO_UNKNOWN;
    }

    //Ignorer la 1ère ligne contenant le nom
    char name_line[MAX_LABYRINTH_NAME_LENGTH + 2];   // nom + '\n' + '\0'
    if(fgets(name_line, sizeof(name_line), save_file) == NULL){
        fclose(save_file);
        return LABYRINTH_IO_INVALID_FORMAT;
    }

    int difficulty, height, width;
    if(
        !read_int(save_file, &difficulty) || 
        !read_int(save_file, &height) ||
        !read_int(save_file, &width) 
    ){
        fclose(save_file);
        return LABYRINTH_IO_INVALID_FORMAT;
    }

    if(!is_valid_labyrinth_params(difficulty, height, width)){
        fclose(save_file);
        return LABYRINTH_IO_CORRUPTED;
    }

    labyrinth = allocate_labyrinth(difficulty, height, width);
    if(labyrinth == NULL){
        fclose(save_file);
        return LABYRINTH_IO_MEMORY_ERROR;
    }

    char c;
    Cell content;
    for(int line = 0 ; height > line ; line++){
        for(int col = 0 ; width > col ; col++){
            c=fgetc(save_file);

            //caractères non attendu
            if( (content = get_cell(c)) == -1 ){
                free_labyrinth(labyrinth);
                fclose(save_file);
                return LABYRINTH_IO_INVALID_FORMAT;
            }

            labyrinth->cells[line][col] = content;
        }

        c=fgetc(save_file);

        //retour à la ligne obligatoire à la fin de chaque ligne sauf pour la dernière ligne 
        if(line < height-1){
            if(c != '\n'){
                free_labyrinth(labyrinth);
                fclose(save_file);
                return LABYRINTH_IO_INVALID_FORMAT;
            }
        }
        //fin de fichier ou retour à la ligne obligatoire pour la dernière ligne
        else{
            if(c != '\n' && c != EOF && c != '\0'){
                free_labyrinth(labyrinth);
                fclose(save_file);
                return LABYRINTH_IO_INVALID_FORMAT;
            }
        }
    }

    return LABYRINTH_IO_SUCCESS;
}


static LabyrinthIOStatus check_save_folder(void){
    struct stat st = {0};   
    if (stat(SAVE_PATH, &st) == -1) {
        switch(errno){
            case ENOENT: //Le dossier n'existe pas, faut le créer
                if(mkdir(SAVE_PATH, 0700) == -1){
                    return errno == EACCES ? LABYRINTH_IO_PERMISSION_ERROR : LABYRINTH_IO_UNKNOWN;
                }
                return LABYRINTH_IO_SUCCESS;
            
            case EACCES: //erreur de permission
                return LABYRINTH_IO_PERMISSION_ERROR;
            
            default: //autres erreurs
                return LABYRINTH_IO_UNKNOWN;
        }
    }

    //véifier que c'est un répértoire et non pas un fichier
    if(!S_ISDIR(st.st_mode)){
        return LABYRINTH_IO_INVALID_FORMAT; 
    }

    return LABYRINTH_IO_SUCCESS;
}

static int file_exists(char* file_path){
    struct stat st = {0};
    int result;
    if (stat(file_path, &st) == -1) {
        switch(errno){
            case ENOENT: //Le fichier n'existe pas
                result = 0;
                break;
            
            default: //Une autre erreur signifie que le fichier existe
                result = 1;
                break;
        }
    }else{
        result = 1; //pas d'erreur, donc le fichier existe
    }

    return result;
}

static int is_valid_labyrinth_name(char* name){
    if(name == NULL){
        return 0;
    }

    size_t length = strlen(name);
    if(length == 0 || length > MAX_LABYRINTH_NAME_LENGTH){
        return 0;
    }

    for(size_t i = 0 ; length > i ; i++){
        char c = name[i];
        int is_allowed = (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9')
            || c == '_' || c == '-';
        if(!is_allowed){
            return 0;
        }
    }

    return 1;
}
