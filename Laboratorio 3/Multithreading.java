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
            Find(Matriz, Palabra);
            
        }else{
            ReadDir.SeparateQuadrants(Matriz, Palabra);
            try{
                Thread.sleep(1000); // Simula cierta actividad
            }catch(InterruptedException e) {
                e.printStackTrace();
            }
        }  
    }

    public void Find(char[][] Matriz, String PalBuscar){
        //HORIZONTAL
        StringBuilder resultado = new StringBuilder();
        for (int i = 0; i < PalBuscar.length(); i++) {
            resultado.append(PalBuscar.charAt(i));
            if (i < PalBuscar.length() - 1) {
                resultado.append(' '); // Agrega un espacio en blanco después de cada carácter, excepto el último.
            }
        }
        String cadenaConEspacios = resultado.toString();
        for (int m = 0; m < Matriz[0].length; m++) {
            String str = new String(Matriz[m]);
            //Verificamos si la palabra esta horizontal en cada submatriz
            if(str.contains(cadenaConEspacios)){
                System.out.println("Encontramos la palabra: "+str+" en la fila "+m+" de la submatriz: ");
                for (int i = 0; i < Matriz.length; i++) {
                    for (int j = 0; j < Matriz[0].length; j++) {
                        System.out.print(Matriz[i][j]);
                    }
                    System.out.println();
                }   
            }
        }
        //VERTICAL
        int numRows = Matriz.length;
        int numCols = Matriz[0].length; // El número de columnas debe ser el número de columnas en la matriz, no en las filas.

        for (int col = 0; col < numCols; col++) {
            StringBuilder palabraVertical = new StringBuilder();
            for (int row = 0; row < numRows; row++) {
                palabraVertical.append(Matriz[row][col]);
            }
            String palabra = palabraVertical.toString();
            // Compara con la palabra a buscar
            if (palabra.contains(PalBuscar)) {
                System.out.println("Encontramos la palabra: "+PalBuscar+" en la columna "+col+" de la submatriz: ");
                for (int i = 0; i < Matriz.length; i++) {
                    for (int j = 0; j < Matriz[0].length; j++) {
                        System.out.print(Matriz[i][j]);
                    }
                    System.out.println();
                }
            }
        }
    }
}
