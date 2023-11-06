public class Find{
    static char[][] matOG;
    public static int send(ReadDir matrizOG){
        matOG = matrizOG.get();
        return 0;
        
    }   

    public void FindWord(char[][] Matriz, String PalBuscar, int op){
        //Si op es 0 buscamos en la matriz entera y si es 1 buscamos en la submatriz para threads y forks
        if(op == 0){
            //HORIZONTAL
            Main tiempo = new Main();
            tiempo.iniciarTiempo();
            long tiempoInicial = tiempo.obtenerTiempo();

            StringBuilder resultado = new StringBuilder();
            for (int i = 0; i < PalBuscar.length(); i++) {
                resultado.append(PalBuscar.charAt(i));
                if (i < PalBuscar.length() - 1) {
                    resultado.append(' ');
                }
            }
            String cadenaConEspacios = resultado.toString();
            for (int m = 0; m < Matriz[0].length/2; m++) {
                String str = new String(Matriz[m]);
                //Verificamos si la palabra esta horizontal en cada submatriz
                if(str.contains(cadenaConEspacios)){
                    long tiempoFinal = System.nanoTime();
                    long tiempoTotal = tiempoFinal - tiempoInicial;
                    System.out.println("Se demoro: "+tiempoTotal+" [nanosegundos]");
                    System.out.println("Encontramos la palabra en Fila: "+m+", Columna: "+(str.indexOf(cadenaConEspacios)/2));
                    System.out.println("------------------------------------------------------------------------------");
                    System.exit(0);
                }
            }
            //VERTICAL
            int numFila = Matriz.length;
            int numCols = Matriz[0].length;
        
            for(int col = 0; col < numCols; col++){
                StringBuilder palabraVertical = new StringBuilder();
                for(int row = 0; row < numFila; row++) {
                    palabraVertical.append(Matriz[row][col]);
                }
                String palabra = palabraVertical.toString();

                //Compara con la palabra a buscar
                if (palabra.contains(PalBuscar)){
                    long tiempoFinal = System.nanoTime();
                    long tiempoTotal = tiempoFinal - tiempoInicial;
                    System.out.println("Se demoro: "+tiempoTotal+" [nanosegundos]");
                    System.out.println("Encontramos la palabra en Fila: "+palabra.indexOf(PalBuscar)+", Columna: "+col/2);
                    System.out.println("------------------------------------------------------------------------------");
                    System.exit(0);
                }
            }

        }else if(op == 1){
            //HORIZONTAL
            Main tiempo = new Main();
            tiempo.iniciarTiempo();
            long tiempoInicial = tiempo.obtenerTiempo();

            StringBuilder resultado = new StringBuilder();
            for (int i = 0; i < PalBuscar.length(); i++) {
                resultado.append(PalBuscar.charAt(i));
                if (i < PalBuscar.length() - 1) {
                    resultado.append(' ');
                }
            }
            String cadenaConEspacios = resultado.toString();
            for (int m = 0; m < Matriz[0].length/2; m++) {
                String str = new String(Matriz[m]);
                //Verificamos si la palabra esta horizontal en cada submatriz
                if(str.contains(cadenaConEspacios)){
                    long tiempoFinal = System.nanoTime();
                    long tiempoTotal = tiempoFinal - tiempoInicial;
                    System.out.println("Se demoro: "+tiempoTotal+" [nanosegundos]");

                    subMatriz(matOG, Matriz, 1);
                    System.out.println("------------------------------------------------------------------------------");
                    System.exit(0);
                }
            }
            //VERTICAL
            int numFila = Matriz.length;
            int numCols = Matriz[0].length;
            for(int col = 0; col < numCols; col++){
                StringBuilder palabraVertical = new StringBuilder();
                for(int row = 0; row < numFila; row++) {
                    palabraVertical.append(Matriz[row][col]);
                }
                String palabra = palabraVertical.toString();

                //Compara con la palabra a buscar
                if (palabra.contains(PalBuscar)){
                    long tiempoFinal = System.nanoTime();
                    long tiempoTotal = tiempoFinal - tiempoInicial;
                    System.out.println("Se demoro: "+tiempoTotal+" [nanosegundos]");

                    subMatriz(matOG, Matriz, col);
                    System.out.println("------------------------------------------------------------------------------");
                    System.exit(0);
                }
            }
        }
    }

    public static void subMatriz(char[][] matrix, char[][] submatrix, int col) {
        int numFila = matrix.length;
        int numCols = matrix[0].length;
        int subFila = submatrix.length/2;
        int subCol = submatrix[0].length-1;
        
        for (int i = 0; i <= numFila - subFila; i++) {
            for (int j = 0; j <= numCols - subCol; j++) {
                boolean found = true;
                for (int x = 0; x < subFila; x++) {
                    for (int y = 0; y < subCol; y++) {
                        if (matrix[i + x][j + y] != submatrix[x][y]) {
                            found = false;
                            break;
                        }
                    }
                    if (!found) {
                        break;
                    }
                }
                if (found) {
                    System.out.println("Encontramos la palabra en Fila: "+i+", Columna: "+(j/2+col/2));
                }
            }
        }
    }
}