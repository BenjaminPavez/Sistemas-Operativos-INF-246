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
#include <sys/ipc.h> //Para fork
#include <sys/shm.h> //Para fork
#define num_cartas 4
#define num_tableros 9
#define tam_tablero 5



//Se definen variables globales
char**** Tableros;
int glob = 0;
int Turno = 1;



//Estructura de Jugador
struct Jugador{
   int pid;
   char* Carta;
   int Turno_actual;
   int Tesoros_encontrados;
   //int prox_x;
   //int prox_y;
};



/*
IMPORTANTE:

LOS JUGADORES ESTAN REPRESENTADOS POR NUMEROS DEL J₁ AL J₄

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
La funcion realiza movimiento del Bot en el tablero

Parametros :
   char *jugador : puntero tipo char que apunta al numero del jugador, este puede ser J₂, J₃ o J₄
   struct Jugador info : estructura tipo Jugador que contiene la informacion del jugador
   
Retorno :
   Nada, ya que es tipo void
 
*/
void MostrarEnTableroBot(char *jugador, struct Jugador info, char*** tableroscopi){
    int seleccion;
    int x = 0;
    int y = 0;
    int val = 1;
    bool paso = false;
    bool flag = true;
    system("clear");
    printf("---------------------------------------------------------------------------------------------\n");
    printf("                                      Es el turno de %s                                      \n", jugador);
    printf("---------------------------------------------------------------------------------------------\n");
    printf("                    Turno Actual: %d                      Tu Carta: %s                       \n", Turno, info.Carta);
    printf("---------------------------------------------------------------------------------------------\n");
    //Imprimo el tablero y encuentro la posición del personaje, esta posición es en base al primer 0 que se encuentre
    for(int i = 0; i < tam_tablero; i++){
        for(int j = 0; j < tam_tablero; j++){
            printf("%s     ", tableroscopi[i][j]);
            if(strcmp(tableroscopi[i][j], jugador) == 0 && !paso){
                x = i;
                y = j;
                paso = true;
            }
        }
        printf("\n");
        printf("\n");
    }
    //Aqui se mueve el personaje
    printf("---------------------------------------------------------------------------------------------\n");
    printf(" Arriba : 1  |  Abajo : 2  |  Derecha : 3  |  Izquierda : 4  |  Usar Carta : 5  |  Salir : 6\n");
    printf("---------------------------------------------------------------------------------------------\n");
    printf("Tu posicion actual es: (%d,%d)\n", x, y);
    printf("-> ");
    scanf("%d", &seleccion);
    //getchar();  // Consumir el carácter de nueva línea en el búfer
    if(seleccion == 1){
        if(x - 1 >= 0 && strcmp(tableroscopi[x - 1][y], "0") == 0) {
            strcpy(tableroscopi[x][y], "0");
            strcpy(tableroscopi[x - 1][y], jugador);
            x--;
        }else{
            printf("%s no se puede mover hacia Arriba\n", jugador);
        }
    }else if(seleccion == 2){
        if(x + 1 < tam_tablero && strcmp(tableroscopi[x + 1][y], "0") == 0){
            strcpy(tableroscopi[x][y], "0");
            strcpy(tableroscopi[x + 1][y], jugador);
            x++;
        }else{
            printf("%s no se puede mover hacia Abajo\n", jugador);
        }
    }else if(seleccion == 3){
        if(y + 1 < tam_tablero && strcmp(tableroscopi[x][y + 1], "0") == 0){
            strcpy(tableroscopi[x][y], "0");
            strcpy(tableroscopi[x][y + 1], jugador);
            y++;
        }else{
            printf("%s no se puede mover hacia Derecha\n", jugador);
        }
    }else if(seleccion == 4){
        if(y - 1 >= 0 && strcmp(tableroscopi[x][y - 1], "0") == 0){
            strcpy(tableroscopi[x][y], "0");
            strcpy(tableroscopi[x][y - 1], jugador);
            y--;
        }else{
            printf("%s no se puede mover hacia Izquierda\n", jugador);
        }
    }else if(seleccion == 5){
        if(strcmp(info.Carta,"Buscar")==0) {
            //Falta implementar la conexion entre mapas y "B's"
        }else if(strcmp(info.Carta,"Escaleras")==0){
            while(flag){
                if(x - 1 >= 0 && strcmp(tableroscopi[x - 1][y],"E")==0){
                    printf("Salta una '%c'.\n", 'E');
                    strcpy(tableroscopi[x][y], "0");
                    strcpy(tableroscopi[x - 1][y], "J₁");
                    x--;
                    flag = false;

                }else if (x + 1 < tam_tablero && strcmp(tableroscopi[x+1][y], "E")==0){ //Dirección sur (abajo)
                    printf("Salta una '%c'.\n", 'E');
                    strcpy(tableroscopi[x][y], "0");
                    strcpy(tableroscopi[x + 1][y], "J₁");
                    x++;
                    flag = false;
                
                }else if (y + 1 < tam_tablero && strcmp(tableroscopi[x][y + 1],"E")==0){ //Dirección este (derecha)
                    printf("Salta una '%c'.\n", 'E');
                    strcpy(tableroscopi[x][y], "0");
                    strcpy(tableroscopi[x][y + 1], "J₁");
                    y++;       
                    flag = false;             
                
                }else if (y - 1 >= 0 && strcmp(tableroscopi[x][y - 1],"E")==0){ //Dirección oeste (izquierda)
                    printf("Salta una '%c'.\n", 'E');
                    strcpy(tableroscopi[x][y], "0");
                    strcpy(tableroscopi[x][y - 1], "J₁");
                    y--;
                    flag = false;
                
                }else{
                    printf("No se encontro Escaleras");
                    flag = false;
                }
            }
        }
        else{
            printf("Error al asignar carta\n");
        }
    }else if(seleccion == 6){
        val = 0;
    }


    printf("\n");
    printf("\n");
    printf("---------------------------------------------------------------------------------------------\n");
    printf("--------------------------------------------Test 2-------------------------------------------\n");
    printf("---------------------------------------------------------------------------------------------\n");
    for(int i = 0; i < tam_tablero; i++){
        for(int j = 0; j < tam_tablero; j++){
            printf("%s     ", tableroscopi[i][j]);
        }
        printf("\n");
        printf("\n");
    }

    //MostrarEnTableroBot("J₂", info, tableroscopi);
    
    

}



