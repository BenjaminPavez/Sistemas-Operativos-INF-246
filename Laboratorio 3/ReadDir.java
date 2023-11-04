import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;


public class ReadDir{
    public static void main(String[] args){
        String directorioRaiz = "./CHECOESLOVAQUIA";
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
            SeparateQuadrants(matriz, palabra);
            lector.close();
        }catch(IOException e){
            e.printStackTrace();
        }
    }

    
    public static void SeparateQuadrants(char[][] matriz, String palabra){
        int size = matriz[0].length+1;
        int halfSize = size / 2;

        char [][] matriz1 = new char[halfSize][halfSize];
        for (int i = 0; i < halfSize/2; i++) {
            for (int j = 0; j < halfSize; j++) {
                matriz1[i][j] = matriz[i][j];
            }
        }
        

        char [][] matriz2 = new char[halfSize][halfSize];
        for (int i = halfSize/2; i < halfSize; i++){
            for(int j = 0; j < halfSize; j++) {
                matriz2[i-(halfSize/2)][j] = matriz[i][j];
            }
        }
        

        char [][] matriz3 = new char[halfSize][halfSize];
        for (int m = 0; m < halfSize/2; m++) {
            for (int n = halfSize; n < size-1; n++) {
                matriz3[m][n-halfSize] = matriz[m][n];
            }
        }

    
        char [][] matriz4 = new char[halfSize][halfSize];
        for (int i = halfSize/2; i < halfSize; i++) {
            for (int j = halfSize; j < size-1; j++) {
                matriz4[i-(halfSize/2)][j-halfSize] = matriz[i][j];
            } 
        }

        //Se descomenta el metodo q se desee ejecutar

        //Metodo 1: Sin Threads ni Forks
        //Find busqueda = new Find();
        //busqueda.FindWord(matriz, palabra);

        //Metodo 2: Con Threads
        //Multithreading.main(null, matriz1, matriz2, matriz3, matriz4, palabra);

        //Metodo 3: Con Forks
        Forks.main(null, matriz1, matriz2, matriz3, matriz4, palabra);

        
    }
}