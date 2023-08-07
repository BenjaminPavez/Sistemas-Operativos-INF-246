void Orientacion(char* nom_archivo) {
    printf("----------------------------------------------------------------\n");
    printf("Nombre del Archivo: %s\n", nom_archivo);
    printf("----------------------------------------------------------------\n");
    FILE* archivo = fopen(nom_archivo, "r"); // Modo lectura
    char bufer[1000]; // Aquí vamos a ir almacenando cada línea

    // Leer la primera línea para obtener la orientación
    if(fgets(bufer, 1000, archivo)) {
        bufer[strcspn(bufer, "\n")] ='\0'; 
        if(strcmp(bufer, "vertical") == 0 || strcmp(bufer, "horizontal") == 0){
            printf("Orientacion: %s\n", bufer);
            // Leer la segunda línea para obtener el tamaño del lado
            if (fgets(bufer, 1000, archivo)) {
                int longitud = strlen(bufer);
                printf("Tamaño: %d x %d\n",(longitud+1)/2, (longitud+1)/2);
                printf("----------------------------------------------------------------\n");
            }
        }else {
            printf("Error: %s\n", bufer);
        }
    }
    fclose(archivo); 
}


//Avance sopa de letras

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <ctype.h>



char* quitaespacios(char *cadena, char *cadenasin) {
    int i, j;
    i = 0;
    j = 0;
    while (cadena[i] != '\0') {
        if (cadena[i] != ' ') {
            cadenasin[j] = cadena[i];
            j++;
        }
        i++;
    }
    cadenasin[j] = '\0'; // Agrega el carácter nulo al final de la cadena resultante
    return cadenasin;
}



// Función para resolver la sopa de letras en la dirección dada (horizontal o vertical)
void solveWordSearch(FILE *archivo, char *targetWord, char *orientation) {
    int wordLength = strlen(targetWord);
    bool found = false;
    char bufer[1000]; // Aquí vamos a ir almacenando cada línea

    //Palabra a buscar en la sopa
    int texto_len = strlen(targetWord);
    char *textoMayus = malloc(sizeof(char) * texto_len); //reservando memoria para la variable a retornar
    for (int i = 0; i < texto_len; i++){
        textoMayus[i] = toupper(targetWord[i]); 
    }

    if(strcmp(orientation, "horizontal") == 0) {
        int fila = 0;
        while(fgets(bufer, sizeof(bufer), archivo)) {
            bufer[strcspn(bufer, "\n")] = '\0'; // Eliminamos el salto de línea
            for (int col = 0; col < strlen(bufer) - wordLength + 1; col++) {
                char *cadenafinal = (char *)malloc(strlen(&bufer[col]) + 1); // +1 para el carácter nulo
                quitaespacios(&bufer[col], cadenafinal);
                if(strstr(cadenafinal, textoMayus) != NULL) {
                    printf("%s\n",&bufer[col]);
                    printf("Palabra encontrada en fila %d, columna %d (horizontal).\n", fila, col);
                    found = true;
                    break;
                }
            }
            fila++;
        }
    }else if (strcmp(orientation, "vertical") == 0) {
        int fila = 0;
        int col = 0;
        int tam = 200;
        int puzzleSize = strlen(bufer);
        for(int x = 0; x < tam; x++){
            char *cadenafinal = (char *)malloc(tam);
            for(int y = 0; y < tam; y++){
                //strcat(cadenafinal, bufer[y][0]);
            }
            //printf();
        }
    }else{
        printf("Orientación inválida: %s\n", orientation);
        return;
    }
}




int main(){
    FILE* archivo = fopen("Carne.txt", "r"); // Modo lectura
    solveWordSearch(archivo,"Carne","vertical");
    /*
    //Palabra a buscar en la sopa
    char *palabra_buscar = "casa";
    int texto_len = strlen(palabra_buscar);
    char *textoMayus = malloc(sizeof(char) * texto_len); //reservando memoria para la variable a retornar
    for (int i = 0; i < texto_len; i++){
        textoMayus[i] = toupper(palabra_buscar[i]); 
    }

    //Sopa
    FILE* archivo = fopen("casa.txt", "r"); // Modo lectura
    
    char *cadena = "A X Y Z D B Y L G E H G J I B F E S H A Q G H Y Q W U B B E D R O C N K W X E P A Z U V C A S A B W";
    char *cadena2 = "c a s a";
    // Asignar memoria para la cadena resultante
    char *cadenafinal = (char *)malloc(strlen(cadena) + 1); // +1 para el carácter nulo


    quitaespacios(cadena, cadenafinal);

    // Liberar la memoria después de su uso
    //free(cadenafinal);
    */

    return 0;
}
