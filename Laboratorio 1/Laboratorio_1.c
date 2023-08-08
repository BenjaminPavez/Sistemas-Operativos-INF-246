#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <ctype.h>
#include <dirent.h>
#include <unistd.h>
#include <time.h>



/*
La funcion resuelve la sopa de letras en la dirección dada (horizontal o vertical)

Parametros :
   FILE *archivo : archivo que contiene la sopa de letras
   char *nom_sopa : puntero tipo char que apunta al nombre del archivo a leer 
   char *orientation : puntero tipo char que apunta a la orientacion de la palabra en la sopa
   
Retorno :
   Nada, ya que es tipo void
 
*/
int moverArchivo(const char* nombreArchivo, const char* origen, const char* destino) {
    // Verificar si la carpeta existe, si no, crearla
    struct stat st = {0};
    if (stat(destino, &st) == -1) {
        // La carpeta no existe, la creamos
        if (mkdir(destino, 0777) == -1) {
            perror("Error al crear la carpeta");
            return -1;
        }
    }
    char rutaOrigen[255]; // Ajusta el tamaño según tus necesidades
    char rutaDestino[255]; // Ajusta el tamaño según tus necesidades

    // Construir las rutas completas de origen y destino
    snprintf(rutaOrigen, sizeof(rutaOrigen), "%s/%s", origen, nombreArchivo);
    snprintf(rutaDestino, sizeof(rutaDestino), "%s/%s", destino, nombreArchivo);

    // Mover el archivo
    if (rename(rutaOrigen, rutaDestino) != 0) {
        perror("Error al mover el archivo");
        return -1;
    }

    printf("Archivo guardado en %s\n", destino);
    return 0;
}



/*
La funcion agrega espacios entre medio de una cadena dada

Parametros :
   char *palabra : puntero tipo char que apunta a la palabra a agregar los espacios
   
Retorno :
   char *finpalabra : puntero tipo char que apunta a la palabra con los espacios
 
*/
char* Espacio(char* palabra){
   int largo = strlen(palabra);
   int nlargo = largo + (largo-1);
   char* finpalabra = malloc(sizeof(char) * nlargo);
   int j = 0;
   for(int i = 0; i < nlargo; i++){
        if(i % 2 == 0) {
            finpalabra[i] = palabra[j];
            j++;
        }else {
            finpalabra[i] = ' ';
        }
    }
   return finpalabra;
}



/*
La funcion remplaza la subcadena dentro de la subcadena por un simbolo dado

Parametros :
   char *source : puntero tipo char que apunta a la cadena
   char *target : puntero tipo char que apunta a la subcadena
   char *replacement : puntero tipo char que apunta a lo que remplazara a la subcadena
   
Retorno :
   Nada, ya que es tipo void
 
*/
void replace(char *source, char *target, char *replacement) {
   int target_length = strlen(target);
   int replacement_length = strlen(replacement);

   char *found = strstr(source, target);

    while (found) {
        // Calcula la longitud de la parte antes de 'target'
        int prefix_length = found - source;

        // Calcula la longitud de la parte después de 'target'
        int suffix_length = strlen(found + target_length);

        // Calcula la longitud total de la nueva cadena
        int new_length = prefix_length + replacement_length + suffix_length + 1;

        // Crea una nueva cadena para almacenar la versión modificada
        char new_string[new_length];

        // Copia la parte antes de 'target' en la nueva cadena
        strncpy(new_string, source, prefix_length);
        new_string[prefix_length] = '\0';

        // Agrega la cadena de reemplazo a la nueva cadena
        strcat(new_string, replacement);

        // Agrega la parte después de 'target' a la nueva cadena
        strcat(new_string, found + target_length);

        // Copia la nueva cadena en 'source'
        strcpy(source, new_string);

        // Busca la siguiente ocurrencia de 'target' en 'source'
        found = strstr(source, target);
    }
    
}



/*
La funcion quita los espacios de una cadena

Parametros :
   char *cadena : puntero tipo char que apunta a la cadena a quitar los espacios
   char *cadenasin : puntero tipo char que apunta a la cadena sin los espacios
   
Retorno :
   char *cadenasin : puntero tipo char que apunta a la cadena sin los espacios
 
*/
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



