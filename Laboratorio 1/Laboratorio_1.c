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
void Orientacion(char* nom_archivo) {
   printf("----------------------------------------------------------------\n");
   printf("Nombre del Archivo: %s\n", nom_archivo);
   printf("----------------------------------------------------------------\n");
   FILE* archivo = fopen(nom_archivo, "r"); // Modo lectura
   char bufer[1000]; // Aquí vamos a ir almacenando cada línea
   // Leer la primera línea para obtener la orientación
   if(fgets(bufer, 1000, archivo)) {
      bufer[strcspn(bufer, "\n")] ='\0'; 
      if(strcmp(bufer, "vertical") == 0 || strcmp(bufer, "horizontal") == 0){
         printf("Orientacion: %s\n", bufer);
         // Leer la segunda línea para obtener el tamaño del lado
         if (fgets(bufer, 1000, archivo)) {
               int longitud = strlen(bufer);
               printf("Tamaño: %d x %d\n",(longitud+1)/2, (longitud+1)/2);
               printf("----------------------------------------------------------------\n");
         }
      }else {
         printf("Error: %s\n", bufer);
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
