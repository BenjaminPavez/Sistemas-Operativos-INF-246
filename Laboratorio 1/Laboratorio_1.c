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
La funcion crea las rutas necesarias para luego clasificar y mover las sopas a la nueva carpeta

Parametros :
   const char* nombreArchivo : puntero tipo char que apunta al nombre del archivo
   const char* orientacion : puntero tipo char que apunta a la orientacion de la palabra en la sopa
   int tamano : entero que indica la dimension de la sopa
   
Retorno :
   retorna -1 si el archivo no se pudo mover o no se pudo crear la carpeta y retorna 0 si se realizo correctamente
 
*/
int moverArchivo(const char* nombreArchivo, char* orientation, int tam){
    char rutaActual[600]; //Tamaño suficiente para almacenar la ruta
    if (getcwd(rutaActual, sizeof(rutaActual)) == NULL) {
        perror("Error al obtener la ruta actual");
        return -1;
    }

    //Verificar si la carpeta destino existe, si no, crearla
    for (int i = 0; i < strlen(orientation); i++) {
        orientation[i] = tolower(orientation[i]);
    }

    //Crear directorio "vertical" 
    char destinoVertical[600];
    snprintf(destinoVertical, sizeof(destinoVertical), "%.590s/vertical", rutaActual);
    struct stat st = {0};
    if (stat(destinoVertical, &st) == -1) {
        if (mkdir(destinoVertical, 0777) == -1) {
            perror("Error al crear la carpeta vertical");
            return -1;
        }
    }

    //Crear directorio "horizontal"
    char destinoHorizontal[600];
    snprintf(destinoHorizontal, sizeof(destinoHorizontal), "%.588s/horizontal", rutaActual);
    if (stat(destinoHorizontal, &st) == -1) {
        if (mkdir(destinoHorizontal, 0777) == -1) {
            perror("Error al crear la carpeta horizontal");
            return -1;
        }
    }

    //Crear subcarpeta en función de la orientación y el tamaño
    const char* subDestino;
    if (strcasecmp(orientation, "vertical") == 0) {
        subDestino = destinoVertical;
    } else if (strcasecmp(orientation, "horizontal") == 0) {
        subDestino = destinoHorizontal;
    } else {
        printf("Orientación inválida: %s\n", orientation);
        return -1;
    }

    char subcarpeta[600];
    snprintf(subcarpeta, sizeof(subcarpeta), "%s/%dx%d", subDestino, tam, tam);

    if (stat(subcarpeta, &st) == -1) {
        if (mkdir(subcarpeta, 0777) == -1) {
            perror("Error al crear la subcarpeta");
            return -1;
        }
    }

    char rutaOrigen[600];
    snprintf(rutaOrigen, sizeof(rutaOrigen), "%.589s/%s", rutaActual, nombreArchivo);

    char rutaDestinoFinal[600];
    snprintf(rutaDestinoFinal, sizeof(rutaDestinoFinal), "%.589s/%s", subcarpeta, nombreArchivo);

    //Se mueve el archivo a la ruta final
    if (rename(rutaOrigen, rutaDestinoFinal) != 0) {
        perror("Error al mover el archivo");
        return -1;
    }

    printf("UBICACION: %s\n", subcarpeta);
    return 0;
}



