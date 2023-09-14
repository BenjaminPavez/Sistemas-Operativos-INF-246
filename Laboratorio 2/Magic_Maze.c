#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <ctype.h>
#include <dirent.h>
#include <unistd.h>
#include <time.h>

#define num_cartas 8





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
            if(strstr(dentry->d_name, "Inicio.txt") != NULL){
                printf("Se encontro el archivo %s\n", dentry->d_name);
            }else{
                directorio[k] = malloc(sizeof(char) * (sizeof(dentry->d_name) / sizeof(dentry->d_name[0])));
                directorio[k] = dentry->d_name;
                k++;
            }
        }
    }
    closedir(d);
    return directorio;
}



/*
La funcion intercambiar dos elementos de un arreglo

Parametros :
   char **a : puntero tipo char que apunta al string que esta en la primera lista
   char **b : puntero tipo char que apunta al string que esta en la segunda lista

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
La funcion desordena una lista usando el algoritmo de Fisher-Yates

Parametros :
   char **arr : puntero tipo char que apunta a una cadena de cadenas de char
   int n : entego que representa el tamaño de la lista

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

    char *Cartas[num_cartas] = {"Arriba", "Abajo", "Derecha", "Izquierda", "Buscar-Arriba", "Escaleras-Abajo", "Escaleras-Izquierda", "Buscar-Derecha"};

    char *cartas_disponibles[2];

    printf("Revolviendo cartas\n");
    shuffle(Cartas, num_cartas);
    int val;
    printf("Presione 1 para sacar las tus cartas: ");
    scanf("%d", &val);
    if(val == 1){
        printf("Tus cartas son\n");
        for(int i = 0; i<2; i++){
            int numero = rand() % num_cartas-1;
            cartas_disponibles[i] = Cartas[numero];
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
