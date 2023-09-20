#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <ctype.h>
#include <dirent.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h> //Para fork
#include <sys/wait.h> //Para fork
#define num_cartas 4
#define num_tableros 8

//Se definen variables globales
char*** Tableros;
int glob = 0;


//Estructura de Jugador
typedef struct{
   char* Carta;
   int Turno_actual;
   int Tesoros_encontrados;
   //int prox_x;
   //int prox_y;
}Jugador;


/*
IMPORTANTE:

LOS JUGADORES ESTAN REPRESENTADOS POR NUMEROS DEL 1 AL 4

*/



/*
La funcion calcula la longitud de la matriz

Parametros :
   char* nom_archivo : puntero tipo char que apunta al nombre del archivo a leer 
   
Retorno :
   Retorna un entero que repreesenta el doble de longitud de la matriz
 
*/
int longit(char*nom_archivo){
    FILE* archivo = fopen(nom_archivo, "r"); 
    char bufer[20];
    fgets(bufer, 20, archivo); //Se salta la primea linea
    int longitud = strlen(fgets(bufer, 20, archivo));
    fclose(archivo);
    return longitud;
}



//NO ESTA IMPLEMENTADA
int RandomPos(){

}



/*
La funcion realiza movimiento del personaje en el tablero

Parametros :
   char **matriz : puntero tipo char que apunta a la matriz que se va a mostrar
   int tam : entero que representa el tamaño de la matriz
   
Retorno :
   Nada, ya que es tipo void
 
*/
void MostrarEnTablero(char **matriz, int tam, char jugador){
    //Si el jugador es igual a 1, es decir somos nosotros osea es nuestro turno
    if(jugador == 'J₁'){
        int seleccion;
        int x = 0;
        int y = 0;
        int val = 1;
        bool paso = false;
        
        while (val == 1){
            system("clear");
            printf("----------------------------------------------------------\n");
            //Imprimo el tablero y encuentro la posición del personaje, esta posición es en base al primer 0 que se encuentre
            for(int i = 0; i < tam; i++){
                for(int j = 0; j < tam; j++){
                    printf("%c     ", matriz[i][j]);
                    if((matriz[i][j] - '0') == 0 && !paso){
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
            printf("Tu posicion actual es: (%d,%d)\n", x, y);
            
            scanf("%d", &seleccion);
            
            if(seleccion == 1) {
                if(x - 1 >= 0 && (matriz[x - 1][y] - '0') == 0) {
                    matriz[x][y] = '0';
                    matriz[x - 1][y] = jugador;
                    x--;
                }else{
                    printf("No se puede mover hacia Arriba\n");
                }
            }else if(seleccion == 2) {
                if(x + 1 < tam && (matriz[x + 1][y] - '0') == 0) {
                    matriz[x][y] = '0';
                    matriz[x + 1][y] = jugador;
                    x++;
                }else{
                    printf("No se puede mover hacia Abajo\n");
                }
            }else if(seleccion == 3) {
                if(y + 1 < tam && (matriz[x][y + 1] - '0') == 0) {
                    matriz[x][y] = '0';
                    matriz[x][y + 1] = jugador;
                    y++;
                }else{
                    printf("No se puede mover hacia Derecha\n");
                }
            }else if(seleccion == 4) {
                if(y - 1 >= 0 && (matriz[x][y - 1] - '0') == 0) {
                    matriz[x][y] = '0';
                    matriz[x][y - 1] = jugador;
                    y--;
                }else{
                    printf("No se puede mover hacia Izquierda\n");
                }
            }else{
                exit(0);
            }
            
            printf("----------------------------------------------------------\n");
            printf("                 Presione 1 para continuar:               \n");
            printf("----------------------------------------------------------\n");
            scanf("%d", &val);
        }

    }else{
        printf("----------------------------------------------------------\n");
        printf("                   Espera al otro jugador                 \n");
        printf("----------------------------------------------------------\n");
        //Si el jugador es distinto a 1, es decir somos sus hijos osea el bot juega
        int sele_bot;
        int x = 0;
        int y = 0;
        int val = 1;
        bool paso = false;
        
        while (val == 1){
            system("clear");
            for (int i = 0; i < tam; i++) {
                for (int j = 0; j < tam; j++) {
                    if ((matriz[i][j] - '0') == 0 && !paso) {
                        x = i;
                        y = j;
                        paso = true;
                    }
                }
            }
            sele_bot = rand() % 4 + 1;
            if(sele_bot == 1) {
                if(x - 1 >= 0 && (matriz[x - 1][y] - '0') == 0){
                    matriz[x][y] = '0';
                    matriz[x - 1][y] = jugador;
                    x--;
                    val = 0;
                }else{
                    break;
                }
            }else if(sele_bot == 2) {
                if(x + 1 < tam && (matriz[x + 1][y] - '0') == 0){
                    matriz[x][y] = '0';
                    matriz[x + 1][y] = jugador;
                    x++;
                    val = 0;
                }else{
                    break;
                }
            }else if(sele_bot == 3) {
                if(y + 1 < tam && (matriz[x][y + 1] - '0') == 0){
                    matriz[x][y] = '0';
                    matriz[x][y + 1] = jugador;
                    y++;
                    val = 0;
                }else{
                    break;
                }
            }else if(sele_bot == 4) {
                if(y - 1 >= 0 && (matriz[x][y - 1] - '0') == 0){
                    matriz[x][y] = '0';
                    matriz[x][y - 1] = jugador;
                    y--;
                    val = 0;
                }else{
                    break;
                }
            }else{
                val = 1;
            }
        }
    }
}



/*
La funcion inicializa los jugadores en el tablero

Parametros :
   char **matriz : puntero tipo char que apunta a la matriz que se va a mostrar
   int tam : entero que representa el tamaño de la matriz
   
Retorno :
   Nada, ya que es tipo void
 
*/
void Forks(Jugador* player){
    //Se guardan los tableros en un array
    printf("MATRIZ EN ARRAY\n");
    for(int i = 0; i < num_tableros; i++){
        printf("Tablero %d\n", i + 1);
        for(int j = 0; j < 5; j++){
            for(int k = 0; k < 5; k++){
                printf("%c     ", Tableros[i][j][k]);
            }
            printf("\n");
        }
        printf("\n");
        printf("\n");
        //MostrarEnTablero(Tableros[i], 5, '1'); //Test
    }

    pid_t jugador_pid[4];

    for (int i = 0; i < 4; i++) {
        jugador_pid[i] = fork();

        if(jugador_pid[i] == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        }else if(jugador_pid[i] == 0) {
            //Código para el proceso hijo (Jugadores 1, 2, 3, 4)
            srand(time(NULL) + getpid());
            for (int j = 0; j <= 15; j++){
                int var = getpid();
                printf("Informacion Jugador %d: PID: %d ! Carta: %s ! Turno Actual: %d ! Tesoros Encontrados: %d \n", i + 1,var,player[i].Carta,player[i].Turno_actual,player[i].Tesoros_encontrados);
                player[i].Turno_actual++;
                sleep(1);
            }
            //Salir del bucle del proceso hijo
            exit(0);
        }
    }

    //Código para el proceso padre
    for(int i = 0; i < 4; i++){
        //Espera a que cada proceso hijo termine
        waitpid(jugador_pid[i], NULL, 0);
    }
}



//NO ESTA IMPLEMENTADA
/*
La funcion cambiar los valores de la matriz por los tesoros

Parametros :
   char val : char que representa un valor de la matriz
   
Retorno :
   Retorna un char que repreesenta el valor modificado al entregado
 
*/
char Tesoros(char val){
    int numAleat = rand() % 100;
    int numAleat2 = rand() % 3;
    int prob = 25;
    if((val - '0')==0){
        char var1;
        if(numAleat<=prob){
            if(numAleat2 == 0){
                var1 = 'c';
            }if(numAleat2 == 1){
                var1 = 't';
            }if(numAleat2 == 2){
                var1 = 'n';
            }if(numAleat2 == 3){
                var1 = 'p';
            }
            return var1;                                                      
        }else{

            return val;
        }

    }else{
        return val;
    }

}



/*
La funcion cambiar los valores de la matriz por los eventos

Parametros :
   char val : char que representa un valor de la matriz
   
Retorno :
   Retorna un char que repreesenta el valor modificado al entregado
 
*/
char Eventos(char val){
    int numAleat = rand() % 100;
    int numAleat2 = rand() % 3;
    int prob = 25;
    if((val - '0')==0){
        char var1;
        if(numAleat<=prob){
            if(numAleat2 == 0){
                var1 = 'c';
            }else if(numAleat2 == 1){
                var1 = 't';
            }else if(numAleat2 == 2){
                var1 = 'n';
            }else if(numAleat2 == 3){
                var1 = 'p';
            }
            return var1;                                                      
        }else{
            return val;
        }
    }else{
        return val;
    }

}



/*
La funcion guarda el tablero en forma de matriz para jugar con el personaje

Parametros :
   char* nom_archivo : puntero tipo char que apunta al nombre del archivo a leer 
   
Retorno :
   Nada, ya que es tipo void
 
*/
void CrearTablero(char* nom_archivo){
    //Open the file for reading
    FILE* archivolectura = fopen(nom_archivo, "r");
    if(archivolectura == NULL){
        perror("Error opening file");
    }
    int longitud = longit(nom_archivo);
    //char **matriz = malloc(sizeof(char *) * (longitud / 2)); //No es necesario
    Tableros[glob] = malloc(sizeof(char *) * (longitud / 2));;

    for(int i = 0; i < longitud / 2; i++){
        Tableros[glob][i] = malloc(sizeof(char *) * (longitud / 2));
        for(int j = 0; j < longitud / 2; j++) {
            int numAleat = rand() % 100;
            int numAleat2 = rand() % 3;
            char variable;
            if(fscanf(archivolectura, "%c ", &variable) == 1){
                char valor = Eventos(variable);
                Tableros[glob][i][j] = valor;
                printf("%c     ", valor);
            }else{
                // Handle end of file or other read errors
                break;
            }
        }
        printf("\n");
    }
    
    glob++;
    fclose(archivolectura);
}



/*
La funcion revisa el directorio actual para encontrar los archivo txt a leer

Parametros :
   No recibe ningun parametro

Retorno :
   Nada, ya que es tipo void
 
*/
char** LeerDir(){
    DIR* d = opendir(".");
    struct dirent* dentry;
    char** directorio = NULL;
    int k = 0;

    while((dentry = readdir(d)) != NULL){
        if(strstr(dentry->d_name, ".txt") != NULL){
            if(strstr(dentry->d_name, "Inicio.txt") == NULL){
                //Reasigna memoria para directorio
                directorio = realloc(directorio, (k + 1) * sizeof(char*));
                if(directorio == NULL){
                    perror("Error allocating memory");
                    exit(EXIT_FAILURE);
                }

                // Asigna memoria para el nombre del archivo y copia el nombre
                directorio[k] = malloc(strlen(dentry->d_name) + 1);
                if(directorio[k] == NULL){
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
void swap(char **a, char **b){
    char *temp = *a;
    *a = *b;
    *b = temp;
}



/*
La funcion desordena una lista usando el algoritmo de Fisher-Yates

Parametros :
   char **arr : puntero tipo char que apunta a una cadena de cadenas de char
   int n : entego que representa el tamaño de la lista

Retorno :
   Nada, ya que es tipo void
 
*/
void shuffle(char **arr, int n){
    srand(time(NULL));
    for (int i = n - 1; i > 0; i--){
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

    //Aqui se guardan todos los tableros
    Tableros = (char***)malloc(sizeof(char**) * num_tableros);

    //Aqui estan todos los jugadores
    Jugador* jugadores = (Jugador*)malloc(sizeof(Jugador) * 4);

    //Se crean las cartas
    char *Cartas[num_cartas] = {"Buscar-Arriba", "Escaleras-Abajo", "Escaleras-Izquierda", "Buscar-Derecha"};

    bool cartas_asignadas[num_cartas];
    for(int i = 0; i < num_cartas; i++){
        cartas_asignadas[i] = false;
    }

    // Inicializar generador de números aleatorios
    srand(time(NULL));

    printf("Revolviendo el mazo de cartas\n");
    shuffle(Cartas, num_cartas);
    

    for(int i = 0; i < 4; i++){
        int numero;
        do{
            numero = rand() % num_cartas;
        }while(cartas_asignadas[numero]);
        jugadores[i].Carta = Cartas[numero];
        cartas_asignadas[numero] = true;
        printf("La carta del Jugador %d es: %s\n", i + 1, jugadores[i].Carta);
    }
    printf("\n");

    
    char **obj = LeerDir();
    shuffle(obj, num_tableros);
    
    for(int i = 0; i<num_tableros; i++){
        printf("%s\n", obj[i]);
        char *cadena = obj[i];
        CrearTablero(cadena);
        printf("\n");
    }

    //Se crean los Jugadores
    Forks(jugadores);

    //Subindices en C
    /*
    printf("J₁");
    printf("J₂");
    printf("J₃");
    printf("J₄");
    */

    //Esta parte la hace la funcion Forks(jugadores) en la linea 550
    /*
    //Se guardan los tableros en un array
    printf("MATRIZ EN ARRAY\n");
    for(int i = 0; i < num_tableros; i++){
        printf("Tablero %d\n", i + 1);
        for(int j = 0; j < 5; j++){
            for(int k = 0; k < 5; k++){
                printf("%c     ", Tableros[i][j][k]);
                
            }
            printf("\n");
            
        }
        printf("\n");
        printf("\n");
        MostrarEnTablero(Tableros[i], 5, '1'); //Test    
    }
    */

    

    

    /*
    //Liberar la memoria antes de salir
    for (int i = 0; i < num_tableros; i++) {
        free(Tableros[i]);
    }
    free(Tableros);
    */

    return 0;
}