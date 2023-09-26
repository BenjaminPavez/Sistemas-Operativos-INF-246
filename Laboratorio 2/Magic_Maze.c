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
int Turno = 15;
int pos_tablero = 0;



//Estructura de Jugador
struct Jugador{
   int pid;
   char* Carta;
   int Rondas;
   int Tesoros_encontrados;
   int prox_x;
   int prox_y;
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



void CambioTablero(char*, struct Jugador*, int, int); //Prototipo de la funcion CambioTablero, mas abajo esta la implementacion



/*
La funcion cambiar los valores de la matriz por los eventos

Parametros :
   char* val : puntero tipo char que representa un valor de la matriz
   
Retorno :
   Retorna un puntero tipo char que repreesenta el valor modificado al entregado
 
*/
void CasillasEspeciales(char* jugador, char* casilla, struct Jugador info) {
    system("clear");
    printf("---------------------------------------------------------------------------------------------\n");
    printf("                                %s esta en una casilla especial                              \n", jugador);
    printf("---------------------------------------------------------------------------------------------\n");
    printf("      PID: %d          Tu Carta: %s           Tus Tesoros: %d        Turno Actual: %d        \n", info.pid, info.Carta, info.Tesoros_encontrados, Turno);
    printf("---------------------------------------------------------------------------------------------\n");
    for(int i = 0; i < tam_tablero; i++){
        for(int j = 0; j < tam_tablero; j++){
            printf("%s     ", Tableros[0][i][j]);
        }
        printf("\n");
        printf("\n");
    }
    if (strcmp(casilla, "t") == 0) {
        printf("---------------------------------------------------------------------------------------------\n");
        printf("                                      Se suman 4 rondas :)                                   \n");
        printf("---------------------------------------------------------------------------------------------\n");
        Turno += 4; //Añadir 4 rondas
    } else if (strcmp(casilla, "n") == 0) {
        printf("---------------------------------------------------------------------------------------------\n");
        printf("                                      Se restan 3 rondas :(                                  \n");
        printf("---------------------------------------------------------------------------------------------\n");
        Turno -= 3; //Restar 3 rondas
    }

    printf("---------------------------------------------------------------------------------------------\n");
    printf("                                    Regresando al Tablero...                                 \n");
    printf("---------------------------------------------------------------------------------------------\n");
    sleep(5);

}



/*
La funcion realiza movimiento del Bot en el tablero

Parametros :
   char *jugador : puntero tipo char que apunta al jugador, este puede ser J₂, J₃ o J₄
   struct Jugador* info : estructura tipo Jugador que contiene la informacion del jugador
   char*** tableroscopi : puntero tipo char que apunta a la matriz que se va a mostrar
   
Retorno :
   Nada, ya que es tipo void
 
*/
void MostrarEnTableroBot(char *jugador, struct Jugador* info, char*** tableroscopi){
    int num_jugador;
    if(strcmp(jugador, "J₁") == 0){
        num_jugador = 0;
    }else if(strcmp(jugador, "J₂") == 0){
        num_jugador = 1;
    }else if(strcmp(jugador, "J₃") == 0){
        num_jugador = 2;
    }else if(strcmp(jugador, "J₄") == 0){
        num_jugador = 3;
    }

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
    printf("    PID: %d     Tu Carta: %s     Tus Tesoros: %d     Turno Actual: %d     Ronda: %d      \n", info[num_jugador].pid, info[num_jugador].Carta, info[num_jugador].Tesoros_encontrados, Turno, info[num_jugador].Rondas);
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
    printf("Tu posicion actual es: (%d,%d)\n", y, x);
    printf("-> ");
    scanf("%d", &seleccion);
    if(seleccion == 1){
        if(x - 1 >= 0 && (strcmp(tableroscopi[x - 1][y], "/") != 0 && strcmp(tableroscopi[x - 1][y], "E") != 0 && strcmp(tableroscopi[x - 1][y], "J₁") != 0 && strcmp(tableroscopi[x - 1][y], "J₂") != 0 && strcmp(tableroscopi[x - 1][y], "J₃") != 0 && strcmp(tableroscopi[x - 1][y], "J₄") != 0)){
            if(strcmp(tableroscopi[x - 1][y], "t") == 0 || strcmp(tableroscopi[x - 1][y], "n") == 0){
                CasillasEspeciales(jugador, tableroscopi[x - 1][y], info[num_jugador]);
                strcpy(tableroscopi[x][y], "0");
                strcpy(tableroscopi[x - 1][y], jugador);
                x--;
            }else{
                strcpy(tableroscopi[x][y], "0");
                strcpy(tableroscopi[x - 1][y], jugador);
                x--;
            }
        }else{
            printf("No se puede mover hacia Arriba\n");
        }
        info[num_jugador].prox_x = x; //Modifico la direccion en x del jugador
        info[num_jugador].prox_y = y; //Modifico la direccion en y del jugador
    }else if(seleccion == 2){
        if(x + 1 < tam_tablero && (strcmp(tableroscopi[x + 1][y], "/") != 0 && strcmp(tableroscopi[x + 1][y], "E") != 0 && strcmp(tableroscopi[x + 1][y], "J₁") !=0  && strcmp(tableroscopi[x + 1][y], "J₂") != 0 && strcmp(tableroscopi[x + 1][y], "J₃") != 0 && strcmp(tableroscopi[x + 1][y], "J₄") != 0)){
            if(strcmp(tableroscopi[x + 1][y], "t") == 0 || strcmp(tableroscopi[x + 1][y], "n") == 0){
                CasillasEspeciales(jugador, tableroscopi[x + 1][y], info[num_jugador]);
                strcpy(tableroscopi[x][y], "0");
                strcpy(tableroscopi[x + 1][y], jugador);
                x++;
            }else{
                strcpy(tableroscopi[x][y], "0");
                strcpy(tableroscopi[x + 1][y], jugador);
                x++;
            }
        }else{
            printf("No se puede mover hacia Abajo\n");
        }
        info[num_jugador].prox_x = x; //Modifico la direccion en x del jugador
        info[num_jugador].prox_y = y; //Modifico la direccion en y del jugador
    }else if(seleccion == 3){
        if(y + 1 < tam_tablero && (strcmp(tableroscopi[x][y + 1], "/") != 0 && strcmp(tableroscopi[x][y + 1], "E") != 0 && strcmp(tableroscopi[x][y + 1], "J₁") !=0 && strcmp(tableroscopi[x][y + 1], "J₂") != 0 && strcmp(tableroscopi[x][y + 1], "J₃") != 0&& strcmp(tableroscopi[x][y + 1], "J₄") != 0)){
            if(strcmp(tableroscopi[x][y + 1], "t") == 0 || strcmp(tableroscopi[x][y + 1], "n") == 0){
                CasillasEspeciales(jugador, tableroscopi[x][y + 1], info[num_jugador]);
                strcpy(tableroscopi[x][y], "0");
                strcpy(tableroscopi[x][y + 1], jugador);
                y++;
            }else{
                strcpy(tableroscopi[x][y], "0");
                strcpy(tableroscopi[x][y + 1], jugador);
                y++;
            }
        }else{
            printf("No se puede mover hacia Derecha\n");
        }
        info[num_jugador].prox_x = x; //Modifico la direccion en x del jugador
        info[num_jugador].prox_y = y; //Modifico la direccion en y del jugador
    }else if(seleccion == 4){
        if(y - 1 < tam_tablero && (strcmp(tableroscopi[x][y - 1], "/") != 0 && strcmp(tableroscopi[x][y - 1], "E") != 0 && strcmp(tableroscopi[x][y - 1], "J₁") !=0 && strcmp(tableroscopi[x][y - 1], "J₂") != 0 && strcmp(tableroscopi[x][y - 1], "J₃") != 0&& strcmp(tableroscopi[x][y - 1], "J₄") != 0)){
            if(strcmp(tableroscopi[x][y - 1], "t") == 0 || strcmp(tableroscopi[x][y - 1], "n") == 0){
                CasillasEspeciales(jugador, tableroscopi[x][y - 1], info[num_jugador]);
                strcpy(tableroscopi[x][y], "0");
                strcpy(tableroscopi[x][y - 1], jugador);
                y--;
            }else{
                strcpy(tableroscopi[x][y], "0");
                strcpy(tableroscopi[x][y - 1], jugador);
                y--;
            }
        }else{
            printf("No se puede mover hacia Izquierda\n");
        }
        info[num_jugador].prox_x = x; //Modifico la direccion en x del jugadorJ₃
        info[num_jugador].prox_y = y; //Modifico la direccion en y del jugador
    }else if(seleccion == 5){
        if(strcmp(info[num_jugador].Carta, "Buscar") == 0){
            CambioTablero(jugador, info, y, x); //Le mandamos la info a la funcion que calcula y realiza el cambio de tablero
        }else if(strcmp(info[num_jugador].Carta,"Escaleras")==0){
            while(flag){
                if(x - 1 >= 0 && strcmp(tableroscopi[x - 1][y],"E")==0){ //arriba
                    printf("Salta una '%c'.\n", 'E');
                    strcpy(tableroscopi[x][y], "0");
                    strcpy(tableroscopi[x - 1][y], jugador);
                    x--;
                    flag = false;

                }else if (x + 1 < tam_tablero && strcmp(tableroscopi[x+1][y], "E")==0){ //abajo
                    printf("Salta una '%c'.\n", 'E');
                    strcpy(tableroscopi[x][y], "0");
                    strcpy(tableroscopi[x + 1][y], jugador);
                    x++;
                    flag = false;
                
                }else if (y + 1 < tam_tablero && strcmp(tableroscopi[x][y + 1],"E")==0){ //derecha
                    printf("Salta una '%c'.\n", 'E');
                    strcpy(tableroscopi[x][y], "0");
                    strcpy(tableroscopi[x][y + 1], jugador);
                    y++;       
                    flag = false;             
                
                }else if (y - 1 >= 0 && strcmp(tableroscopi[x][y - 1],"E")==0){ //izquierda
                    printf("Salta una '%c'.\n", 'E');
                    strcpy(tableroscopi[x][y], "0");
                    strcpy(tableroscopi[x][y - 1], jugador);
                    y--;
                    flag = false;
                
                }else{
                    printf("No se encontro Escaleras\n");
                    flag = false;
                }
            }
        }
        else{
            printf("Error al usar carta\n");
        }
        info[num_jugador].prox_x = x; //Modifico la direccion en x del jugador
        info[num_jugador].prox_y = y; //Modifico la direccion en y del jugador
    }else if(seleccion == 6){
        val = 0;
    }
    info[num_jugador].Rondas++;


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

}



/*
La funcion realiza movimiento de nosotros en el tablero

Parametros :
   struct Jugador* info : estructura tipo Jugador que contiene la informacion del jugador
   char*** tableroscopi : puntero tipo char que apunta a la matriz que se va a mostrar
   
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
        printf("    PID: %d     Tu Carta: %s     Tus Tesoros: %d     Turno Actual: %d     Ronda: %d      \n", info[0].pid, info[0].Carta, info[0].Tesoros_encontrados, Turno, info[0].Rondas);
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
        printf("Tu posicion actual es: (%d,%d)\n", y, x);
        printf("-> ");
        scanf("%d", &seleccion);
        if(seleccion == 1){
            if(x - 1 >= 0 && (strcmp(tableroscopi[x - 1][y], "/") != 0 && strcmp(tableroscopi[x - 1][y], "E") != 0 && strcmp(tableroscopi[x - 1][y], "J₂") != 0 && strcmp(tableroscopi[x - 1][y], "J₃") != 0 && strcmp(tableroscopi[x - 1][y], "J₄") != 0)){
                if(strcmp(tableroscopi[x - 1][y], "t") == 0 || strcmp(tableroscopi[x - 1][y], "n") == 0){
                    CasillasEspeciales("J₁", tableroscopi[x - 1][y], info[0]);
                    strcpy(tableroscopi[x][y], "0");
                    strcpy(tableroscopi[x - 1][y], "J₁");
                    x--;
                }else{
                    strcpy(tableroscopi[x][y], "0");
                    strcpy(tableroscopi[x - 1][y], "J₁");
                    x--;
                }
            }else{
                printf("No se puede mover hacia Arriba\n");
            }
            info[0].prox_x = x; //Modifico la direccion en x del jugador
            info[0].prox_y = y; //Modifico la direccion en y del jugador
        }else if(seleccion == 2){
            if(x + 1 < tam_tablero && (strcmp(tableroscopi[x + 1][y], "/") != 0 && strcmp(tableroscopi[x + 1][y], "E") != 0 && strcmp(tableroscopi[x + 1][y], "J₂") != 0 && strcmp(tableroscopi[x + 1][y], "J₃") != 0 && strcmp(tableroscopi[x + 1][y], "J₄") != 0)){
                if(strcmp(tableroscopi[x + 1][y], "t") == 0 || strcmp(tableroscopi[x + 1][y], "n") == 0){
                    CasillasEspeciales("J₁", tableroscopi[x + 1][y], info[0]);
                    strcpy(tableroscopi[x][y], "0");
                    strcpy(tableroscopi[x + 1][y], "J₁");
                    x++;
                }else{
                    strcpy(tableroscopi[x][y], "0");
                    strcpy(tableroscopi[x + 1][y], "J₁");
                    x++;
                }
            }else{
                printf("No se puede mover hacia Abajo\n");
            }
            info[0].prox_x = x; //Modifico la direccion en x del jugador
            info[0].prox_y = y; //Modifico la direccion en y del jugador
        }else if(seleccion == 3){
            if(y + 1 < tam_tablero && (strcmp(tableroscopi[x][y + 1], "/") != 0 && strcmp(tableroscopi[x][y + 1], "E") != 0 && strcmp(tableroscopi[x][y + 1], "J₂") != 0 && strcmp(tableroscopi[x][y + 1], "J₃") != 0&& strcmp(tableroscopi[x][y + 1], "J₄") != 0)){
                if(strcmp(tableroscopi[x][y + 1], "t") == 0 || strcmp(tableroscopi[x][y + 1], "n") == 0){
                    CasillasEspeciales("J₁", tableroscopi[x][y + 1], info[0]);
                    strcpy(tableroscopi[x][y], "0");
                    strcpy(tableroscopi[x][y + 1], "J₁");
                    y++;
                }else{
                    strcpy(tableroscopi[x][y], "0");
                    strcpy(tableroscopi[x][y + 1], "J₁");
                    y++;
                }
            }else{
                printf("No se puede mover hacia Derecha\n");
            }
            info[0].prox_x = x; //Modifico la direccion en x del jugador
            info[0].prox_y = y; //Modifico la direccion en y del jugador
        }else if(seleccion == 4){
            if(y - 1 < tam_tablero && (strcmp(tableroscopi[x][y - 1], "/") != 0 && strcmp(tableroscopi[x][y - 1], "E") != 0 && strcmp(tableroscopi[x][y - 1], "J₂") != 0 && strcmp(tableroscopi[x][y - 1], "J₃") != 0&& strcmp(tableroscopi[x][y - 1], "J₄") != 0)){
                if(strcmp(tableroscopi[x][y - 1], "t") == 0 || strcmp(tableroscopi[x][y - 1], "n") == 0){
                    CasillasEspeciales("J₁", tableroscopi[x][y - 1], info[0]);
                    strcpy(tableroscopi[x][y], "0");
                    strcpy(tableroscopi[x][y - 1], "J₁");
                    y--;
                }else{
                    strcpy(tableroscopi[x][y], "0");
                    strcpy(tableroscopi[x][y - 1], "J₁");
                    y--;
                }
            }else{
                printf("No se puede mover hacia Izquierda\n");
            }
            info[0].prox_x = x; //Modifico la direccion en x del jugador
            info[0].prox_y = y; //Modifico la direccion en y del jugador
        }else if(seleccion == 5){
            if(strcmp(info[0].Carta, "Buscar") == 0){
                CambioTablero("J₁", info, y, x); //Le mandamos la info a la funcion que calcula y realiza el cambio de tablero
            }else if(strcmp(info[0].Carta,"Escaleras")==0){
                while(flag){
                    if(x - 1 >= 0 && strcmp(tableroscopi[x - 1][y], "E") == 0){ //arriba
                        printf("Salta una '%c'.\n", 'E');
                        strcpy(tableroscopi[x][y], "0");
                        strcpy(tableroscopi[x - 1][y], "J₁");
                        x--;
                        flag = false;
                    
                    }else if(x + 1 < tam_tablero && strcmp(tableroscopi[x + 1][y], "E") == 0){  //abajo
                        printf("Salta una '%c'.\n", 'E');
                        strcpy(tableroscopi[x][y], "0");
                        strcpy(tableroscopi[x + 1][y], "J₁");
                        x++;
                        flag = false;
                    
                    }else if(y + 1 < tam_tablero && strcmp(tableroscopi[x][y + 1], "E") == 0){  //derecha
                        printf("Salta una '%c'.\n", 'E');
                        strcpy(tableroscopi[x][y], "0");
                        strcpy(tableroscopi[x][y + 1], "J₁");
                        y++;       
                        flag = false;             
                    
                    }else if(y - 1 >= 0 && strcmp(tableroscopi[x][y - 1],"E") == 0){  //izquierda
                        printf("Salta una '%c'.\n", 'E');
                        strcpy(tableroscopi[x][y], "0");
                        strcpy(tableroscopi[x][y - 1], "J₁");
                        y--;
                        flag = false;
                    }else{
                        printf("No se encontro Escaleras\n");
                        flag = false;
                    }
                }
            }else{
                printf("Error al asignar carta\n");
            }
            info[0].prox_x = x; //Modifico la direccion en x del jugador
            info[0].prox_y = y; //Modifico la direccion en y del jugador
        }else if(seleccion == 6){
            MostrarEnTableroBot("J₂", info, tableroscopi);
            MostrarEnTableroBot("J₃", info, tableroscopi);
            MostrarEnTableroBot("J₄", info, tableroscopi);
            Turno--;
            //val = 0;
        }
        info[0].Rondas++;
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

}



/*
La funcion cambia de tablero a los jugadores

Parametros :
   char *jugador : puntero tipo char que apunta al jugador, este puede ser J₁, J₂, J₃ o J₄
   struct Jugador* player : puntero tipo Jugador que contiene la informacion de todos los jugadores
   int x : entero que representa la posicion en el eje x del jugador
   int y : entero que representa la posicion en el eje y del jugador
   
Retorno :
   Nada, ya que es tipo void
 
*/
void CambioTablero(char *jugador, struct Jugador* player, int x, int y){
    printf("\n");
    printf("\n");
    printf("---------------------------------------------------------------------------------------------\n");
    printf("                                        Cambio de Tablero                                    \n");
    printf("---------------------------------------------------------------------------------------------\n");
    //Vemos la posicion del jugador para sacar su numero del array de struct
    int num_jugador;
    if(strcmp(jugador, "J₁") == 0){
        num_jugador = 0;
    }else if(strcmp(jugador, "J₂") == 0){
        num_jugador = 1;
    }else if(strcmp(jugador, "J₃") == 0){
        num_jugador = 2;
    }else if(strcmp(jugador, "J₄") == 0){
        num_jugador = 3;
    }

    //Vemos donde puede estar la posible B en los otros tableros
    if(y == tam_tablero - 1){
        y = 0;
    }else if(y == 0){
        y = tam_tablero - 1;
    }

    if(x == tam_tablero - 1){
        x = 0;
    }else if(x == 0){
        x = tam_tablero - 1;
    }

    int pos;
    //Con las nuevas coordenadas busca en los otros tableros si hay una B en tal posicion
    for(int k = pos_tablero+1; k < num_tableros; k++){
        printf("Buscando en el tablero %d\n", k);
        if(strcmp(Tableros[k][y][x], "B") == 0){
            player[num_jugador].prox_x = x; //Modifico la direccion en x del jugador
            player[num_jugador].prox_y = y; //Modifico la direccion en y del jugador
            strcpy(Tableros[k][y][x], jugador); //Traspaso el jugador al nuevo tablero
            printf("\n");
            printf("\n");
            printf("---------------------------------------------------------------------------------------------\n");
            printf("                                        Salto de Tablero                                     \n");
            printf("---------------------------------------------------------------------------------------------\n");
            for(int i = 0; i < tam_tablero; i++){
                for(int j = 0; j < tam_tablero; j++){
                    printf("%s     ", Tableros[k][i][j]);
                }
                printf("\n");
                printf("\n");
            }
            pos = k;
            break; //Si encuentra una B se sale del for
            
        }
    }
    MostrarEnTablero(player, Tableros[pos]);
}



/*
La función envía la estructura de jugador a través de una tubería.

Parámetros:
    int fd : descriptor de archivo de la tubería para escribir.
    struct Jugador* player : puntero a la estructura de Jugador a enviar.

Retorno:
    Nada, ya que es tipo void.
*/
void EnviarJugador(int fd, struct Jugador* player){
    // Escribe la estructura del jugador en la tubería
    write(fd, player, sizeof(struct Jugador));
}



/*
La función recibe la estructura de jugador desde una tubería.

Parámetros:
    int fd : descriptor de archivo de la tubería para leer.
    struct Jugador* player : puntero a la estructura de Jugador donde se almacenará la información recibida.

Retorno:
    Nada, ya que es tipo void.
*/
void RecibirJugador(int fd, struct Jugador* player) {
    // Lee la estructura del jugador desde la tubería
    read(fd, player, sizeof(struct Jugador));
}



/*
La funcion inicializa los jugadores en el tablero

Parametros :
   struct Jugador* player : puntero tipo Jugador que contiene la informacion de todos los jugadores
   
Retorno :
   Nada, ya que es tipo void
 
*/
void Forks(struct Jugador* player){
    pid_t jugador_pid[4];
    int pipes[4][2]; //Un array de tuberías para comunicarse con los procesos hijos

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
                    MostrarEnTablero(player, Tableros[pos_tablero]);  //REVISARRRRR, se cambio el 0 por el pos_tablero
                    EnviarJugador(pipes[j][1], &player[1]);
                    EnviarJugador(pipes[j][1], &player[2]);
                    EnviarJugador(pipes[j][1], &player[3]);
                }else if(i >= 1){
                    RecibirJugador(pipes[i][0], &player[i]);
                }else{
                    exit(0);
                }
                player[i].Rondas--; //REVISAR
                pos_tablero++; //REVISAR
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
        player[i].pid = jugador_pid[i];
    }  
}