/*
La funcion resuelve la sopa de letras en la dirección dada (horizontal o vertical)

Parametros :
   FILE *archivo : archivo que contiene la sopa de letras
   char *nom_sopa : puntero tipo char que apunta al nombre del archivo a leer 
   char *orientation : puntero tipo char que apunta a la orientacion de la palabra en la sopa
   
Retorno :
   Nada, ya que es tipo void
 
*/
void Solucion(FILE *archivo, char *nom_sopa, char *orientation){
    int longitud;
    int nom_archivo2Length = strlen(nom_sopa);
    bool found = false;
    char bufer[1000]; // Aquí vamos a ir almacenando cada línea
    char *nombre = nom_sopa;

    //Palabra a buscar en la sopa
    int texto_len = strlen(nom_sopa);
    char *textoMayus = malloc(sizeof(char) * texto_len); //reservando memoria para la variable a retornar
    for (int i = 0; i < texto_len; i++){
        textoMayus[i] = toupper(nom_sopa[i]); 
    }

    if(strcmp(orientation, "horizontal") == 0) {
        //Horizontal
        int fila = 0;
        //Se crea el nuevo archivo con las respuestas
        FILE *nuevoArchivo;
        strcat(nom_sopa, "_resuelta.txt");
        nuevoArchivo = fopen(nom_sopa, "w");
        fputs("horizontal", nuevoArchivo); // Escribir línea modificada en el nuevo archivo
        fputc('\n', nuevoArchivo);
        bool tam = true;
        while(fgets(bufer, sizeof(bufer), archivo)){
            //Se calcula la dimension de la sopa
            if(tam){
                longitud = strlen(bufer);
                tam = false;
            }
            for (int col = 0; col < strlen(bufer) - nom_archivo2Length + 1; col++) {
                char *cadenafinal = (char *)malloc(strlen(&bufer[col]) + 1); // +1 para el carácter nulo
                quitaespacios(&bufer[col], cadenafinal);
                if(strstr(cadenafinal, textoMayus) != NULL){
                    //Modifico el archivo para decir que se encontro
                    char *target = Espacio(textoMayus);
                    char *rem = malloc(sizeof(char) * strlen(textoMayus));
                    for(int i = 0; i < strlen(textoMayus); i++){
                        rem[i] = '-';
                    }
                    char *replacement = Espacio(rem);
                    replace(&bufer[col], target, replacement);
                    printf("Palabra encontrada en fila %d, columna %d (horizontal).\n", fila + 2, col);
                    found = true;
                    break;
                }  
            }
            bufer[strcspn(bufer, "\n")] = '\0'; // Eliminamos el salto de línea después de procesar la línea
            fputs(bufer, nuevoArchivo); // Escribir línea modificada en el nuevo archivo
            fputc('\n', nuevoArchivo); // Agregar un salto de línea después de cada línea
            fila++;
        }
        
    }else if (strcmp(orientation, "vertical") == 0) { //cambiar a vertical
        //Vertical
        char buffer[100];
        fgets(buffer, sizeof(buffer), archivo);
        char matriz[100][100];  // Ajusta el tamaño de la matriz según tus necesidades
        int filas = 100;
        int columnas = 100;
        for (int i = 0; i < filas; i++) {
            for (int j = 0; j < columnas; j++) {
                if (fscanf(archivo, " %c", &matriz[i][j]) == EOF) {
                    perror("Error al leer el archivo");
                    return;
                }
                
            }
        }
        fclose(archivo);

        //Transponer la matriz en el mismo arreglo
        for (int i = 0; i < filas; i++) {
            for (int j = i + 1; j < columnas; j++) {
                char temp = matriz[i][j];
                matriz[i][j] = matriz[j][i];
                matriz[j][i] = temp;
            }
        }

        // Imprimir matriz transpuesta
        for (int i = 0; i < columnas; i++) {
            char *cadenafinal = (char *)malloc(strlen(matriz[i]) + 1); // +1 para el carácter nulo
            quitaespacios(matriz[i], cadenafinal);
            if(strstr(cadenafinal, textoMayus) != NULL) {
                printf("Palabra encontrada en la columna %d (vertical).\n",i);
                found = true;
                break;
            }
        }

    }else{
        printf("Orientación inválida: %s\n", orientation);
        return;
    }
    //Verificacion, RECORDAR ELIMINAR AL ENVIAR
    printf("----------------------------------------------------------------\n");
    printf("Nombre del Archivo: %s\n", nombre);
    printf("----------------------------------------------------------------\n");
    printf("Orientacion: %s\n", orientation);
    printf("Tamaño: %d x %d\n",(longitud+1)/2, (longitud+1)/2);
    printf("----------------------------------------------------------------\n");
    printf("\n");
    printf("\n");




    //Se verifica que la sopa se pudo resolver
    if(found){
        printf("Sopa de letras resuelta\n");
    }else{
        printf("No se pudo resolver la sopa de letras\n");
    }

}



