public class Main {
    private long tiempoInicio;

    public void iniciarTiempo(){
        tiempoInicio = System.nanoTime();

    }

    public long obtenerTiempo(){
        return tiempoInicio;
    }
    public static void main(String[] args) {
        System.out.println("----------------------------------Inicio----------------------------------");
        ReadDir.main(args);
    }
}
