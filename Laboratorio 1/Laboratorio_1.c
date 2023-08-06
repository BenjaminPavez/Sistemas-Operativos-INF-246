#include<stdio.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<dirent.h>
#include<time.h>
#include<string.h>



/*
La funcion a partir del nombre del archivo extrae la informacion necesaria

Parametros :
   char* nom_archivo : puntero tipo char que apunta al nombre del archivo a leer 
   
Retorno :
   Nada, ya que es tipo void
 
*/
void Orientacion(char*nom_archivo){
   printf("----------------------------------------------------------------\n");
   printf("Nombre del Archivo: %s\n",nom_archivo);
   printf("----------------------------------------------------------------\n");
   FILE *archivo = fopen(nom_archivo, "r"); // Modo lectura
   char bufer[1000];         // Aquí vamos a ir almacenando cada línea
   while (fgets(bufer, 1000, archivo)){
      if(strcmp(bufer, "vertical") == 0){
         fgets(bufer, 1000, archivo);
         int longitud = (strlen(bufer)-1);
         printf("Orientacion: Vertical\n");
         printf("Numero de Filas: %i\n",longitud);
         printf("Numero de Columnas: %i\n",longitud);
         printf("----------------------------------------------------------------\n");
         printf("\n");
         printf("\n");
         break;
      }else if((strcmp(bufer, "horizontal") == 0)){
         fgets(bufer, 1000, archivo);
         int longitud = (strlen(bufer)-1);
         printf("Orientacion: Horizontal\n");
         printf("Numero de Filas: %i\n",longitud);
         printf("Numero de Columnas: %i\n",longitud);
         printf("----------------------------------------------------------------\n");
         printf("\n");
         printf("\n");
         break;
      }else{
         printf("Error: %s\n",bufer);
         break;
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
         //printf("%lu. %s\n", i, dentry->d_name);
         Orientacion(dentry->d_name);
         i++;
      }
   }

   closedir(d);
}



//Esta funcion quizas se borra para que no solo haga una funcion
/*
La funcion crear los directorios necesarios para guardar las sopas de letras

Parametros :
   int tipo : entero que representa si es de tipo horizontal o vertical

Retorno :
   Nada, ya que es tipo void
 
*/
void CrearDir(int tipo){
   if(tipo == 0){
      mkdir("./horizontal", S_IRWXU);
      printf("Horizontal\n");
   }else{
      mkdir("./vertical", S_IRWXU);
      printf("Vertical\n");
   }

}




/*
La funcion Main

Parametros :
   No recibe ningun parametro

Retorno :
   retorna 0 si se ejecuto correctamente
 
*/
int main(){
   printf("Hola Mundo CTM\n");
   LeerDir();
   return 0;

}
