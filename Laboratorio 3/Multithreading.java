public class Multithreading{
    public static void main(String[] args, char[][] matriz1, char[][] matriz2, char[][] matriz3, char[][] matriz4, String palabra){

        RunnableTask task1 = new RunnableTask(matriz1, palabra);
        Thread thread1 = new Thread(task1);
        RunnableTask task2 = new RunnableTask(matriz2, palabra);
        Thread thread2 = new Thread(task2);
        RunnableTask task3 = new RunnableTask(matriz3, palabra);
        Thread thread3 = new Thread(task3);
        RunnableTask task4 = new RunnableTask(matriz4, palabra);
        Thread thread4 = new Thread(task4);
      

        thread1.start();
        thread2.start();
        thread3.start();
        thread4.start();
    }
}

class RunnableTask implements Runnable{
    private char[][] Matriz;
    private String Palabra;

    public RunnableTask(char[][] matriz, String pala) {  //char[][] matriz
        this.Matriz = matriz;
        this.Palabra = pala;
    }

    @Override
    public void run() {
        try{
            Thread.sleep(1000); // Simula cierta actividad
        }catch(InterruptedException e) {
            e.printStackTrace();
        }
        if(Matriz[0].length/2 == Palabra.length()){
            Busqueda busqueda = new Busqueda();
            busqueda.Find(Matriz, Palabra);
            
        }else{
            ReadDir.SeparateQuadrants(Matriz, Palabra);
            try{
                Thread.sleep(1000); // Simula cierta actividad
            }catch(InterruptedException e) {
                e.printStackTrace();
            }
        }  
    }
}
