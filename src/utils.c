#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include "utils.h"

int read_int(FILE* input_descriptor, int* var){
    if(input_descriptor == NULL || var == NULL){
        return 0;
    }

    //Lire une ligne entière pour ne pas laisser de caractères en attente dans le flux
    char buffer[READ_INT_BUFFER_SIZE];
    if(fgets(buffer, sizeof(buffer), input_descriptor) == NULL){
        return 0;
    }

    //Si la ligne est trop longue, consommer le reste et refuser l'entrée
    if(strchr(buffer, '\n') == NULL && !feof(input_descriptor)){
        int c;
        while((c = fgetc(input_descriptor)) != '\n' && c != EOF);
        return 0;
    }

    char* end;
    errno = 0;
    long value = strtol(buffer, &end, 10);

    //Aucun chiffre n'a été lu (entrée vide, "abc", "-"...)
    if(end == buffer){
        return 0;
    }

    //Vérifier que la valeur tient dans un int
    if(errno == ERANGE || value < INT_MIN || value > INT_MAX){
        return 0;
    }

    //Seuls des espaces (dont le retour à la ligne) sont autorisés après le nombre
    while(*end != '\0'){
        if(!isspace((unsigned char)*end)){
            return 0;
        }
        end++;
    }

    *var = (int)value;
    return 1;
}
