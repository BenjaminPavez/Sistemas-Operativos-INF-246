#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <ctype.h>
#include <dirent.h>
#include <unistd.h>
#include <time.h>

#define NUM_ACCIONES 6
#define NUM_DIRECCIONES 4




/*
La funcion guarda el tablero en forma de matriz para jugar con el personaje

Parametros :
   char* nom_archivo : puntero tipo char que apunta al nombre del archivo a leer 
   
Retorno :
   Retorna una estructura que almacena la matriz
 
*/
char** CrearTablero(char* nom_archivo){
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
            if(matriz[i][j] == 'J'){
                printf("%c",matriz[i][j]);

            }else{
                printf(" %c ",matriz[i][j]);
            }
            
        }
        printf("\n");
    }
    fclose(archivolectura);

    return matriz;

}

int RandomPos(){

}


void MostrarTablero(){

    /*
    ========================================
               Magic Maze
    ========================================

            Jugador Principal       Bot 1
            Tesoro: [ ]           Tesoro: [ ]

    ----------------------------------------
    | / | / | / | / | / | / | / | / | / | / |
    ----------------------------------------
    | / |   |   |   |   |   |   |   |   |   |
    ----------------------------------------
    | / |   |   |   |   |   |   |   |   |   |
    ----------------------------------------
    | / |   |   |   |   |   |   |   |   |   |
    ----------------------------------------
    | / |   |   |   |   |   |   |   |   |   |
    ----------------------------------------

            Jugador 3              Bot 2
            Tesoro: [ ]           Tesoro: [ ]

    ========================================
    Turno Actual: Jugador Principal
    ========================================
    1. Acción: [Buscar] [Escaleras] [Derecha] [Izquierda] [Arriba] [Abajo]
    Elige una acción: [ ]
    */

    printf("----------------------------------------");

}



/*
La funcion revisa el directorio actual para encontrar los archivo txt a leer

Parametros :
   No recibe ningun parametro

Retorno :
   Nada, ya que es tipo void
 
*/
char** LeerDir(){
    DIR *d = opendir(".");
    struct dirent *dentry;
    char **directorio = malloc(sizeof(char) * 10);
    int k = 0;
    
    while((dentry = readdir(d)) != NULL) {
        if(strstr(dentry->d_name, ".txt") != NULL){
            directorio[k] = malloc(sizeof(char) * (sizeof(dentry->d_name) / sizeof(dentry->d_name[0])));
            directorio[k] = dentry->d_name;
            k++;
        }
    }
    closedir(d);
    return directorio;
}



//Función para intercambiar dos elementos de un arreglo
/*
La funcion revisa el directorio actual para encontrar los archivo txt a leer

Parametros :
   No recibe ningun parametro

Retorno :
   Nada, ya que es tipo void
 
*/
void swap(char **a, char **b) {
    char *temp = *a;
    *a = *b;
    *b = temp;
}



// Función para desordenar una lista usando el algoritmo de Fisher-Yates
/*
La funcion revisa el directorio actual para encontrar los archivo txt a leer

Parametros :
   No recibe ningun parametro

Retorno :
   Nada, ya que es tipo void
 
*/
void shuffle(char **arr, int n) {
    srand(time(NULL));
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        swap(&arr[i], &arr[j]); // Usa la función swap corregida
    }
}



/*
La funcion revisa el directorio actual para encontrar los archivo txt a leer

Parametros :
   No recibe ningun parametro

Retorno :
   Nada, ya que es tipo void
 
*/
int main(){
    printf("========================================\n");
    printf("               Magic Maze               \n");
    printf("========================================\n");
    //printf("\n");

    char *Acciones[NUM_ACCIONES] = {"Buscar", "Escaleras", "Derecha", "Izquierda", "Arriba", "Abajo"};
    char *Direcciones[NUM_DIRECCIONES] = {"Norte", "Sur", "Este", "Oeste"}; 

    char *Cartas[NUM_ACCIONES] = {"Arriba", "Abajo", "Derecha", "Izquierda", "Buscar-Arriba", "Escaleras-Abajo", "Escaleras-Izquierda", "Buscar-Derecha"};

    char *cartas_disponibles[NUM_ACCIONES * 2 + NUM_DIRECCIONES * 6];
   

    printf("Revolviendo cartas\n");
    int val;
    printf("Presione 1 para sacar las tus cartas: ");
    scanf("%d", &val);
    if(val == 1){
        printf("Tus cartas son\n");
        for(int i = 0; i<2; i++){
            printf("%d. %s\n",i+1,cartas_disponibles[i]);
        }
        printf("\n");
    }else{
        exit(0);
    }
     
    


    //CrearTablero("tablero1.txt");
    char **obj = LeerDir();
    printf("Lista original: \n");
    for (int i = 0; i < 9; i++) {
        printf("%s\n", obj[i]);
    }

    shuffle(obj, 9);

    printf("\n");

    printf("Lista desordenada: \n");
    for (int i = 0; i < 9; i++) {
        printf("%s\n", obj[i]);
    }

    
    return 0;
}
