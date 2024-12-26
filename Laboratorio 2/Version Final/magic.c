#include "magic.h"



/*
La funcion revisa el directorio actual para encontrar los archivo txt a leer

Parametros :
   No recibe ningun parametro

Retorno :
   Nada, ya que es tipo void

*/
int start() {
    printf("========================================================================================================================================================================================================\n");
    printf("                                                                                                Magic Maze                                                                                              \n");
    printf("========================================================================================================================================================================================================\n");

    //Aqui se guardan todos los tableros
    Tableros = malloc(sizeof(char***) * 9);

    for (int i = 0; i < num_tableros; i++) {
        Tableros[i] = malloc(sizeof(char**) * tam_tablero);
        for (int j = 0; j < tam_tablero; j++) {
            Tableros[i][j] = malloc(sizeof(char*) * tam_tablero);
            for (int k = 0; k < tam_tablero; k++) {
                Tableros[i][j][k] = malloc(sizeof(char) * 3); //Suficiente espacio para "J₁", "J₂", "J₃", "J₄"
            }
        }
    }

    //Asignar memoria para fondo
    fondo = malloc(sizeof(Cell**) * tam_tablero_fondo);

    for (int i = 0; i < tam_tablero_fondo; i++) {
        fondo[i] = malloc(sizeof(Cell*) * tam_tablero_fondo);
        for (int j = 0; j < tam_tablero_fondo; j++) {
            fondo[i][j] = malloc(sizeof(Cell));
            fondo[i][j]->color = strdup(BG_BLUE);
            strcpy(fondo[i][j]->character, " ");
        }
    }

    jugadores = (struct Jugador*)malloc(sizeof(struct Jugador) * 4);

    char* Cartas[num_cartas] = { "Buscar", "Escaleras", "Escaleras", "Buscar" };

    //Se verifican que no se repitan las cartas
    bool cartas_asignadas[num_cartas];
    for (int i = 0; i < num_cartas; i++) {
        cartas_asignadas[i] = false;
    }

    //Se verifican que no se repitan los tableros 
    for (int i = 0; i < num_tableros; i++) {
        tableros_vistos[i] = false;
    }

    //Inicializar generador de numeros aleatorios
    srand(time(NULL));
    printf("\n");
    printf("-> Revolviendo el mazo de cartas y los tableros...\n");
    printf("\n");
    combinarCartas(Cartas, num_cartas);

    for (int i = 0; i < 4; i++) {
        int numero;
        do {
            numero = rand() % num_cartas;
        } while (cartas_asignadas[numero]);
        jugadores[i].Carta = Cartas[numero];
        jugadores[i].Rondas = 1; //REVISAR
        jugadores[i].tabl = 0;
        jugadores[i].tab_X = X;
        jugadores[i].tab_Y = Y;
        cartas_asignadas[numero] = true;
    }
    printf("-> Tu carta es: %s\n", jugadores[0].Carta);
    printf("\n");


    char** obj = LeerDir();
    combinarMapas(obj, num_tableros);

    for (int i = 0; i < num_tableros; i++) {
        char* nom_tabl = obj[i];
        CrearTablero(nom_tabl);
    }

    //Se colocan los tesoros
    Tesoros();
    int nueva_partida;
    printf("--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
    printf("                                                                                    Presione 1 para nueva partida:                                                                                      \n");
    printf("--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
    printf("--> ");
    scanf("%d", &nueva_partida);

    if (nueva_partida == 1) {
        //Se crean los Jugadores
        InicioPartida();
    }
    else {
        //Se sale del programa
        exit(0);
    }

    return 0;
}