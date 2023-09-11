#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <ctype.h>
#include <dirent.h>
#include <unistd.h>



/*
La funcion guarda el tablero en forma de matriz para jugar con el personaje

Parametros :
   char* nom_archivo : puntero tipo char que apunta al nombre del archivo a leer 
   
Retorno :
   Retorna una estructura que almacena la matriz
 
*/
char** Tablero(char* nom_archivo){
    //Leo el archivo para calcular la longitud de la linea
    FILE* archivo = fopen(nom_archivo, "r"); 
    char bufer[20];
    fgets(bufer, 20, archivo); //Se salta la primea linea
    int longitud = strlen(fgets(bufer, 20, archivo));
    fclose(archivo);

    //Abro el archivo para extraer la sopa
    FILE* archivolectura = fopen(nom_archivo, "r");

    char **matriz = malloc(sizeof(char *) * longitud);

    //Se guarda el contenido del archivo en una estructura
    for(int i = 0; i < longitud; i++) {
        matriz[i] = malloc(sizeof(char) * longitud); //Asigna memoria para cada fila
        for(int j = 0; j < longitud; j++) {
            fscanf(archivolectura, "%c", &matriz[i][j]);
            printf("%c ",matriz[i][j]);
        }
        printf("\n");
    }
    fclose(archivolectura);

    return matriz;

}

int main(){

    Tablero("archivo.txt");

    /*
    //Jugadores
    __pid_t player1;
    player1 = fork();
    printf("Su pid es: %d\n",getpid());
    */
    return 0;
}