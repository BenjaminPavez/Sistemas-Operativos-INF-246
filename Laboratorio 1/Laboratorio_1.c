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
La funcion crea un directorio y mueve los archivos txt (revisar)

Parametros :
   const char* nombreArchivo : puntero tipo char que apunta al nombre del archivo
   const char* origen : puntero tipo char que apunta al nombre (revisar)
   const char* destino : puntero tipo char que apunta (revisar)
   
Retorno :
   retorna -1 si el archivo no se pudo mover o no se pudo crear la carpeta y retorna 0 si se realizo correctamente
 
*/
int moverArchivo(const char* nombreArchivo, const char* origen, const char* destino) {
    //Verificar si la carpeta existe, si no, crearla
    struct stat st = {0};
    if (stat(destino, &st) == -1) {
        //La carpeta no existe, la creamos
        if (mkdir(destino, 0777) == -1) {
            perror("Error al crear la carpeta");
            return -1;
        }
    }
    char rutaOrigen[255]; //Ajusta el tamaño según tus necesidades
    char rutaDestino[255]; //Ajusta el tamaño según tus necesidades

    //Construir las rutas completas de origen y destino
    snprintf(rutaOrigen, sizeof(rutaOrigen), "%s/%s", origen, nombreArchivo);
    snprintf(rutaDestino, sizeof(rutaDestino), "%s/%s", destino, nombreArchivo);

    //Mover el archivo
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
La funcion quita los espacios de una cadena

Parametros :
   char *cadena : puntero tipo char que apunta a la cadena a quitar los espacios
   char *cadenasin : puntero tipo char que apunta a la cadena sin los espacios
   
Retorno :
   char *cadenasin : puntero tipo char que apunta a la cadena sin los espacios
 
*/
void quitaespacios(char *cadena, char *cadenasin) {
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

    //Agrega el carácter nulo al final de la cadena resultante
    cadenasin[j] = '\0'; 
}



void convertirMayusculas(char *cadena) {
    for (int i = 0; cadena[i]; i++) {
        cadena[i] = toupper(cadena[i]);
    }
}



/*
La funcion resuelve la sopa de letras en la dirección dada (horizontal o vertical)

Parametros :
   FILE *archivo : archivo que contiene la sopa de letras
   char *nom_sopa : puntero tipo char que apunta al nombre del archivo a leer 
   char *orientation : puntero tipo char que apunta a la orientacion de la palabra en la sopa
   int tam : entero que contiene la dimension de la sopa de letras
   
Retorno :
   Nada, ya que es tipo void
 
*/
void Solucion(FILE *archivo, char *nom_sopa, char *orientation, int tam){
    int fila = 0;

    //Medicion tiempo
    double time_spent = 0.0;
    clock_t begin = clock();

    bool found = false;
    char bufer[500]; // Aquí vamos a ir almacenando cada línea

    //Palabra a buscar en la sopa
    convertirMayusculas(nom_sopa);

    if(strcasecmp(orientation, "horizontal") == 0) {
        //Horizontal
        char cadenafinal[500]; // Define un buffer para cadenafinal
        while(fgets(bufer, sizeof(bufer), archivo)){
            quitaespacios(bufer, cadenafinal);
            
            if(strstr(cadenafinal, nom_sopa) != NULL){
                //Se calcula el tiempo de ejecucion
                clock_t end = clock();
                //calcula el tiempo transcurrido encontrando la diferencia (end - begin) y
                //dividiendo la diferencia por CLOCKS_PER_SEC para convertir a segundos
                time_spent += (double)(end - begin) / CLOCKS_PER_SEC;

                //Verificacion, RECORDAR ELIMINAR AL ENVIAR
                printf("ESTADO: Palabra encontrada en la fila %d.\n",fila+2);
                printf("Tiempo que transcurrido: %f\n",time_spent);
                printf("----------------------------------------------------------------\n");
                printf("\n");
                printf("\n");

                found = true;
                break;
            }
            fila++;
            memset(cadenafinal, 0, 500); //limpiar para que no lo una con la basura
        }
        memset(cadenafinal, 0, 500); //limpiar para que no lo una con la basura

        fclose(archivo);
        
    }else if(strcasecmp(orientation, "vertical") == 0){ 
        tam = (tam+1)/2; 

        char **matriz = malloc(sizeof(char *) * tam);
        for (int i = 0; i < tam; i++) {
            matriz[i] = malloc(sizeof(char) * tam); // Asigna memoria para cada fila
            for (int j = 0; j < tam; j++) {
                fscanf(archivo, " %c", &matriz[i][j]);
            }
        }
        
        for (int i = 0; i < tam; i++) {
            for (int j = i + 1; j < tam; j++) {
                char temp = matriz[i][j];
                matriz[i][j] = matriz[j][i];
                matriz[j][i] = temp;
            }
        }

        //Escribir la matriz transpuesta en el archivo de salida
        for (int i = 0; i < tam; i++){
            char columna[500];
            for (int j = 0; j < tam; j++) {
                columna[j] = matriz[i][j];
            }
            columna[tam] = '\0'; 
            if(strstr(columna, nom_sopa) != NULL){
                //printf("%s",columna);
                //Se calcula el tiempo de ejecucion
                clock_t end = clock();
                //calcula el tiempo transcurrido encontrando la diferencia (end - begin) y
                //dividiendo la diferencia por CLOCKS_PER_SEC para convertir a segundos
                time_spent += (double)(end - begin) / CLOCKS_PER_SEC;

                //Verificacion, RECORDAR ELIMINAR AL ENVIAR
                printf("ESTADO: Palabra encontrada en la columna %d.\n",i+2);
                printf("Tiempo que transcurrido: %f\n",time_spent);
                printf("----------------------------------------------------------------\n");
                printf("\n");
                printf("\n");
                found = true;
            }
        }
        
        //Se libera la matriz para evitar leaks en la memoria
        for (int i = 0; i < tam; i++) {
            free(matriz[i]);
        }
        free(matriz);

    }else{
        printf("Orientación inválida: %s\n", orientation);
        return;
    }

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
    char sentido[12];
    
    //Leo el archivo para calcular la longitud de la linea
    FILE* archivo = fopen(nom_archivo, "r"); // Modo lectura
    char bufer[1000]; // Aquí vamos a ir almacenando cada línea
    fgets(bufer, 1000, archivo); //Se salta la primea linea
    int longitud = strlen(fgets(bufer, 1000, archivo));
    fclose(archivo);

    //Abro el archivo para extraer la sopa
    FILE* archivolectura = fopen(nom_archivo, "r+");
    char linea_arch[1000]; //Aquí vamos a ir almacenando cada línea

    //Leer la primera línea para obtener la orientación
    if(fgets(linea_arch, 1000, archivolectura)){
        linea_arch[strcspn(linea_arch, "\n")] ='\0'; 
        if(strcasecmp(linea_arch, "vertical") == 0 || strcasecmp(linea_arch, "horizontal") == 0){
            strcpy(sentido,linea_arch);

            //Resolvemos la sopa de letras
            strncpy(nom_archivo2, nom_archivo, strlen(nom_archivo) - 4); //Restamos 4 para eliminar la extensión ".txt"
            nom_archivo2[strlen(nom_archivo) - 4] = '\0'; 

            //Verificacion, RECORDAR ELIMINAR AL ENVIAR
            printf("----------------------------------------------------------------\n");
            printf("Nombre del Archivo: %s\n", nom_archivo);
            printf("----------------------------------------------------------------\n");
            printf("Orientacion: %s\n", linea_arch);
            printf("Tamaño: %d x %d\n",(longitud+1)/2, (longitud+1)/2);

            Solucion(archivolectura, nom_archivo2, sentido, longitud);
            

            //sprintf(destino, "%dx%d", (longitud+1)/2, (longitud+1)/2);
            //moverArchivo(nom_archivo, rutaActual, destino);

        }else{
            printf("Error: La oriencion %s no existe\n",linea_arch);
        }
    }
    
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
    LeerDir();
    return 0;
}
