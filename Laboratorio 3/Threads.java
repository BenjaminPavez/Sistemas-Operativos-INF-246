
//PRUEBAS
public class Threads{
    public static void main(String[] args, char[][] matriz1, char[][] matriz2, char[][] matriz3, char[][] matriz4, int tam) {
        RunnableTask task1 = new RunnableTask(matriz1,tam);
        Thread thread1 = new Thread(task1);
        RunnableTask task2 = new RunnableTask(matriz2,tam);
        Thread thread2 = new Thread(task2);
        //RunnableTask task3 = new RunnableTask(matriz3,tam);
        //Thread thread3 = new Thread(task3);
        //RunnableTask task4 = new RunnableTask(matriz4,tam);
        //Thread thread4 = new Thread(task4);
      

        thread1.start();
        thread2.start();
        //thread3.start();
        //thread4.start();
    }
}

class RunnableTask implements Runnable{
    private char[][] Matriz;
    private int tam_palabra;

    public RunnableTask(char[][] matriz, int taman) {  //char[][] matriz
        this.Matriz = matriz;
        this.tam_palabra = taman;
    }

    @Override
    public void run() {
        try{
            Thread.sleep(10000); // Simula cierta actividad
        }catch(InterruptedException e) {
            e.printStackTrace();
        }
        if(Matriz[0].length/2 == tam_palabra){
            try{
                Thread.sleep(10000); // Simula cierta actividad
            }catch(InterruptedException e) {
                e.printStackTrace();
            }
            System.out.println("No hay matriz");
        }else{
            ReadDir.SeparateQuadrants(Matriz, tam_palabra);
            try{
                Thread.sleep(10000); // Simula cierta actividad
            }catch(InterruptedException e) {
                e.printStackTrace();
            }
        }  
    }
}