//NO ESTA IMPLEMENTADA
/*
La funcion cambiar los valores de la matriz por los tesoros

Parametros :
   char* val : puntero tipo char que representa un valor de la matriz
   
Retorno :
   Retorna un puntero tipo char que repreesenta el valor modificado al entregado
 
*/
char *TesorosWithString(char *val){
    int numAleat2 = rand() % 3;;
    if (strcmp(val, "0") == 0) {
        char *var1 = malloc(2 * sizeof(char));  //Asigna memoria para un string de 2 caracteres
        if(true){
            if (numAleat2 == 0) {
                strcpy(var1, "c");
            } else if (numAleat2 == 1) {
                strcpy(var1, "t");
            } else if (numAleat2 == 2) {
                strcpy(var1, "n");
            } else if (numAleat2 == 3) {
                strcpy(var1, "p");
            }
            return var1;
        }else{
            free(var1);  //Libera la memoria si no se usa
            return val;
        }
    } else {
        return val;
    }

}



/*
La funcion cambiar los valores de la matriz por los eventos

Parametros :
   char* val : puntero tipo char que representa un valor de la matriz
   
Retorno :
   Retorna un puntero tipo char que repreesenta el valor modificado al entregado
 
*/
char *EventosWithString(char *val){
    int numAleat2 = rand() % 3;;
    if (strcmp(val, "0") == 0) {
        char *var1 = malloc(2 * sizeof(char));  //Asigna memoria para un string de 2 caracteres
        if(true){
            if (numAleat2 == 0) {
                strcpy(var1, "c");
            } else if (numAleat2 == 1) {
                strcpy(var1, "t");
            } else if (numAleat2 == 2) {
                strcpy(var1, "n");
            } else if (numAleat2 == 3) {
                strcpy(var1, "p");
            }
            return var1;
        }else{
            free(var1);  //Libera la memoria si no se usa
            return val;
        }
    } else {
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
        //Asignar memoria para la matriz y las cadenas
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
                            if(num == '1'){
                                strcpy(matriz[i][j], "J₁");
                            }else if(num == '2'){
                                strcpy(matriz[i][j], "J₂");
                            }else if(num == '3'){
                                strcpy(matriz[i][j], "J₃");
                            }else if(num == '4'){
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
                Tableros[glob][i][j] = malloc(sizeof(char) * 3); //REVISADO
                char *valor = EventosWithString(matriz[i][j]);
                strcpy(Tableros[glob][i][j], valor);
            }
        }
    }else{
        for(int i = 0; i < longitud / 2; i++){
            Tableros[glob][i] = malloc(sizeof(char *) * (longitud / 2));
            for(int j = 0; j < longitud / 2; j++) {
                Tableros[glob][i][j] = malloc(sizeof(char) * 3); //REVISADO
                char *variable;
                if(fscanf(archivolectura, "%s ", variable) == 1){
                    char *valor = EventosWithString(variable);
                    strcpy(Tableros[glob][i][j], valor);
                }else{
                    //Handle end of file or other read errors
                    break;
                }
            }
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
        swap(&arr[i], &arr[j]); //Usa la función swap corregida
    }
}