/*
La funcion realiza movimiento de nosotros en el tablero

Parametros :
   struct Jugador info : estructura tipo Jugador que contiene la informacion del jugador
   
Retorno :
   Nada, ya que es tipo void
 
*/
void MostrarEnTablero(struct Jugador* info, char*** tableroscopi){
    //Si el jugador es igual a 1, es decir somos nosotros osea es nuestro turno
    int seleccion;
    int x = 0;
    int y = 0;
    int val = 1;
    bool paso = false;
    bool flag = true;
    while(val == 1){
        system("clear");
        printf("---------------------------------------------------------------------------------------------\n");
        printf("                                          Es tu turno                                        \n");
        printf("---------------------------------------------------------------------------------------------\n");
        printf("                    Turno Actual: %d                      Tu Carta: %s                       \n",Turno, info[0].Carta);
        printf("---------------------------------------------------------------------------------------------\n");
        //Imprimo el tablero y encuentro la posición del personaje, esta posición es en base al primer 0 que se encuentre
        for(int i = 0; i < tam_tablero; i++){
            for(int j = 0; j < tam_tablero; j++){
                printf("%s     ", tableroscopi[i][j]);
                if(strcmp(tableroscopi[i][j], "J₁") == 0 && !paso){
                    x = i;
                    y = j;
                    paso = true;
                }
            }
            printf("\n");
            printf("\n");
        }
        //Aqui se mueve el personaje
        printf("---------------------------------------------------------------------------------------------\n");
        printf(" Arriba : 1  |  Abajo : 2  |  Derecha : 3  |  Izquierda : 4  |  Usar Carta : 5  |  Salir : 6\n");
        printf("---------------------------------------------------------------------------------------------\n");
        printf("Tu posicion actual es: (%d,%d)\n", x, y);
        printf("-> ");
        scanf("%d", &seleccion);
        //getchar();  // Consumir el carácter de nueva línea en el búfer
        if(seleccion == 1){
            if(x - 1 >= 0 && strcmp(tableroscopi[x - 1][y], "0") == 0) {
                strcpy(tableroscopi[x][y], "0");
                strcpy(tableroscopi[x - 1][y], "J₁");
                x--;
            }else{
                printf("No se puede mover hacia Arriba\n");
            }
        }else if(seleccion == 2){
            if(x + 1 < tam_tablero && strcmp(tableroscopi[x + 1][y], "0") == 0){
                strcpy(tableroscopi[x][y], "0");
                strcpy(tableroscopi[x + 1][y], "J₁");
                x++;
            }else{
                printf("No se puede mover hacia Abajo\n");
            }
        }else if(seleccion == 3){
            if(y + 1 < tam_tablero && strcmp(tableroscopi[x][y + 1], "0") == 0){
                strcpy(tableroscopi[x][y], "0");
                strcpy(tableroscopi[x][y + 1], "J₁");
                y++;
            }else{
                printf("No se puede mover hacia Derecha\n");
            }
        }else if(seleccion == 4){
            if(y - 1 >= 0 && strcmp(tableroscopi[x][y - 1], "0") == 0){
                strcpy(tableroscopi[x][y], "0");
                strcpy(tableroscopi[x][y - 1], "J₁");
                y--;
            }else{
                printf("No se puede mover hacia Izquierda\n");
            }
        }else if(seleccion == 5){
            if(strcmp(info[0].Carta, "Buscar")==0) {
                //Falta implementar la conexion entre mapas y "B's"
            }else if(strcmp(info[0].Carta,"Escaleras")==0){
                while (flag){
                    if(x - 1 >= 0 && strcmp(tableroscopi[x - 1][y], "E") == 0){
                        printf("Salta una '%c'.\n", 'E');
                        strcpy(tableroscopi[x][y], "0");
                        strcpy(tableroscopi[x - 1][y], "J₁");
                        x--;
                        flag = false;
                    
                    }else if(x + 1 < tam_tablero && strcmp(tableroscopi[x+1][y], "E") == 0){  //Dirección sur (abajo)
                        printf("Salta una '%c'.\n", 'E');
                        strcpy(tableroscopi[x][y], "0");
                        strcpy(tableroscopi[x + 1][y], "J₁");
                        x++;
                        flag = false;
                    
                    }else if(y + 1 < tam_tablero && strcmp(tableroscopi[x][y + 1], "E") == 0){  //Dirección este (derecha)
                        printf("Salta una '%c'.\n", 'E');
                        strcpy(tableroscopi[x][y], "0");
                        strcpy(tableroscopi[x][y + 1], "J₁");
                        y++;       
                        flag = false;             
                    
                    }else if(y - 1 >= 0 && strcmp(tableroscopi[x][y - 1],"E") == 0){  //Dirección oeste (izquierda)
                        printf("Salta una '%c'.\n", 'E');
                        strcpy(tableroscopi[x][y], "0");
                        strcpy(tableroscopi[x][y - 1], "J₁");
                        y--;
                        flag = false;
                    }else{
                        printf("No se encontro Escaleras");
                        flag = false;
                    }
                }
            }
            else{
                printf("Error al asignar carta\n");
            }
        }else if(seleccion == 6){
            MostrarEnTableroBot("J₂", info[1], tableroscopi);
            MostrarEnTableroBot("J₃", info[2], tableroscopi);
            MostrarEnTableroBot("J₄", info[3], tableroscopi);
            //val = 0;
        }
    }

    printf("\n");
    printf("\n");
    printf("---------------------------------------------------------------------------------------------\n");
    printf("--------------------------------------------Test 2-------------------------------------------\n");
    printf("---------------------------------------------------------------------------------------------\n");
    for(int i = 0; i < tam_tablero; i++){
        for(int j = 0; j < tam_tablero; j++){
            printf("%s     ", tableroscopi[i][j]);
        }
        printf("\n");
        printf("\n");
    }

    //MostrarEnTableroBot("J₂", info, tableroscopi);

}



