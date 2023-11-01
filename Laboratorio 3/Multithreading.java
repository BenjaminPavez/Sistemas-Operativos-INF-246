public class Multithreading extends Thread{
    public void Multithreading(){};

    public void run(){
        ReadDir dir = new ReadDir();
        dir.main();
    } 

}