/*
La funcion desordena una lista pero pone al inicio obligatoriamente al archivo Inicio.txt

Parametros :
   char **arr : puntero tipo char que apunta a una cadena de cadenas de char
   int n : entego que representa el tamaño de la lista

Retorno :
   Nada, ya que es tipo void
 
*/
void shuffleWithFixedFirstElement(char **arr, int n) {
    srand(time(NULL));
    //Encuentra la posición del elemento "archivo.txt" en el arreglo
    int positionArchivoTxt = -1;
    for (int i = 0; i < n; i++) {
        if (strcmp(arr[i], "Inicio.txt") == 0) {
            positionArchivoTxt = i;
            break;
        }
    }
    
    if (positionArchivoTxt == -1) {
        //"Inicio.txt" no se encontró en el arreglo, no se puede garantizar que esté en la primera posición
        shuffle(arr, n); //Simplemente desordena el arreglo sin garantizar la posición
        return;
    }
    
    //Intercambia "Inicio.txt" con el primer elemento (si no está ya en la primera posición)
    if (positionArchivoTxt != 0) {
        swap(&arr[0], &arr[positionArchivoTxt]);
    }
    
    //Luego, desordena el resto del arreglo (sin incluir "Inicio.txt")
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
                Tableros[i][j][k] = malloc(sizeof(char) * 3); //Suficiente espacio para "J₁", "J₂", "J₃", "J₄" + '\0'
            }
        }
    }
    
    //Aqui estan todos los jugadores
    struct Jugador* jugadores = (struct Jugador*)malloc(sizeof(struct Jugador) * 4);

    //Se crean las cartas
    char *Cartas[num_cartas] = {"Buscar", "Escaleras", "Escaleras", "Buscar"};

    bool cartas_asignadas[num_cartas];
    for(int i = 0; i < num_cartas; i++){
        cartas_asignadas[i] = false;
    }

    //Inicializar generador de números aleatorios
    srand(time(NULL));

    printf("-> Revolviendo el mazo de cartas y los tableros...\n");
    printf("\n");
    shuffle(Cartas, num_cartas);
    
    for(int i = 0; i < 4; i++){
        int numero;
        do{
            numero = rand() % num_cartas;
        }while(cartas_asignadas[numero]);
        jugadores[i].Carta = Cartas[numero];
        jugadores[i].Rondas = 1; //REVISAR
        cartas_asignadas[numero] = true;
    }
    printf("-> Tu carta es: %s\n", jugadores[0].Carta);
    printf("\n");

    
    char **obj = LeerDir();
    shuffleWithFixedFirstElement(obj, num_tableros);
    
    for(int i = 0; i < num_tableros; i++){
        char *nom_tabl = obj[i];
        CrearTablero(nom_tabl);
    }

    int nueva_partida;
    printf("---------------------------------------------------------------------------------------------\n");
    printf("                               Presione 1 para nueva partida:                                \n");
    printf("---------------------------------------------------------------------------------------------\n");
    printf("--> ");
    scanf("%d", &nueva_partida);
    if(nueva_partida == 1){
        //Se crean los Jugadores
        Forks(jugadores);
    }else{
        //Se sale del programa
        exit(0);
    }
    

    return 0;
}
