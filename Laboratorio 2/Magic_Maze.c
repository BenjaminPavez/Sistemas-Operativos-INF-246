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



int longit(char*nom_archivo){
    FILE* archivo = fopen(nom_archivo, "r"); 
    char bufer[20];
    fgets(bufer, 20, archivo); //Se salta la primea linea
    int longitud = strlen(fgets(bufer, 20, archivo));
    fclose(archivo);
    return longitud;
}



int RandomPos(){

}



void MostrarEnTablero(char **matriz, int tam){
    int seleccion;
    int x = 0;
    int y = 0;
    int val = 1;
    while(val == 1){
        system("clear");
        printf("----------------------------------------------------------\n");
        //Imprimo el tablero y encuentro la posicion del personaje, esta posicion es en base al primer 0 que se encuentre
        bool paso = false;
        for (int i = 0; i < tam; i++) {
            for (int j = 0; j <  tam; j++) {
                printf("%c     ", matriz[i][j]);
                if((matriz[i][j] - '0')==0 && paso == false){
                    x = i;
                    y = j;
                    paso = true;
                }
            }
            printf("\n");
            printf("\n");
        }
        printf("----------------------------------------------------------\n");
        printf("Arriba : 1  |  Abajo : 2  |  Derecha : 3  |  Izquierda : 4\n");
        printf("----------------------------------------------------------\n");
        printf("Tu posicion actual es: (%d,%d)\n", x=1, y+1);
        
        scanf("%d", &seleccion);
        if(seleccion == 1){
            if((x - 1) < 0){
                printf("No se puede mover hacia Arriba\n");
            }else{
                if((matriz[x-1][y] - '0') == 0){ //(matriz[x-1][y] - '0') == 0
                    matriz[x][y] = '0';
                    matriz[x-1][y] = '?';
                    x--;
                    
                }else{
                    printf("No se puede mover hacia Arriba\n");
                }
                
            }
            
        }else if(seleccion == 2){
            if((x + 1) > tam){
                printf("No se puede mover hacia Abajo\n");
            }else{
                if((matriz[x+1][y] - '0') == 0){ //(matriz[x+1][y] - '0') == 0
                    matriz[x][y] = '0';
                    matriz[x+1][y] = '?';
                    x++;
                    
                }else{
                    printf("No se puede mover hacia Abajo\n");
                }
                
            }
        
        }else if(seleccion == 3){
            if((x + 1) > tam){
                printf("No se puede mover hacia Derecha\n");
            }else{
                if((matriz[x][y+1] - '0') == 0){ //(matriz[x][y+1] - '0') == 0
                    matriz[x][y] = '0';
                    matriz[x][y+1] = '?';
                    y++;
                    
                }else{
                    printf("No se puede mover hacia Derecha\n");
                }
                
            }

        }else if(seleccion == 4){
            if((y - 1) < 0){
                printf("No se puede mover hacia Izquierda\n");
            }else{
                if((matriz[x][y-1] - '0') == 0){ //(matriz[x][y-1] - '0') == 0
                    matriz[x][y] = '0';
                    matriz[x][y-1] = '?';
                    y--;
                    
                }else{
                    printf("No se puede mover hacia Izquierda\n");
                }
                
            }

        }else{
            exit(0);
        }
        printf("----------------------------------------------------------\n");
        printf("                 Presione 1 para continuar:               \n");
        printf("----------------------------------------------------------\n");
        
        scanf("%d", &val);
    }


}



/*
La funcion guarda el tablero en forma de matriz para jugar con el personaje

Parametros :
   char* nom_archivo : puntero tipo char que apunta al nombre del archivo a leer 
   
Retorno :
   Retorna una estructura que almacena la matriz
 
*/

void CrearTablero(char* nom_archivo) {
    // Open the file for reading
    FILE* archivolectura = fopen(nom_archivo, "r");

    if (archivolectura == NULL) {
        perror("Error opening file");
    }

    int longitud = longit(nom_archivo);
    char **matriz = malloc(sizeof(char *) * longitud);

    for (int i = 0; i < (longitud/2); i++) {
        matriz[i] = malloc(sizeof(char) * longitud);
        for (int j = 0; j <  (longitud/2); j++) {
            if (fscanf(archivolectura, " %c", &matriz[i][j]) == 1) {
                printf("%c     ", matriz[i][j]);
            }else{
                // Handle end of file or other read errors
                break;
            }
        }
        printf("\n");
        printf("\n");
    }

    fclose(archivolectura);

    //return matriz;
    MostrarEnTablero(matriz, (longitud/2));
}



/*
La funcion revisa el directorio actual para encontrar los archivo txt a leer

Parametros :
   No recibe ningun parametro

Retorno :
   Nada, ya que es tipo void
 
*/
char** LeerDir() {
    DIR* d = opendir(".");
    struct dirent* dentry;
    char** directorio = NULL;
    int k = 0;

    while ((dentry = readdir(d)) != NULL) {
        if (strstr(dentry->d_name, ".txt") != NULL) {
            if (strstr(dentry->d_name, "Inicio.txt") == NULL) {
                // Reasigna memoria para directorio
                directorio = realloc(directorio, (k + 1) * sizeof(char*));
                if (directorio == NULL) {
                    perror("Error allocating memory");
                    exit(EXIT_FAILURE);
                }

                // Asigna memoria para el nombre del archivo y copia el nombre
                directorio[k] = malloc(strlen(dentry->d_name) + 1);
                if (directorio[k] == NULL) {
                    perror("Error allocating memory");
                    exit(EXIT_FAILURE);
                }
                strcpy(directorio[k], dentry->d_name);
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

    //Se crean las cartas
    char *Cartas[num_cartas] = {"Arriba", "Abajo", "Derecha", "Izquierda", "Buscar-Arriba", "Escaleras-Abajo", "Escaleras-Izquierda", "Buscar-Derecha"};
    char *cartas_disponibles[2];

    printf("Revolviendo el mazo de cartas\n");
    shuffle(Cartas, num_cartas);
    int val;
    printf("Presione 1 para sacar las tus cartas: ");
    scanf("%d", &val);
    if(val == 1){
        printf("Tus cartas son\n");
        for(int i = 0; i<2; i++){
            int numero = rand() % num_cartas-1;
            cartas_disponibles[i] = strdup(Cartas[numero]);
            printf("%d. %s\n",i+1,cartas_disponibles[i]);
        }
        printf("\n");
    }else{
        exit(0);
    }
     

    
    char **obj = LeerDir();
    shuffle(obj, 9);
    for(int i = 0; i<9; i++){
        printf("%s\n", obj[i]);
        char *cadena = obj[i];
        CrearTablero(cadena);
        printf("\n");
    }
    return 0;
}
