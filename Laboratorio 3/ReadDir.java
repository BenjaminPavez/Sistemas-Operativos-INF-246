import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;


public class ReadDir{
    public static void main(String[] args){
        String directorioRaiz = "./";

        ViewDir(new File(directorioRaiz));
    }


    /**
     * Se encarga de explorar un directorio y sus subdirectorios
     *
     * File directorio: directorio a explorar
     *
     * Al ser una funcion void no retorna nada
    **/
    public static void ViewDir(File directorio){
        if(directorio.isDirectory()){
            File[] archivosYDirectorios = directorio.listFiles();
            if(archivosYDirectorios != null){
                for(File archivoODirectorio : archivosYDirectorios){
                    ViewDir(archivoODirectorio);
                }
            }
        }else if(directorio.isFile()){
            if(directorio.getName().toLowerCase().endsWith(".txt")){
                ViewMatrix(directorio);
            }
        }
    }


    /**
     * Se encarga de leer y mostrar la matriz de un archivo
     *
     * File archivo: archivo a leer
     *
     * Al ser una funcion void no retorna nada
    **/
    public static void ViewMatrix(File archivo){
        try{
            BufferedReader lector = new BufferedReader(new FileReader(archivo));
            String linea;
            System.out.println("Contenido de " + archivo.getName() + ":");

            //Datos archivo
            int tam = Integer.parseInt(lector.readLine());
            System.out.println("Tamaño matriz: " + tam + "x" + tam);
            String palabra = lector.readLine();
            System.out.println("Palabra: " + palabra);
            //Fin datos archivo

            char [][] matriz = new char[tam][tam];
            int i = 0;
            while((linea = lector.readLine()) != null){
                matriz[i] = linea.toCharArray();
                i++;
            }
            System.out.println(i + " lineas leidas.");
            SeparateQuadrants(matriz);
            lector.close();
        }catch(IOException e){
            e.printStackTrace();
        }
    }

    
    public static void SeparateQuadrants(char[][] matriz) {
        int size = matriz[0].length+1;
        int halfSize = size / 2;
        System.out.println("LA MITAD ES: " + halfSize);


        //FUNCIONA BIEN
        System.out.println("Cuadrante superior izquierdo:");
        for (int i = 0; i < halfSize/2; i++) {
            for (int j = 0; j < halfSize; j++) {
                System.out.print(matriz[i][j]);
            }
            System.out.println();
        }
        

        System.out.println("Cuadrante inferior izquierdo:");
        for (int i = halfSize/2; i < halfSize; i++){
            for(int j = 0; j < halfSize; j++) {
                System.out.print(matriz[i][j]);
            }
            System.out.println();
        }


        //FUNCIONA BIEN
        System.out.println("Cuadrante superior derecho:");
        for (int m = 0; m < halfSize/2; m++) {
            for (int n = halfSize; n < size; n++) {
                System.out.print(matriz[m][n]);
            }
            System.out.println();
        }
    

        //FUNCIONA BIEN MAS O MENOS REVISAR
        System.out.println("Cuadrante inferior derecho:");
        for (int i = halfSize/2; i < halfSize; i++) {
            for (int j = halfSize; j < size; j++) { // Corregir el rango aquí
                System.out.print(matriz[i][j]);
            }
            System.out.println();
        }
        
    }
}