/*
La funcion inicializa los jugadores en el tablero

Parametros :
   char **matriz : puntero tipo char que apunta a la matriz que se va a mostrar
   int tam : entero que representa el tamaño de la matriz
   
Retorno :
   Nada, ya que es tipo void
 
*/
void Forks(struct Jugador* player){
    pid_t jugador_pid[4];
    int pid_1 = 0;

    //Código para el proceso padre
    for(int i = 0; i < 4; i++){
        // Espera un breve período de tiempo antes de crear el próximo proceso hijo
        if (i > 0) {
            sleep(50);  //Ajusta el tiempo de espera según tus necesidades, se dejo en 50 para que tenga tiempo para elegir la carta o el movimiento
        }

        jugador_pid[i] = fork();
        if(jugador_pid[i] == -1){
            perror("fork");
            exit(EXIT_FAILURE);
        }else if(jugador_pid[i] == 0){
            //Código para el proceso hijo (Jugadores 1, 2, 3, 4)
            srand(time(NULL) + getpid());
            player[i].pid = getpid();
            for (int j = 0; j <= 15; j++){
                //player[i].pid = jugador_pid[i];
                if(i == 0){
                    MostrarEnTablero(player, Tableros[0]);
                }else if(i == 1){
                    MostrarEnTableroBot("J₂", player[i], Tableros[0]);
                }else if(i == 2){
                    MostrarEnTableroBot("J₃", player[i], Tableros[0]);
                }else if(i == 3){
                    MostrarEnTableroBot("J₄", player[i], Tableros[0]);
                }else{
                    exit(0);
                }
                player[i].Turno_actual++;
                sleep(50);
            }
            //Salir del bucle del proceso hijo
            exit(0);
        }
    }

    
    //Código para el proceso padre
    for(int i = 0; i < 4; i++){
        //Espera a que cada proceso hijo termine
        waitpid(jugador_pid[i], NULL, 0);
        //player[i].pid = jugador_pid[i];
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
    Tableros[glob] = malloc(sizeof(char *) * (longitud / 2));

    if(strcmp(nom_archivo, "Inicio.txt") == 0){
        //printf("Archivo abierto correctamente\n");

        char ***matriz = malloc(sizeof(char **) * 5);
        // Asignar memoria para la matriz y las cadenas
        for (int i = 0; i < 5; i++) {
            matriz[i] = malloc(sizeof(char *) * 5);
            for (int j = 0; j < 5; j++) {
                matriz[i][j] = malloc(sizeof(char) * 3); //Suficiente espacio para "J₁", "J₂", "J₃", "J₄"
            }
        }
        for(int i = 0; i < 5; i++){
            for(int j = 0; j < 5; j++){
                char juga;
                if(fscanf(archivolectura, " %c", &juga) == 1){
                    if(juga == 'J') {
                        char num;
                        if (fscanf(archivolectura, "%c", &num) == 1){
                            if (num == '1') {
                                strcpy(matriz[i][j], "J₁");
                            }else if (num == '2') {
                                strcpy(matriz[i][j], "J₂");
                            }else if (num == '3') {
                                strcpy(matriz[i][j], "J₃");
                            }else if (num == '4') {
                                strcpy(matriz[i][j], "J₄");
                            }
                        }
                    }else{
                        matriz[i][j][0] = juga;
                        matriz[i][j][1] = '\0';
                    }
                }
            }
        }
        //Imprimir la matriz con superíndices
        for (int i = 0; i < 5; i++){
            Tableros[glob][i] = malloc(sizeof(char *) * (longitud / 2));
            for (int j = 0; j < 5; j++){
                Tableros[glob][i][j] = malloc(sizeof(char) * 3); //Revisar
                Tableros[glob][i][j] = matriz[i][j];
                //printf("%s     ", matriz[i][j]);
            }
            //printf("\n");
        }
    }else{
        for(int i = 0; i < longitud / 2; i++){
            Tableros[glob][i] = malloc(sizeof(char *) * (longitud / 2));
            for(int j = 0; j < longitud / 2; j++) {
                Tableros[glob][i][j] = malloc(sizeof(char) * 3); //Revisar
                int numAleat = rand() % 100;
                int numAleat2 = rand() % 3;
                char variable;
                if(fscanf(archivolectura, "%c ", &variable) == 1){
                    char valor = Eventos(variable);
                    Tableros[glob][i][j][0] = valor;
                    Tableros[glob][i][j][1] = '\0'; // Agregar el carácter nulo al final para formar una cadena válida
                    //printf("%c     ", valor);
                }else{
                    // Handle end of file or other read errors
                    break;
                }
            }
            //printf("\n");
        }
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



void shuffleWithFixedFirstElement(char **arr, int n) {
    srand(time(NULL));
    
    // Encuentra la posición del elemento "archivo.txt" en el arreglo
    int positionArchivoTxt = -1;
    for (int i = 0; i < n; i++) {
        if (strcmp(arr[i], "Inicio.txt") == 0) {
            positionArchivoTxt = i;
            break;
        }
    }
    
    if (positionArchivoTxt == -1) {
        // "archivo.txt" no se encontró en el arreglo, no se puede garantizar que esté en la primera posición
        shuffle(arr, n); // Simplemente desordena el arreglo sin garantizar la posición
        return;
    }
    
    // Intercambia "archivo.txt" con el primer elemento (si no está ya en la primera posición)
    if (positionArchivoTxt != 0) {
        swap(&arr[0], &arr[positionArchivoTxt]);
    }
    
    // Luego, desordena el resto del arreglo (sin incluir "archivo.txt")
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % i + 1; // Comienza desde la segunda posición
        swap(&arr[i], &arr[j]);
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
    printf("=============================================================================================\n");
    printf("                                           Magic Maze                                        \n");
    printf("=============================================================================================\n");
    

    //Aqui se guardan todos los tableros
    Tableros = malloc(sizeof(char ***) * 9);

    // Luego, para cada elemento del array, asigna una matriz como lo hiciste antes
    for (int i = 0; i < 9; i++) {
        Tableros[i] = malloc(sizeof(char **) * 5);
        for (int j = 0; j < 5; j++) {
            Tableros[i][j] = malloc(sizeof(char *) * 5);
            for (int k = 0; k < 5; k++) {
                Tableros[i][j][k] = malloc(sizeof(char) * 3); // Suficiente espacio para "J₁", "J₂", "J₃", "J₄" + '\0'
            }
        }
    }
    

    //Aqui estan todos los jugadores
    struct Jugador* jugadores = (struct Jugador*)malloc(sizeof(struct Jugador) * 4);

    //Se crean las cartas
    char *Cartas[num_cartas] = {"Buscar-Arriba", "Escaleras-Abajo", "Escaleras-Izquierda", "Buscar-Derecha"};

    bool cartas_asignadas[num_cartas];
    for(int i = 0; i < num_cartas; i++){
        cartas_asignadas[i] = false;
    }

    // Inicializar generador de números aleatorios
    srand(time(NULL));

    printf("-> Revolviendo el mazo de cartas y los tableros...\n");
    shuffle(Cartas, num_cartas);
    

    for(int i = 0; i < 4; i++){
        int numero;
        do{
            numero = rand() % num_cartas;
        }while(cartas_asignadas[numero]);
        jugadores[i].Carta = Cartas[numero];
        cartas_asignadas[numero] = true;
        //printf("La carta del Jugador %d es: %s\n", i + 1, jugadores[i].Carta);
    }
    printf("-> Tu carta es: %s\n", jugadores[0].Carta);
    printf("\n");

    
    char **obj = LeerDir();
    //shuffle(obj, num_tableros);
    shuffleWithFixedFirstElement(obj, num_tableros);
    
    for(int i = 0; i < num_tableros; i++){
        //printf("%s\n", obj[i]);
        char *nom_tabl = obj[i];
        CrearTablero(nom_tabl);
        //printf("\n");
    }

    int Turno;
    printf("---------------------------------------------------------------------------------------------\n");
    printf("                               Presione 1 para nueva partida:                                \n");
    printf("---------------------------------------------------------------------------------------------\n");
    printf("--> ");
    scanf("%d", &Turno);
    if(Turno == 1){
        //Se crean los Jugadores
        Forks(jugadores);
    }else{
        //Se sale del programa
        exit(0);
    }
    
    /*
    for(int i = 0; i < 4; i++){
        printf("-----------------Informacion Jugador %d-----------------\n", i + 1);
        printf("PID: %d \n",jugadores[i].pid);
        printf("Carta: %s \n",jugadores[i].Carta);
        printf("Turno Actual: %d \n",jugadores[i].Turno_actual);
        printf("Tesoros Encontrados: %d \n",jugadores[i].Tesoros_encontrados);
        printf("--------------------------------------------------------\n");
    }
    */


    return 0;
}