/*
La funcion a partir del nombre del archivo extrae la informacion necesaria

Parametros :
   char* nom_archivo : puntero tipo char que apunta al nombre del archivo a leer 
   
Retorno :
   Nada, ya que es tipo void
 
*/
void Orientacion(char* nom_archivo) {
    // Obtenemos origen de los archivos txt
    char rutaActual[FILENAME_MAX]; // Tamaño suficiente para almacenar la ruta
    if (getcwd(rutaActual, sizeof(rutaActual)) == NULL) {
        perror("Error al obtener la ruta actual");
    }

    char nom_archivo2[25]; // Tamaño suficiente para almacenar la palabra extraída
    char destino[25];
    char sentido[12];
    
    FILE* archivo = fopen(nom_archivo, "r+"); // Modo lectura
    char bufer[1000]; // Aquí vamos a ir almacenando cada línea

    // Leer la primera línea para obtener la orientación
    if(fgets(bufer, 1000, archivo)) {
        bufer[strcspn(bufer, "\n")] ='\0'; 
        if(strcmp(bufer, "vertical") == 0 || strcmp(bufer, "horizontal") == 0){
            strcpy(sentido,bufer);
            //Leer la segunda línea para obtener el tamaño del lado (ERROR Solucionado)
            int longitud = strlen(bufer);
            
            //Resolvemos la sopa de letras
            strncpy(nom_archivo2, nom_archivo, strlen(nom_archivo) - 4); // Restamos 4 para eliminar la extensión ".txt"
            nom_archivo2[strlen(nom_archivo) - 4] = '\0'; 
            Solucion(archivo, nom_archivo2, sentido);

            sprintf(destino, "%dx%d", (longitud+1)/2, (longitud+1)/2);
            //moverArchivo(nom_archivo, rutaActual, destino);
            
            
        }else{
            printf("Error: %s\n", bufer);
            printf("ACAAAAAAA\n");
        }
    }
    fclose(archivo); 
}



/*
La funcion revisa el directorio actual para encontrar los archivo txt a leer

Parametros :
   No recibe ningun parametro

Retorno :
   Nada, ya que es tipo void
 
*/
void LeerDir() {
    DIR *d = opendir(".");
    struct dirent *dentry;
    size_t i = 1;
    
    while ((dentry = readdir(d)) != NULL) {
        if (strstr(dentry->d_name, ".txt") != NULL) {
            Orientacion(dentry->d_name);
            i++;
        }
    }
    closedir(d);
}



/*
La funcion Main

Parametros :
   No recibe ningun parametro

Retorno :
   retorna 0 si se ejecuto correctamente
 
*/
int main(){
    /*
    //Carpetas Horizontal
    mkdir("./horizontal", S_IRWXU);

    mkdir("./horizontal/50x50", S_IRWXU);
    mkdir("./horizontal/100x100", S_IRWXU);
    mkdir("./horizontal/200x200", S_IRWXU);

    //Carpetas Vertical
    mkdir("./vertical", S_IRWXU);

    mkdir("./vertical/50x50", S_IRWXU);
    mkdir("./vertical/100x100", S_IRWXU);
    mkdir("./vertical/200x200", S_IRWXU);
    */
    LeerDir();
    return 0;
}
