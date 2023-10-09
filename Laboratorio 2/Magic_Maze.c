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



//Constantes
#define num_cartas 4
#define num_tableros 9
#define tam_tablero 5
#define tam_tablero_fondo 50



//Colores del fondo
#define BG_BLUE "\x1B[44m"
#define BLACK "\x1B[40m"
#define RESET "\x1B[0m"



//Se definen variables globales
char**** Tableros;
int glob = 0;
int Turno = 15;
int pos_tablero = 0;
bool tableros_vistos[num_tableros];



//Coordenada en el tablero del fondo
int X = 20;
int Y = 20;



/*
Estructura de Jugador

Miembros :
   char *color : puntero tipo char que apunta al color de fondo
   char character[3] : array de char que representa el carácter a mostrar en la casilla
   
*/
typedef struct {
    char *color;
    char character[3];
} Cell;

Cell ***fondo; //Declaración de fondo



/*
Estructura de Jugador

Miembros :
   int pid : entero que representa el PID del proceso
   char* Carta : puntero tipo char que apunta al nombre de la carta del jugador
   int Rondas : entero que representa las rondas que lleva el jugador
   int Tesoros_encontrados : entero que representa los tesoros que encontro el jugador
   int prox_x : entero que representa la posicion el jugador en la coordenada x
   int prox_y : entero que representa la posicion el jugador en la coordenada y
   int tabl : entero que representa el tablero donde esta el jugador
   
*/
struct Jugador{
   int pid;
   char* Carta;
   int Rondas;
   int Tesoros_encontrados;
   int prox_x;
   int prox_y;
   int tabl;
   int tab_X;
   int tab_Y;
};

struct Jugador* jugadores; //Declaracion de los jugadores



/*
IMPORTANTE:

LOS JUGADORES ESTAN REPRESENTADOS POR NUMEROS DEL J₁ AL J₄

*/



/*
La funcion imprime un carácter con un ancho fijo

Parametros :
   char* character : puntero tipo char que representa un valor de la matriz
   char* color : puntero tipo char que representa un valor de la matriz
   int fixedWidth : puntero tipo char que representa un valor de la matriz
   
Retorno :
   Nada, ya que es tipo void
 
*/
// Función para imprimir un carácter con un ancho fijo
void printCharacterWithFixedWidthAndColor(char* character, char* color, int fixedWidth) {
    int charLength = strlen(character);
    int adjustedWidth = fixedWidth;

    //Verificar si el carácter es especial (por ejemplo, "J₁") y ajustar el ancho
    if (strstr(character, "J") != NULL) {
        adjustedWidth += 2; // Ajustar el ancho por 2 espacios adicionales
    }

    //Calcular cuántos espacios se deben agregar en cada lado
    int padding = adjustedWidth - charLength;

    //Imprimir el color
    printf("%s", color);

    //Imprimir los espacios iniciales
    for (int i = 0; i < padding / 2; i++) {
        printf(" ");
    }

    //Imprimir el carácter
    printf("%s", character);

    //Imprimir los espacios finales
    for (int i = 0; i < padding / 2; i++) {
        printf(" ");
    }

    //Si la longitud es impar, agregar un espacio adicional al final
    if (padding % 2 != 0) {
        printf(" ");
    }

    printf(RESET); // Restablecer el color después de imprimir el carácter
}



/*
La funcion cambiar los valores de la matriz por los eventos

Parametros :
   Nada, no recibe ningun parametro
   
Retorno :
   Nada, ya que es tipo void
 
*/
void PrintTablero(){
    // Imprimir el tablero grande
    for(int i = 0; i < tam_tablero_fondo; i++) {
        for(int j = 0; j < tam_tablero_fondo; j++) {
            // Si el carácter es diferente de espacio, cambiar el fondo a azul
            if(strcmp(fondo[i][j]->character, " ") != 0) {
                fondo[i][j]->color = strdup(BG_BLUE);
            }
            printCharacterWithFixedWidthAndColor(fondo[i][j]->character, fondo[i][j]->color, 4); // 4 es el ancho fijo deseado
        }
        
        printf("\n"); // Agregar una línea de guiones después de cada fila
        if (i % tam_tablero == tam_tablero - 1) {
            // Imprimir la línea de guiones extra después de la última fila de la matriz
            for (int k = 0; k < 200; k++) {
                // Establecer el color de fondo a azul para cada guión
                printf("%s ", BG_BLUE);
            }
            printf("%s\n", RESET);
        }
    }

}



/*
La funcion modifica el tablero del fondo

Parametros :
   char*** tableroscopi : puntero tipo char que apunta a la matriz que se va a mostrar
   
Retorno :
   Nada, ya que es tipo void
 
*/
void ModificacionTablero(char*** tableroscopi) {
    for(int i = 0; i < tam_tablero; i++){
        for(int j = 0; j < tam_tablero; j++){
            //Modificar el fondo y el carácter de una casilla específica
            fondo[i+X][j+Y]->color = strdup("\x1B[41m"); //Cambiar el color de una casilla
            strcpy(fondo[i+X][j+Y]->character, tableroscopi[i][j]); //Colocar un carácter en la casilla
        }
    }
}



