import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;


public class ReadDir{
    public static void main(String[] args){
        String directorioRaiz = "./ESTE";
        ViewDir(new File(directorioRaiz));
    }


    /**
     * Se encarga de explorar un directorio y sus subdirectorios
     *
     * File directorio: directorio a explorar
     *
     * Al ser una funcion void no retorna nada
    **/
    public static void ViewDir(File directorio) {
        System.out.println("Directorio actual: " + directorio.getName());
        if (directorio.isDirectory()) {
            File[] archivos = directorio.listFiles();
            if (archivos != null) {
                for (File archivo : archivos) {
                    if (archivo.isFile() && archivo.getName().toLowerCase().endsWith(".txt")) {
                        ViewMatrix(archivo);
                    }
                }
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
            SeparateQuadrants(matriz, palabra.length());
            lector.close();
        }catch(IOException e){
            e.printStackTrace();
        }
    }

    
    public static void SeparateQuadrants(char[][] matriz, int tamano){
        int size = matriz[0].length+1;
        int halfSize = size / 2;
        System.out.println("LA MITAD ES: " + halfSize);

        if(tamano == size){
            System.out.println("BUSCAMOS LA PALABRA"); //Busca la palabra
            return;
        }else{
            char [][] matriz1 = new char[halfSize][halfSize];
            System.out.println("Cuadrante superior izquierdo:");
            for (int i = 0; i < halfSize/2; i++) {
                for (int j = 0; j < halfSize; j++) {
                    matriz1[i][j] = matriz[i][j];
                    System.out.print(matriz1[i][j]);
                }
                System.out.println();
            }
            
    
            char [][] matriz2 = new char[halfSize][halfSize];
            System.out.println("Cuadrante inferior izquierdo:");
            for (int i = halfSize/2; i < halfSize; i++){
                for(int j = 0; j < halfSize; j++) {
                    matriz2[i-(halfSize/2)][j] = matriz[i][j];
                    System.out.print(matriz2[i-(halfSize/2)][j]);
                }
                System.out.println();
            }
         

            char [][] matriz3 = new char[halfSize][halfSize];
            System.out.println("Cuadrante superior derecho:");
            for (int m = 0; m < halfSize/2; m++) {
                for (int n = halfSize; n < size-1; n++) {
                    matriz3[m][n-halfSize] = matriz[m][n];
                    System.out.print(matriz3[m][n-halfSize]);
                }
                System.out.println();
            }

        
            char [][] matriz4 = new char[halfSize][halfSize];
            System.out.println("Cuadrante inferior derecho:");
            for (int i = halfSize/2; i < halfSize; i++) {
                for (int j = halfSize; j < size-1; j++) { // Corregir el rango aquí
                    matriz4[i-(halfSize/2)][j-halfSize] = matriz[i][j];
                    System.out.print(matriz4[i-(halfSize/2)][j-halfSize]);
                }
                System.out.println();
            }

            Threads.main(null, matriz1, matriz2, matriz3, matriz4, tamano);

        }
    }
}
