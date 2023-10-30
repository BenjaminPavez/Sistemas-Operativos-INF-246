import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;

public class LeerArchivosDeDirectorio {
    public static void main(String[] args) {
        // Especifica la ruta del directorio que deseas explorar
        String directorioRaiz = "./";

        // Llama a la función para explorar el directorio
        explorarDirectorio(new File(directorioRaiz));
    }

    public static void explorarDirectorio(File directorio) {
        // Verifica si el objeto "directorio" es realmente un directorio
        if (directorio.isDirectory()) {
            // Lista de archivos y subdirectorios en el directorio
            File[] archivosYDirectorios = directorio.listFiles();
            if (archivosYDirectorios != null) {
                for (File archivoODirectorio : archivosYDirectorios) {
                    // Llama recursivamente a la función para explorar subdirectorios
                    explorarDirectorio(archivoODirectorio);
                }
            }
        } else if (directorio.isFile()) {
            // Si es un archivo, verifica si es un archivo de texto (puedes ajustar la extensión)
            if (directorio.getName().toLowerCase().endsWith(".txt")) {
                // Lee y muestra el contenido del archivo de texto
                leerYMostrarContenido(directorio);
            }
        }
    }

    public static void leerYMostrarContenido(File archivo) {
        try {
            BufferedReader lector = new BufferedReader(new FileReader(archivo));
            String linea;
            System.out.println("Contenido de " + archivo.getName() + ":");
            while ((linea = lector.readLine()) != null) {
                System.out.println(linea);
            }
            lector.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}