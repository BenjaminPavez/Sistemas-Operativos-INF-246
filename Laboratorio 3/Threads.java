public class Threads {
    public static void main(String[] args) {
        // Crear instancias de las clases que implementan Runnable
        RunnableTask task1 = new RunnableTask("Tarea 1");
        RunnableTask task2 = new RunnableTask("Tarea 2");

        // Crear objetos Thread y asignarles las tareas
        Thread thread1 = new Thread(task1);
        Thread thread2 = new Thread(task2);

        // Iniciar los threads
        thread1.start();
        thread2.start();
    }
}

class RunnableTask implements Runnable {
    private String taskName;

    public RunnableTask(String name) {
        this.taskName = name;
    }

    @Override
    public void run() {
        for (int i = 1; i <= 5; i++) {
            System.out.println(taskName + " está en el paso " + i);
            try {
                Thread.sleep(1000); // Simula cierta actividad
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
        System.out.println(taskName + " ha terminado.");
    }
}