/*
La funcion modifica el tablero del fondo

Parametros :
   char*** tableroscopi : puntero tipo char que apunta a la matriz que se va a mostrar
   
Retorno :
   Nada, ya que es tipo void
 
*/
void ModificacionTablero2(char*** tableroscopi, int a, int b) {
    for(int i = 0; i < tam_tablero; i++){
        for(int j = 0; j < tam_tablero; j++){
            //Modificar el fondo y el carácter de una casilla específica
            fondo[i+a][j+b]->color = strdup("\x1B[41m"); //Cambiar el color de una casilla
            strcpy(fondo[i+a][j+b]->character, tableroscopi[i][j]); //Colocar un carácter en la casilla
        }
    }
}




void CambioTablero(char*, int, int); //Prototipo de la funcion CambioTablero, mas abajo esta la implementacion



/*
La funcion cambiar los valores de la matriz por los eventos

Parametros :
   char* jugador : puntero tipo char que apunta al jugador, este puede ser J₂, J₃ o J₄
   char* casilla : puntero tipo char que representa la casilla a modificar
   struct Jugador info : estructura tipo Jugador que contiene la informacion del jugador
   
Retorno :
   Nada, ya que es tipo void
 
*/
void CasillasEspeciales(char* jugador, char* casilla, struct Jugador info) {
    system("clear");
    printf("--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
    printf("                                                                                     %s esta en una casilla especial                                                                                    \n", jugador);
    printf("--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
    printf("                      PID: %d                      Tu Carta: %s                      Tus Tesoros: %d                      Turno Actual: %d                      Ronda: %d                      \n", info.pid, info.Carta, info.Tesoros_encontrados, Turno,info.Rondas);
    printf("--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
    if (strcmp(casilla, "t") == 0) {
        printf("--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
        printf("                                                                                            Se suman 4 rondas :)                                                                                        \n");
        printf("--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
        Turno += 4; //Añadir 4 rondas
    } else if (strcmp(casilla, "n") == 0) {
        printf("--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
        printf("                                                                                            Se restan 3 rondas :(                                                                                       \n");
        printf("--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
        Turno -= 3; //Restar 3 rondas
    }

    printf("--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
    printf("                                                                                          Regresando al Tablero...                                                                                      \n");
    printf("--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
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
void MenuBot(char *jugador, char*** tableroscopi){
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
    while(val == 1){
        system("clear");
        printf("--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
        printf("                                                                                             Es el turno de %s                                                                                          \n", jugador);
        printf("--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
        printf("                      PID: %d                      Tu Carta: %s                      Tus Tesoros: %d                      Turno Actual: %d                      Ronda: %d                      \n", jugadores[num_jugador].pid, jugadores[num_jugador].Carta, jugadores[num_jugador].Tesoros_encontrados, Turno, jugadores[num_jugador].Rondas);
        printf("--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
        PrintTablero();
        //Imprimo el tablero y encuentro la posición del personaje, esta posición es en base al primer 0 que se encuentre
        for(int i = 0; i < tam_tablero_fondo; i++){
            for(int j = 0; j < tam_tablero_fondo; j++){
                if(strcmp(fondo[i][j]->character, jugador) == 0 && !paso){
                    x = i;
                    y = j;
                    paso = true;
                }
            }
        }
        //Aqui se mueve el personaje
        printf("--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
        printf("          Arriba : 1        |          Abajo : 2          |          Derecha : 3          |          Izquierda : 4          |          Usar Carta : 5          |          Cambiar de jugador : 6            \n");
        printf("--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
        printf("Tu posicion actual es: (%d,%d)\n", y, x);
        printf("-> ");
        scanf("%d", &seleccion);
        if(seleccion == 1){
            if(strcmp(fondo[x - 1][y]->character, "/") != 0 && strcmp(fondo[x - 1][y]->character, "E") != 0 && strcmp(fondo[x - 1][y]->character, "J₁") != 0 && strcmp(fondo[x - 1][y]->character, "J₂") != 0 && strcmp(fondo[x - 1][y]->character, "J₃") != 0 && strcmp(fondo[x - 1][y]->character, "J₄") != 0){
                if(strcmp(fondo[x - 1][y]->character, "t") == 0 || strcmp(fondo[x - 1][y]->character, "n") == 0){
                    CasillasEspeciales(jugador, fondo[x - 1][y]->character, jugadores[num_jugador]);
                }
                else if(strcmp(fondo[x - 1][y]->character, "T") == 0 && jugadores[num_jugador].Tesoros_encontrados == 0){
                    jugadores[num_jugador].Tesoros_encontrados = 1;
                    printf("Tienes el Tesoro\n");
                    sleep(5);
                }
                strcpy(fondo[x][y]->character, "0");
                strcpy(fondo[x - 1][y]->character, jugador);
                x--;
            }else{
                printf("No se puede mover hacia Arriba\n");
            }
            jugadores[num_jugador].prox_x = x; //Modifico la direccion en x del jugador
            jugadores[num_jugador].prox_y = y; //Modifico la direccion en y del jugador
        }else if(seleccion == 2){
            if(strcmp(fondo[x + 1][y]->character, "/") != 0 && strcmp(fondo[x + 1][y]->character, "E") != 0 && strcmp(fondo[x + 1][y]->character, "J₁") !=0  && strcmp(fondo[x + 1][y]->character, "J₂") != 0 && strcmp(fondo[x + 1][y]->character, "J₃") != 0 && strcmp(fondo[x + 1][y]->character, "J₄") != 0){
                if(strcmp(fondo[x + 1][y]->character, "t") == 0 || strcmp(fondo[x + 1][y]->character, "n") == 0){
                    CasillasEspeciales(jugador, fondo[x + 1][y]->character, jugadores[num_jugador]);
                }
                else if(strcmp(fondo[x + 1][y]->character, "T") == 0 && jugadores[num_jugador].Tesoros_encontrados == 0){
                    jugadores[num_jugador].Tesoros_encontrados = 1;
                    printf("Tienes el Tesoro\n");
                    sleep(5);
                }
                strcpy(fondo[x][y]->character, "0");
                strcpy(fondo[x + 1][y]->character, jugador);
                x++;
            }else{
                printf("No se puede mover hacia Abajo\n");
            }
            jugadores[num_jugador].prox_x = x; //Modifico la direccion en x del jugador
            jugadores[num_jugador].prox_y = y; //Modifico la direccion en y del jugador
        }else if(seleccion == 3){
            if(strcmp(fondo[x][y + 1]->character, "/") != 0 && strcmp(fondo[x][y + 1]->character, "E") != 0 && strcmp(fondo[x][y + 1]->character, "J₁") !=0 && strcmp(fondo[x][y + 1]->character, "J₂") != 0 && strcmp(fondo[x][y + 1]->character, "J₃") != 0&& strcmp(fondo[x][y + 1]->character, "J₄") != 0){
                if(strcmp(fondo[x][y + 1]->character, "t") == 0 || strcmp(fondo[x][y + 1]->character, "n") == 0){
                    CasillasEspeciales(jugador, fondo[x][y + 1]->character, jugadores[num_jugador]);
                }
                else if(strcmp(fondo[x][y + 1]->character, "T") == 0 && jugadores[num_jugador].Tesoros_encontrados == 0){
                    jugadores[num_jugador].Tesoros_encontrados = 1;
                    printf("Tienes el Tesoro\n");
                    sleep(5);
                }
                strcpy(fondo[x][y]->character, "0");
                strcpy(fondo[x][y + 1]->character, jugador);
                y++;
            }else{
                printf("No se puede mover hacia Derecha\n");
            }
            jugadores[num_jugador].prox_x = x; //Modifico la direccion en x del jugador
            jugadores[num_jugador].prox_y = y; //Modifico la direccion en y del jugador
        }else if(seleccion == 4){
            if(strcmp(fondo[x][y - 1]->character, "/") != 0 && strcmp(fondo[x][y - 1]->character, "E") != 0 && strcmp(fondo[x][y - 1]->character, "J₁") !=0 && strcmp(fondo[x][y - 1]->character, "J₂") != 0 && strcmp(fondo[x][y - 1]->character, "J₃") != 0&& strcmp(fondo[x][y - 1]->character, "J₄") != 0){
                if(strcmp(fondo[x][y - 1]->character, "t") == 0 || strcmp(fondo[x][y - 1]->character, "n") == 0){
                    CasillasEspeciales(jugador, fondo[x][y - 1]->character, jugadores[num_jugador]); 
                }
                else if(strcmp(fondo[x][y - 1]->character, "T") == 0 && jugadores[num_jugador].Tesoros_encontrados == 0){
                    jugadores[num_jugador].Tesoros_encontrados = 1;
                    printf("Tienes el Tesoro\n");
                    sleep(5);
                }
                strcpy(fondo[x][y]->character, "0");
                strcpy(fondo[x][y - 1]->character, jugador);
                y--;
            }else{
                printf("No se puede mover hacia Izquierda\n");
            }
            jugadores[num_jugador].prox_x = x; //Modifico la direccion en x del jugadorJ₃
            jugadores[num_jugador].prox_y = y; //Modifico la direccion en y del jugador
        }else if(seleccion == 5){
            if(strcmp(jugadores[num_jugador].Carta, "Buscar") == 0){
                strcpy(fondo[x][y]->character, "0");
                CambioTablero(jugador, (y - jugadores[num_jugador].tab_Y), (x - jugadores[num_jugador].tab_X)); //Le mandamos la info a la funcion que calcula y realiza el cambio de tablero
                break;
            }else if(strcmp(jugadores[num_jugador].Carta,"Escaleras")==0){
                while(flag){
                    if(x - 1 >= 0 && strcmp(fondo[x - 1][y]->character,"E")==0){ //arriba
                        printf("Salta una '%c'.\n", 'E');
                        strcpy(fondo[x][y]->character, "0");
                        strcpy(fondo[x - 1][y]->character, jugador);
                        x--;
                        flag = false;

                    }else if (x + 1 < tam_tablero && strcmp(fondo[x+1][y]->character, "E")==0){ //abajo
                        printf("Salta una '%c'.\n", 'E');
                        strcpy(fondo[x][y]->character, "0");
                        strcpy(fondo[x + 1][y]->character, jugador);
                        x++;
                        flag = false;
                    
                    }else if (y + 1 < tam_tablero && strcmp(fondo[x][y + 1]->character,"E")==0){ //derecha
                        printf("Salta una '%c'.\n", 'E');
                        strcpy(fondo[x][y]->character, "0");
                        strcpy(fondo[x][y + 1]->character, jugador);
                        y++;       
                        flag = false;             
                    
                    }else if (y - 1 >= 0 && strcmp(fondo[x][y - 1]->character,"E")==0){ //izquierda
                        printf("Salta una '%c'.\n", 'E');
                        strcpy(fondo[x][y]->character, "0");
                        strcpy(fondo[x][y - 1]->character, jugador);
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
            jugadores[num_jugador].prox_x = x; //Modifico la direccion en x del jugador
            jugadores[num_jugador].prox_y = y; //Modifico la direccion en y del jugador
        }else if(seleccion == 6){
            val = 0;
            Turno--;
            break;
        }
        jugadores[num_jugador].Rondas++;
    }

}



/*
La funcion realiza movimiento de nosotros en el tablero

Parametros :
   char*** tableroscopi : puntero tipo char que apunta a la matriz que se va a mostrar
   
Retorno :
   Nada, ya que es tipo void
 
*/
void Menu(char*** tableroscopi){
    //Si el jugador es igual a 1, es decir somos nosotros osea es nuestro turno
    int seleccion;
    int x = 0;
    int y = 0;
    int val = 1;
    bool paso = false;
    bool flag = true;
    while(val == 1){
        system("clear");
        printf("--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
        printf("                                                                                                Es tu turno                                                                                             \n");
        printf("--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
        printf("                      PID: %d                      Tu Carta: %s                      Tus Tesoros: %d                      Turno Actual: %d                      Ronda: %d                      \n", jugadores[0].pid, jugadores[0].Carta, jugadores[0].Tesoros_encontrados, Turno, jugadores[0].Rondas);
        printf("--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
        PrintTablero();
        //Imprimo el tablero y encuentro la posición del personaje, esta posición es en base al primer 0 que se encuentre
        for(int i = 0; i < tam_tablero_fondo; i++){
            for(int j = 0; j < tam_tablero_fondo; j++){
                if(strcmp(fondo[i][j]->character, "J₁") == 0 && !paso){
                    x = i;
                    y = j;
                    paso = true;
                }
            }
        }
        //Aqui se mueve el personaje
        printf("--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
        printf("          Arriba : 1        |          Abajo : 2          |          Derecha : 3          |          Izquierda : 4          |          Usar Carta : 5          |          Cambiar de jugador : 6            \n");
        printf("--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
        printf("Tu posicion actual es: (%d,%d)\n", y, x);
        printf("-> ");
        scanf("%d", &seleccion);
        if(seleccion == 1){
            if(strcmp(fondo[x - 1][y]->character, "/") != 0 && strcmp(fondo[x - 1][y]->character, "E") != 0 && strcmp(fondo[x - 1][y]->character, "J₂") != 0 && strcmp(fondo[x - 1][y]->character, "J₃") != 0 && strcmp(fondo[x - 1][y]->character, "J₄") != 0){
                if(strcmp(fondo[x - 1][y]->character, "t") == 0 || strcmp(fondo[x - 1][y]->character, "n") == 0){
                    CasillasEspeciales("J₁", fondo[x - 1][y]->character, jugadores[0]);
                }
                else if(strcmp(fondo[x - 1][y]->character, "T") == 0 && jugadores[0].Tesoros_encontrados == 0){
                    jugadores[0].Tesoros_encontrados = 1;
                    printf("Tienes el Tesoro\n");
                    sleep(5);
                }
                strcpy(fondo[x][y]->character, "0");
                strcpy(fondo[x - 1][y]->character, "J₁");
                x--;
            }else{
                printf("No se puede mover hacia Arriba\n");
            }
            jugadores[0].prox_x = x; //Modifico la direccion en x del jugador
            jugadores[0].prox_y = y; //Modifico la direccion en y del jugador
        }else if(seleccion == 2){
            if(strcmp(fondo[x + 1][y]->character, "/") != 0 && strcmp(fondo[x + 1][y]->character, "E") != 0 && strcmp(fondo[x + 1][y]->character, "J₂") != 0 && strcmp(fondo[x + 1][y]->character, "J₃") != 0 && strcmp(fondo[x + 1][y]->character, "J₄") != 0){
                if(strcmp(fondo[x + 1][y]->character, "t") == 0 || strcmp(fondo[x + 1][y]->character, "n") == 0){
                    CasillasEspeciales("J₁", fondo[x + 1][y]->character, jugadores[0]);
                }
                else if(strcmp(fondo[x + 1][y]->character, "T") == 0 && jugadores[0].Tesoros_encontrados == 0){
                    jugadores[0].Tesoros_encontrados = 1;
                    printf("Tienes el Tesoro\n");
                    sleep(5);
                }                
                strcpy(fondo[x][y]->character, "0");
                strcpy(fondo[x + 1][y]->character, "J₁");
                x++;
            }else{
                printf("No se puede mover hacia Abajo\n");
            }
            jugadores[0].prox_x = x; //Modifico la direccion en x del jugador
            jugadores[0].prox_y = y; //Modifico la direccion en y del jugador
        }else if(seleccion == 3){
            if(strcmp(fondo[x][y + 1]->character, "/") != 0 && strcmp(fondo[x][y + 1]->character, "E") != 0 && strcmp(fondo[x][y + 1]->character, "J₂") != 0 && strcmp(fondo[x][y + 1]->character, "J₃") != 0&& strcmp(fondo[x][y + 1]->character, "J₄") != 0){
                if(strcmp(fondo[x][y + 1]->character, "t") == 0 || strcmp(fondo[x][y + 1]->character, "n") == 0){
                    CasillasEspeciales("J₁", fondo[x][y + 1]->character, jugadores[0]);
                }
                else if(strcmp(fondo[x][y + 1]->character, "T") == 0 && jugadores[0].Tesoros_encontrados == 0){
                    jugadores[0].Tesoros_encontrados = 1;
                    printf("Tienes el Tesoro\n");
                    sleep(5);
                }
                strcpy(fondo[x][y]->character, "0");
                strcpy(fondo[x][y + 1]->character, "J₁");
                y++;
            }else{
                printf("No se puede mover hacia Derecha\n");
            }
            jugadores[0].prox_x = x; //Modifico la direccion en x del jugador
            jugadores[0].prox_y = y; //Modifico la direccion en y del jugador
        }else if(seleccion == 4){
            if(strcmp(fondo[x][y - 1]->character, "/") != 0 && strcmp(fondo[x][y - 1]->character, "E") != 0 && strcmp(fondo[x][y - 1]->character, "J₂") != 0 && strcmp(fondo[x][y - 1]->character, "J₃") != 0&& strcmp(fondo[x][y - 1]->character, "J₄") != 0){
                if(strcmp(fondo[x][y - 1]->character, "t") == 0 || strcmp(fondo[x][y - 1]->character, "n") == 0){
                    CasillasEspeciales("J₁", fondo[x][y - 1]->character, jugadores[0]);
                }
                else if(strcmp(fondo[x][y - 1]->character, "T") == 0 && jugadores[0].Tesoros_encontrados == 0){
                    jugadores[0].Tesoros_encontrados = 1;
                    printf("Tienes el Tesoro\n");
                    sleep(5);
                }
                strcpy(fondo[x][y]->character, "0");
                strcpy(fondo[x][y - 1]->character, "J₁");
                y--;
            }else{
                printf("No se puede mover hacia Izquierda\n");
            }
            jugadores[0].prox_x = x; //Modifico la direccion en x del jugador
            jugadores[0].prox_y = y; //Modifico la direccion en y del jugador
        }else if(seleccion == 5){
            if(strcmp(jugadores[0].Carta, "Buscar") == 0){
                strcpy(fondo[x][y]->character, "0");
                CambioTablero("J₁", (y - jugadores[0].tab_Y), (x - jugadores[0].tab_X)); //Le mandamos la info a la funcion que calcula y realiza el cambio de tablero
                break;
            }else if(strcmp(jugadores[0].Carta,"Escaleras")==0){
                while(flag){
                    if(x - 1 >= 0 && strcmp(fondo[x - 1][y]->character, "E") == 0){ //arriba
                        printf("Salta una '%c'.\n", 'E');
                        strcpy(fondo[x][y]->character, "0");
                        strcpy(fondo[x - 1][y]->character, "J₁");
                        x--;
                        flag = false;
                    
                    }else if(x + 1 < tam_tablero && strcmp(fondo[x + 1][y]->character, "E") == 0){  //abajo
                        printf("Salta una '%c'.\n", 'E');
                        strcpy(fondo[x][y]->character, "0");
                        strcpy(fondo[x + 1][y]->character, "J₁");
                        x++;
                        flag = false;
                    
                    }else if(y + 1 < tam_tablero && strcmp(fondo[x][y + 1]->character, "E") == 0){  //derecha
                        printf("Salta una '%c'.\n", 'E');
                        strcpy(fondo[x][y]->character, "0");
                        strcpy(fondo[x][y + 1]->character, "J₁");
                        y++;       
                        flag = false;             
                    
                    }else if(y - 1 >= 0 && strcmp(fondo[x][y - 1]->character,"E") == 0){  //izquierda
                        printf("Salta una '%c'.\n", 'E');
                        strcpy(fondo[x][y]->character, "0");
                        strcpy(fondo[x][y - 1]->character, "J₁");
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
            jugadores[0].prox_x = x; //Modifico la direccion en x del jugador
            jugadores[0].prox_y = y; //Modifico la direccion en y del jugador
        }else if(seleccion == 6){
            val = 0;
            Turno--;
            break;
        }
        jugadores[0].Rondas++;
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
void CambioTablero(char *jugador, int x, int y){
    tableros_vistos[0]=true;
    printf("\n");
    printf("\n");
    printf("--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
    printf("                                                                                              Cambio de Tablero                                                                                         \n");
    printf("--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
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
    x = abs(x);
    y = abs(y);
    //Vemos donde puede estar la posible B en los otros tableros
    if(y == tam_tablero - 1){
        y = 0;
        jugadores[num_jugador].tab_X = jugadores[num_jugador].tab_X + tam_tablero;
    }else if(y == 0){
        y = tam_tablero - 1;
        jugadores[num_jugador].tab_X = jugadores[num_jugador].tab_X - tam_tablero;
    }
    if(x == tam_tablero - 1){
        x = 0;
        jugadores[num_jugador].tab_Y = jugadores[num_jugador].tab_Y + tam_tablero;
    }else if(x == 0){
        x = tam_tablero - 1;
        jugadores[num_jugador].tab_Y = jugadores[num_jugador].tab_Y - tam_tablero;
    }

    int pos;
    //Con las nuevas coordenadas busca en los otros tableros si hay una B en tal posicion

    for(int k = 0; k < num_tableros; k++){
        if(strcmp(Tableros[k][y][x], "B") == 0 && tableros_vistos[k] == false){
            jugadores[num_jugador].prox_x = x; //Modifico la direccion en x del jugador
            jugadores[num_jugador].prox_y = y; //Modifico la direccion en y del jugador
            strcpy(Tableros[k][y][x], jugador); //Traspaso el jugador al nuevo tablero
            printf("\n");
            printf("\n");
            printf("--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
            printf("                                                                                              Salto de Tablero                                                                                          \n");
            printf("--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
            
            pos = k;
            tableros_vistos[pos] = true;
            break; //Si encuentra una B se sale del for
            
        }
    }
    jugadores[num_jugador].tabl = pos;
    ModificacionTablero2(Tableros[pos],jugadores[num_jugador].tab_X,jugadores[num_jugador].tab_Y); 
}




/*
La función envía la estructura de jugador a través de una tubería.

Parámetros:
    int pipe_fd : descriptor de archivo de la tubería para escribir.
    struct Jugador* player : puntero a la estructura de Jugador a enviar.

Retorno:
    Nada, ya que es tipo void.
*/
void EnviarJugador(int pipe_fd, const struct Jugador *jugador) {
    // Escribe la estructura del jugador en la tubería
    write(pipe_fd, jugador, sizeof(struct Jugador));
}



/*
La función recibe la estructura de jugador desde una tubería.

Parámetros:
    int fd : descriptor de archivo de la tubería para leer.
    struct Jugador* player : puntero a la estructura de Jugador donde se almacenará la información recibida.

Retorno:
    Nada, ya que es tipo void.
*/
void RecibirJugador(int pipe_fd, struct Jugador *jugador) {
    // Lee la estructura del jugador desde la tubería
    read(pipe_fd, jugador, sizeof(struct Jugador));
}



/*
La funcion inicializa los jugadores en el tablero

Parametros :
   struct Jugador* player : puntero tipo Jugador que contiene la informacion de todos los jugadores
   
Retorno :
   Nada, ya que es tipo void
 
*/
void InicioPartida() {
    pid_t jugador_pid[4];
    int pipes[4][2]; //Un array de tuberías para comunicarse con los procesos hijos

    //Código para el proceso padre
    ModificacionTablero(Tableros[0]);
    tableros_vistos[0] = true;

    // Crear las tuberías antes de crear los procesos hijos
    for(int i = 0; i < 4; i++) {
        if(pipe(pipes[i]) == -1) {
            perror("pipe");
            exit(EXIT_FAILURE);
        }

        jugador_pid[i] = fork();
        if(jugador_pid[i] == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        }else if(jugador_pid[i] == 0) {
            // Código para el proceso hijo (Jugadores 1, 2, 3, 4)
            close(pipes[i][1]); // Cerrar el extremo de escritura de la tubería en el proceso hijo
            struct Jugador jugador;
            jugador.pid = getpid();
            // Leer datos de otros jugadores
            for(int j = 0; j < 4; j++) {
                if(i != j) { // Evitar enviar datos al propio proceso
                    RecibirJugador(pipes[j][0], &jugador);
                }
            }
            close(pipes[i][0]); //Cerrar el extremo de lectura de la tubería en el proceso hijo
            exit(0);
        }else{
            close(pipes[i][0]); //Cerrar el extremo de lectura de la tubería en el proceso padre
        }
    }

    for(int i = 0; i < 4; i++){
        jugadores[i].pid = (int)jugador_pid[i];
    }
    int contador = 0;
    while(contador < 16 && jugadores[0].Tesoros_encontrados == 0 && jugadores[1].Tesoros_encontrados == 0 && jugadores[2].Tesoros_encontrados == 0 && jugadores[3].Tesoros_encontrados == 0){
        Menu(Tableros[jugadores[0].tabl]);
        MenuBot("J₂", Tableros[jugadores[1].tabl]);
        MenuBot("J₃", Tableros[jugadores[2].tabl]);
        MenuBot("J₄", Tableros[jugadores[3].tabl]);

        //Enviar datos de jugador a otros jugadores
        for(int i = 0; i < 4; i++){
            if(i != 0) { //Evitar enviar datos al propio proceso
                EnviarJugador(pipes[i][1], &jugadores[0]);
            }
        }
        system("clear");
        printf("--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
        printf("                                                                                             Cargando Ronda %d                                                                                           \n",contador+1);
        printf("--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
        contador++;
    }


    //Matar a los procesos hijos
    for (int i = 0; i < 4; i++) {
        kill(jugador_pid[i], SIGTERM);
    }

    //Esperar a que todos los procesos hijos terminen
    for(int i = 0; i < 4; i++) {
        waitpid(jugador_pid[i], NULL, 0);
    }

    printf("--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
    printf("                                                                                                  FIN                                                                                                   \n");
    printf("--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
}





/*
La funcion cambiar los valores de la matriz por los eventos

Parametros :
   char* val : puntero tipo char que representa un valor de la matriz
   
Retorno :
   Retorna un puntero tipo char que repreesenta el valor modificado al entregado
 
*/
char *eventos(){
    int numAleat2 = rand() % 4;
    char *var1 = malloc(3 * sizeof(char));  //Asigna memoria para un string de 2 caracteres
    if(numAleat2 == 0){
        strcpy(var1, "c");
    }else if(numAleat2 == 1){
        strcpy(var1, "t");
    }else if(numAleat2 == 2){
        strcpy(var1, "n");
    }else if(numAleat2 == 3){
        strcpy(var1, "p");
    }
    return var1;
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
    Tableros[glob] = malloc(sizeof(char *) * tam_tablero);

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
            Tableros[glob][i] = malloc(sizeof(char *) * (tam_tablero));
            for (int j = 0; j < 5; j++){
                Tableros[glob][i][j] = malloc(sizeof(char) * 3); //REVISADO
                char *valor = malloc(strlen(matriz[i][j]) + 1); 
                strcpy(valor,matriz[i][j]);
                strcpy(Tableros[glob][i][j], valor);
            }
        }
        char* valor1 = eventos();
        int i;
        int j;
        do{
            i = rand() % 5;
            j = rand() % 5;
        }while(strcmp(Tableros[glob][i][j],"B") == 0 || strcmp(Tableros[glob][i][j],"E") == 0 || strcmp(Tableros[glob][i][j],"/") == 0 || strcmp(Tableros[glob][i][j], "J₁") == 0|| strcmp(Tableros[glob][i][j], "J₂") == 0 || strcmp(Tableros[glob][i][j], "J₃") == 0 || strcmp(Tableros[glob][i][j], "J₄") == 0);
        strcpy(Tableros[glob][i][j], valor1);
    }else{
        for(int i = 0; i < tam_tablero ; i++){
            Tableros[glob][i] = malloc(sizeof(char *) * (tam_tablero));
            for(int j = 0; j < tam_tablero ; j++) {
                Tableros[glob][i][j] = malloc(sizeof(char) * 3); //REVISADO
                char *variable = malloc(3 * sizeof(char)); 
                if(fscanf(archivolectura, "%s ", variable) == 1){
                    strcpy(Tableros[glob][i][j], variable);
                }else{
                    break;
                }
            }
        }
        char* valor1 = eventos();
        int i;
        int j;
        do{
            i = rand() % 5;
            j = rand() % 5;
        }while(strcmp(Tableros[glob][i][j],"B") == 0 || strcmp(Tableros[glob][i][j],"E") == 0 || strcmp(Tableros[glob][i][j],"/") == 0 || strcmp(Tableros[glob][i][j], "J₁") == 0|| strcmp(Tableros[glob][i][j], "J₂") == 0 || strcmp(Tableros[glob][i][j], "J₃") == 0 || strcmp(Tableros[glob][i][j], "J₄") == 0);
        strcpy(Tableros[glob][i][j], valor1);
        
    }
    
    glob++;
    fclose(archivolectura);
}

/*
La función verifica si el número está repetido en un arreglo de números.

Parámetros:
    int numeros[] : Un arreglo de enteros que contiene números generados previamente.
    int n : La cantidad de números en el arreglo.
    int num : El número que se desea verificar si está repetido.

Retorno:
    Retorna  1 si el número está repetido en el arreglo o 0 si es un número nuevo.
*/
int numRepetido(int numeros[], int n, int num) {
    for (int i = 0; i < n; i++) {
        if (numeros[i] == num) {
            return 1; // El número ya ha sido generado
        }
    }
    return 0; // El número es nuevo
}


/*
La funcion busca al azar 4 tableros y coloca 1 tepera por jugador

Parametros :
   char* val : puntero tipo char que representa un valor de la matriz
   
Retorno :
   Nada, ya que es tipo void
 
*/
void Tesoros() {
    int numeros[4];
    int numerosAdicionales[4][2];
    int tab;
    int cor_x;
    int cor_y;

    // Genera 4 números aleatorios distintos en el rango de 0 a 9
    for (int i = 0; i < 4; i++) {
        do{
            int numAleatorio;
            do {
                numAleatorio = rand() % 8 + 1; // Genera un número aleatorio entre 0 y 9
            } while (numRepetido(numeros, i, numAleatorio)); // Verifica si el número ya ha sido generado

            numeros[i] = numAleatorio; // Almacena el número aleatorio en el arreglo

            // Genera 2 números adicionales entre 0 y 4 basados en el número aleatorio
            for (int j = 0; j < 2; j++) {
                numerosAdicionales[i][j] = rand() % 5; // Genera un número aleatorio entre 0 y 4
            }
            tab = numeros[i];
            cor_x = numerosAdicionales[i][0];
            cor_y = numerosAdicionales[i][1];

        }while (strcmp(Tableros[tab][cor_x][cor_y], "B") == 0 || strcmp(Tableros[tab][cor_x][cor_y], "E") == 0 || strcmp(Tableros[tab][cor_x][cor_y], "/") == 0 || strcmp(Tableros[tab][cor_x][cor_y], "J₁") == 0 || strcmp(Tableros[tab][cor_x][cor_y], "J₂") == 0 || strcmp(Tableros[tab][cor_x][cor_y], "J₃") == 0 || strcmp(Tableros[tab][cor_x][cor_y], "J₄") == 0 || strcmp(Tableros[tab][cor_x][cor_y], "T") == 0);
        strcpy(Tableros[tab][cor_x][cor_y], "T");
    }
}
/*
La funcion revisa el directorio actual para encontrar los archivo txt a leer

Parametros :
   No recibe ningun parametro

Retorno :
    Retorna un puntero doble tipo char que representa un array donde estan los nombres de los txt
 
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
    printf("========================================================================================================================================================================================================\n");
    printf("                                                                                                Magic Maze                                                                                              \n");
    printf("========================================================================================================================================================================================================\n");
    
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

    //Asignar memoria para fondo
    fondo = malloc(sizeof(Cell **) * tam_tablero_fondo);

    for (int i = 0; i < tam_tablero_fondo; i++) {
        fondo[i] = malloc(sizeof(Cell *) * tam_tablero_fondo);
        for (int j = 0; j < tam_tablero_fondo; j++) {
            fondo[i][j] = malloc(sizeof(Cell));
            fondo[i][j]->color = strdup(BG_BLUE);
            strcpy(fondo[i][j]->character, " "); // Carácter por defecto
        }
    }
    
    // Aqui estan todos los jugadores
    jugadores = (struct Jugador*)malloc(sizeof(struct Jugador) * 4);

    // Se crean las cartas
    char *Cartas[num_cartas] = {"Buscar", "Escaleras", "Escaleras", "Buscar"};

    // Se verifican que no se repitan las cartas
    bool cartas_asignadas[num_cartas];
    for(int i = 0; i < num_cartas; i++){
        cartas_asignadas[i] = false;
    }
    // Se verifican que no se repitan los tableros 
    for(int i = 0; i < num_tableros; i++){
        tableros_vistos[i] = false;
    }

    // Inicializar generador de números aleatorios
    srand(time(NULL));
    printf("\n");
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
        jugadores[i].tabl = 0;
        jugadores[i].tab_X = X;
        jugadores[i].tab_Y = Y;
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
    // Se colocan los tesoros
    Tesoros();
    int nueva_partida;
    printf("--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
    printf("                                                                                    Presione 1 para nueva partida:                                                                                      \n");
    printf("--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
    printf("--> ");
    scanf("%d", &nueva_partida);
    
    if(nueva_partida == 1){
        //Se crean los Jugadores
        InicioPartida();
    }else{
        //Se sale del programa
        exit(0);
    }
    
    return 0;
}
