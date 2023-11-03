import java.util.concurrent.RecursiveTask;
import java.util.concurrent.ForkJoinPool;

public class MultithreadingWithForks {
    public static void main(String[] args, char[][] matriz1, char[][] matriz2, char[][] matriz3, char[][] matriz4, String palabra) {

        ForkJoinPool pool = new ForkJoinPool();

        BusquedaTask task1 = new BusquedaTask(matriz1, palabra);
        // BusquedaTask task2 = new BusquedaTask(matriz2, palabra);
        // BusquedaTask task3 = new BusquedaTask(matriz3, palabra);
        // BusquedaTask task4 = new BusquedaTask(matriz4, palabra);

        Integer resultado1 = pool.invoke(task1);
        // Integer resultado2 = pool.invoke(task2);
        // Integer resultado3 = pool.invoke(task3);
        // Integer resultado4 = pool.invoke(task4);

        // Realiza acciones con los resultados si es necesario
    }
}

class BusquedaTask extends RecursiveTask<Integer> {
    private char[][] Matriz;
    private String Palabra;

    public BusquedaTask(char[][] matriz, String palabra) {
        this.Matriz = matriz;
        this.Palabra = palabra;
    }

    @Override
    protected Integer compute() {
        try {
            Thread.sleep(100000); // Simula cierta actividad
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        if (Matriz[0].length / 2 == Palabra.length()) {
            Busqueda busqueda = new Busqueda();
            busqueda.Find(Matriz, Palabra);
            return 0;
        } else {
            ReadDir.SeparateQuadrants(Matriz, Palabra);
            return 0;
        }
    }
}