/*
La funcion quita los espacios de una cadena

Parametros :
   char *cadena : puntero tipo char que apunta a la cadena a quitar los espacios
   char *cadenasin : puntero tipo char que apunta a la cadena sin los espacios
   
Retorno :
   char *cadenasin : puntero tipo char que apunta a la cadena sin los espacios
 
*/
void quitaespacios(char *cadena, char *cadenasin){
    int i = 0;
    int j = 0;
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



/*
La funcion convierte una cadena en minuscula a mayusculas

Parametros :
   char *cadena : puntero tipo char que apunta a la cadena a modificar
   
Retorno :
   Nada, ya que es tipo void
 
*/
void convertirMayusculas(char *cadena){
    for (int i = 0; cadena[i]; i++) {
        cadena[i] = toupper(cadena[i]);
    }
}



//OPTIMIZACION
/*
La funcion remplaza la funcion strstr pa

Parametros :
   const char *cadena1 : puntero tipo char que apunta a la linea de la sopa de letras
   const char *cadena2 : puntero tipo char que apunta al nombre a buscar en la linea
   
Retorno :
   Retona NULL si no esta o la linea donde aparecio la palabra
 
*/
char *strstrmodificada(const char *cadena1, const char *cadena2) {
    if (*cadena2 == '\0') {
        return (char *)cadena1;
    }
    while (*cadena1) {
        const char *h = cadena1;
        const char *n = cadena2;
        while (*n && (*h == *n)) {
            h++;
            n++;
        }
        if (*n == '\0') {
            return (char *)cadena1;
        }
        cadena1++;
    }
    return NULL;
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
void Solucion(FILE *archivo, char*nom_archivo, char *nom_sopa, char *orientation, int tam){
    char rutaActual[FILENAME_MAX]; //Tamaño suficiente para almacenar la ruta
    if (getcwd(rutaActual, sizeof(rutaActual)) == NULL) {
        perror("Error al obtener la ruta actual");
    }
    int fila = 0;

    //Medicion tiempo
    double time_spent = 0.0;
    clock_t begin = clock();

    bool found = false;  //Flag
    char bufer[500];  //Se guarda cada linea del archivo

    //Se transforma el nombre de la sopa en mayusculas
    convertirMayusculas(nom_sopa);

    if(strcasecmp(orientation, "horizontal") == 0) {
        //Horizontal
        tam = (tam+1)/2; //Dimension real
        char cadenafinal[500];

        while(fgets(bufer, sizeof(bufer), archivo)){
            quitaespacios(bufer, cadenafinal);
            //Para probar la optimizacion, reemplace strstr(cadenafinal, nom_sopa) != NULL por strstrmodificada(columna, nom_sopa) != NULL
            if(strstr(cadenafinal, nom_sopa) != NULL){
                //Se calcula el tiempo de ejecucion
                clock_t end = clock();
                time_spent += (double)(end - begin) / CLOCKS_PER_SEC;

                //Verificacion de la busqueda
                printf("ESTADO: Palabra encontrada en la fila %d.\n",fila+2);
                printf("Tiempo que transcurrido: %f\n",time_spent);
                found = true;
                moverArchivo(nom_archivo,orientation ,tam);
                printf("------------------------------------------------------------------------------------------\n");
                printf("\n");
                printf("\n");              
                break;
            }
            fila++;
        }

        if(found == false){
            clock_t end = clock();
            time_spent += (double)(end - begin) / CLOCKS_PER_SEC;

            //Verificacion de la busqueda
            printf("ESTADO: Palabra NO encontrada.\n");
            printf("Tiempo que transcurrido: %f\n",time_spent);
            printf("------------------------------------------------------------------------------------------\n");
            printf("\n");
            printf("\n");
        }

    }else if(strcasecmp(orientation, "vertical") == 0){
        //Vertical
        tam = (tam+1)/2; //Dimension real
        char **matriz = malloc(sizeof(char *) * tam);

        //Se guarda el contenido del archivo en una estructura
        for(int i = 0; i < tam; i++) {
            matriz[i] = malloc(sizeof(char) * tam); //Asigna memoria para cada fila
            for(int j = 0; j < tam; j++) {
                fscanf(archivo, " %c", &matriz[i][j]);
            }
        }
        
        //Se calcula la traspuesta a la matriz
        for(int i = 0; i < tam; i++) {
            for(int j = i + 1; j < tam; j++) {
                char temp = matriz[i][j];
                matriz[i][j] = matriz[j][i];
                matriz[j][i] = temp;
            }
        }

        //Se busca la palabra en cada linea de la matriz traspuesta
        for(int i = 0; i < tam; i++){
            char columna[500];
            for(int j = 0; j < tam; j++) {
                columna[j] = matriz[i][j];
            }
            columna[tam] = '\0';
            //Para probar la optimizacion, reemplace strstr(cadenafinal, nom_sopa) != NULL por strstrmodificada(columna, nom_sopa) != NULL
            if(strstr(columna, nom_sopa) != NULL){
                clock_t end = clock();
                time_spent += (double)(end - begin) / CLOCKS_PER_SEC;
                
                //Verificacion de la busqueda
                printf("ESTADO: Palabra encontrada en la columna %d.\n",i+2);
                printf("Tiempo que transcurrido: %f\n",time_spent);
                found = true;
                moverArchivo(nom_archivo,orientation ,tam);
                printf("------------------------------------------------------------------------------------------\n");
                printf("\n");
                printf("\n");
                break;
            }
        }
        if(found == false){
            clock_t end = clock();
            time_spent += (double)(end - begin) / CLOCKS_PER_SEC;

            //Verificacion de la busqueda
            printf("ESTADO: Palabra NO encontrada.\n");
            printf("Tiempo que transcurrido: %f\n",time_spent);
            printf("------------------------------------------------------------------------------------------\n");
            printf("\n");
            printf("\n");
            
        }

        //Se libera la matriz para evitar leaks en la memoria
        for(int i = 0; i < tam; i++) {
            free(matriz[i]);
        }
        free(matriz);

    }else{
        printf("Orientación inválida: %s\n", orientation);
        return;
    }

}



/*
La funcion a partir del nombre del archivo extrae la orientacion y la dimension de la sopa

Parametros :
   char* nom_archivo : puntero tipo char que apunta al nombre del archivo a leer 
   
Retorno :
   Nada, ya que es tipo void
 
*/
void Orientacion(char* nom_archivo){
    //Tamaño suficiente para almacenar la palabra extraída
    char nom_archivo2[25]; 
    
    //Leo el archivo para calcular la longitud de la linea
    FILE* archivo = fopen(nom_archivo, "r"); 
    char bufer[500];
    fgets(bufer, 500, archivo); //Se salta la primea linea
    int longitud = strlen(fgets(bufer, 500, archivo));
    fclose(archivo);

    //Abro el archivo para extraer la sopa
    FILE* archivolectura = fopen(nom_archivo, "r");
    char linea_arch[500];

    //Leer la primera línea para obtener la orientación
    if(fgets(linea_arch, 500, archivolectura)){
        linea_arch[strcspn(linea_arch, "\n")] ='\0'; 
        if(strcasecmp(linea_arch, "vertical") == 0 || strcasecmp(linea_arch, "horizontal") == 0){

            //Resolvemos la sopa de letras
            strncpy(nom_archivo2, nom_archivo, strlen(nom_archivo) - 4); //Restamos 4 para eliminar la extensión ".txt"
            nom_archivo2[strlen(nom_archivo) - 4] = '\0'; 

            //Verificacion de la busqueda
            printf("------------------------------------------------------------------------------------------\n");
            printf("Nombre del Archivo: %s\n", nom_archivo);
            printf("------------------------------------------------------------------------------------------\n");
            printf("Orientacion: %s\n", linea_arch);
            printf("Tamaño: %d x %d\n",(longitud+1)/2, (longitud+1)/2);

            //Buscamos la palabra en la sopa
            Solucion(archivolectura, nom_archivo,nom_archivo2, linea_arch, longitud);
            fclose(archivolectura);

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
void LeerDir(){
    DIR *d = opendir(".");
    struct dirent *dentry;
    size_t i = 1;
    
    while((dentry = readdir(d)) != NULL) {
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