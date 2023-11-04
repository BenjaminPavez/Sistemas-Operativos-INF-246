import java.util.concurrent.RecursiveTask;
import java.util.concurrent.ForkJoinPool;

public class Forks {
    public static void main(String[] args, char[][] matriz1, char[][] matriz2, char[][] matriz3, char[][] matriz4, String palabra) {
        ForkJoinPool pool = new ForkJoinPool();

        MatrizSearchTask tarea1 = new MatrizSearchTask(matriz1, palabra);
        MatrizSearchTask tarea2 = new MatrizSearchTask(matriz2, palabra);
        MatrizSearchTask tarea3 = new MatrizSearchTask(matriz3, palabra);
        MatrizSearchTask tarea4 = new MatrizSearchTask(matriz4, palabra);

        int resultado1 = pool.invoke(tarea1);
        int resultado2 = pool.invoke(tarea2);
        int resultado3 = pool.invoke(tarea3);
        int resultado4 = pool.invoke(tarea4);

        int resultadoTotal = resultado1 + resultado2 + resultado3 + resultado4;

        System.out.println("La palabra '" + palabra + "' se encontró " + resultadoTotal + " veces en las matrices.");
    }
}

class MatrizSearchTask extends RecursiveTask<Integer>{
    private char[][] matriz;
    private String palabra;
    private static final long serialVersionUID = 10000000;  //Para que no salga el warning

    public MatrizSearchTask(char[][] matriz, String palabra){
        this.matriz = matriz;
        this.palabra = palabra;
    }

    @Override
    protected Integer compute(){
        try {
            // Pausa la ejecución durante 5 segundos (5000 milisegundos)
            Thread.sleep(5000);
        } catch (InterruptedException e) {
            // Manejo de excepciones si es necesario
            e.printStackTrace();
        }
        if(matriz[0].length/2 == palabra.length()){
            Find busqueda = new Find();
            busqueda.FindWord(matriz, palabra);
            return 1;
            
        }else{
            ReadDir.SeparateQuadrants(matriz, palabra);
            System.out.println("CREO MAS PROCESOS");
        }  
        return 0;
    }
}