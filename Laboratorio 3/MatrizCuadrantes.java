public class MatrizCuadrantes{
    public static void main(String[] args) {
        int[][] matriz = {
            {1, 2, 3, 4, 17 , 18},
            {5, 6, 7, 8, 19, 20},
            {9, 10, 11, 12,21,22},
            {13, 14, 15, 16,23,24},
            {25, 26, 27, 28,29,30},
            {25, 26, 27, 28,29,30}
        };

        int filas = matriz.length;
        int columnas = matriz[0].length;

        if (filas % 2 != 0 || columnas % 2 != 0) {
            System.out.println("La matriz no se puede dividir en 4 cuadrantes.");
            return;
        }

        int mitadFilas = filas / 2;
        int mitadColumnas = columnas / 2;

        // Cuadrante superior izquierdo
        int[][] cuadranteSuperiorIzquierdo = new int[mitadFilas][mitadColumnas];
        for (int i = 0; i < mitadFilas; i++) {
            for (int j = 0; j < mitadColumnas; j++) {
                cuadranteSuperiorIzquierdo[i][j] = matriz[i][j];
            }
        }

        // Cuadrante superior derecho
        int[][] cuadranteSuperiorDerecho = new int[mitadFilas][mitadColumnas];
        for (int i = 0; i < mitadFilas; i++) {
            for (int j = mitadColumnas; j < columnas; j++) {
                cuadranteSuperiorDerecho[i][j - mitadColumnas] = matriz[i][j];
            }
        }

        // Cuadrante inferior izquierdo
        int[][] cuadranteInferiorIzquierdo = new int[mitadFilas][mitadColumnas];
        for (int i = mitadFilas; i < filas; i++) {
            for (int j = 0; j < mitadColumnas; j++) {
                cuadranteInferiorIzquierdo[i - mitadFilas][j] = matriz[i][j];
            }
        }

        // Cuadrante inferior derecho
        int[][] cuadranteInferiorDerecho = new int[mitadFilas][mitadColumnas];
        for (int i = mitadFilas; i < filas; i++) {
            for (int j = mitadColumnas; j < columnas; j++) {
                cuadranteInferiorDerecho[i - mitadFilas][j - mitadColumnas] = matriz[i][j];
            }
        }

        // Imprimir los cuadrantes
        imprimirMatriz(cuadranteSuperiorIzquierdo);
        imprimirMatriz(cuadranteSuperiorDerecho);
        imprimirMatriz(cuadranteInferiorIzquierdo);
        imprimirMatriz(cuadranteInferiorDerecho);
    }

    public static void imprimirMatriz(int[][] matriz) {
        for (int i = 0; i < matriz.length; i++) {
            for (int j = 0; j < matriz[i].length; j++) {
                System.out.print(matriz[i][j] + " ");
            }
            System.out.println();
        }
        System.out.println();
    }
}
 
