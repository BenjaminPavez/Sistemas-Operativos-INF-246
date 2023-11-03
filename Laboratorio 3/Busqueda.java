public class Busqueda {
    public void Find(char[][] Matriz, String PalBuscar){
        //HORIZONTAL
        Main tiempo = new Main();
        tiempo.iniciarTiempo();
        long tiempoInicial = tiempo.obtenerTiempo();

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

                long tiempoFinal = System.nanoTime();
                long tiempoTotal = tiempoFinal - tiempoInicial;
                System.out.println(tiempoTotal);
                System.out.println("Encontramos la palabra: "+str+" en la fila "+m+" de la submatriz: "); 
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

                long tiempoFinal = System.nanoTime();
                long tiempoTotal = tiempoFinal - tiempoInicial;
                System.out.println(tiempoTotal);
                System.out.println("Encontramos la palabra: "+PalBuscar+" en la columna "+col+" de la submatriz: ");
            }
        }
    }
}
