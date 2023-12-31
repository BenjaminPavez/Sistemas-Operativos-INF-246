import threading
import time
from datetime import datetime
import random

#Mutex para el ingreso a PdLamparas
mutex_Pdl = threading.Lock()


#Clase Departamento que simula un departamento de la universidad
class Departamento:
    #Constructor
    def __init__(self, nombre, capacidad_fila, duracion_consulta, capacidad_departamento):
        self.nombre = nombre
        self.capacidad_fila = capacidad_fila
        self.duracion_consulta = duracion_consulta
        self.capacidad_departamento = capacidad_departamento
        self.sem_fila = threading.Semaphore(capacidad_fila)
        self.mutex_departamento = threading.Lock()
        self.tiempo = 0


    #Metodos
    '''
    El metodo simula la consulta de un departamento

        Parametros:
            self (Departamento): Objeto de la clase Departamento

        Retorno:
            El metodo no retorna nada
    '''
    def Simular_consulta(self):
        time.sleep(self.duracion_consulta)
    

    '''
    El metodo hace ingresar a una persona a la fila de un departamento

        Parametros:
            self (Departamento): Objeto de la clase Departamento
            persona (str): Nombre de la persona que ingresa a la fila
            nDepto (int): Numero del departamento al que ingresa la persona

        Retorno:
            El metodo no retorna nada
    '''
    def Ingresar_fila(self, persona, nDepto):
        tiempoLlegada = datetime.now().strftime('%H:%M:%S.%f')[:-3]
        #Se intenta ingresar al departamento
        start_time = time.time()
        while self.capacidad_fila - self.sem_fila._value < self.capacidad_departamento:
            self.tiempo = time.time() - start_time
            if self.tiempo >= 10:
                break
        tiempoEntradaD = datetime.now().strftime('%H:%M:%S.%f')[:-3]


        self.tiempo = 0
        with open(f"Departamento_de_{self.nombre}.txt", "a") as file:
            file.write(f"{persona}, {tiempoLlegada}, {tiempoEntradaD}, {nDepto}\n")
        self.sem_fila.release()
        self.Iniciar_departamento(persona, nDepto)
        

    '''
    El metodo inicia la consulta de un departamento

        Parametros:
            self (Departamento): Objeto de la clase Departamento
            persona (str): Nombre de la persona que ingresa a la fila
            nDepto (int): Numero del departamento al que ingresa la persona

        Retorno:
            El metodo no retorna nada
    '''
    def Iniciar_departamento(self, persona, nDepto):
        self.Simular_consulta()
        #Salida en caso de ser necesario
        if nDepto == 2:
            with open("Salida.txt", "a") as file:
                file.write(f"{persona}, {datetime.now().strftime('%H:%M:%S.%f')[:-3]}\n")



'''
La funcion hace ingresar a una persona a la fila de dos departamentos

    Parametros:
        persona (str): Nombre de la persona que ingresa a la fila
        departamentos (list): Lista de objetos de la clase Departamento

    Retorno:
        La funcion no retorna nada
'''
def ingreso_Pdl(persona, departamentos):
    Departamento1, Departamento2 = random.sample(departamentos, 2)
    mutex_Pdl.acquire()
    tiempoLlegada = datetime.now().strftime('%H:%M:%S.%f')
    tiempoEntrada = "" 
    tiempoEntrada2 = "" 
    try:
        Departamento1.sem_fila.acquire(blocking=True, timeout=-1)
        while Departamento1.capacidad_fila - Departamento1.sem_fila._value-1 < 0:
            print(f'#Se queda esperando en PdLamparas - {persona} {Departamento1.sem_fila._value} - {Departamento1.nombre}')
            time.sleep(5)
        
        mutex_Pdl.release()
        tiempoEntrada = datetime.now().strftime('%H:%M:%S.%f')[:-2]
        Departamento1.Ingresar_fila(persona, 1)

        Departamento2.sem_fila.acquire(blocking=True, timeout=-1)
        while Departamento2.capacidad_fila - Departamento2.sem_fila._value-1 < 0:
            print(f'#Se queda esperando en PdLamparas - {persona} - {datetime.now().strftime("%H:%M:%S.%f")[:-3]} - n=2 {Departamento2.nombre} - {Departamento2.capacidad_fila - Departamento2.sem_fila._value} -A {Departamento2.sem_fila._value}')
            time.sleep(5)

        tiempoEntrada2 = datetime.now().strftime('%H:%M:%S.%f')[:-2]
        Departamento2.Ingresar_fila(persona, 2)
    finally:
        with open("PdLamparas.txt", "a") as file:
            file.write(
                f"{persona}, {tiempoLlegada}, {Departamento1.nombre}, {tiempoEntrada}, {Departamento2.nombre}, {tiempoEntrada2}  \n")



#Lista de objetos de la clase Departamento
departamentos = [
    Departamento("Matematicas", 20, 9, 10), #Capacidad = 20, Duracion = 9, Capacidad Departamento = 10
    Departamento("Informatica", 8, 5, 2),   #Capacidad = 8, Duracion = 5, Capacidad Departamento = 2
    Departamento("Fisica", 15, 7, 5),       #Capacidad = 15, Duracion = 7, Capacidad Departamento = 5
    Departamento("Quimica", 6, 4, 3),       #Capacidad = 6, Duracion = 4, Capacidad Departamento = 3
    Departamento("DEFIDER", 6, 1, 5),       #Capacidad = 6, Duracion = 1, Capacidad Departamento = 5
    Departamento("Mecanica", 9, 4, 4),      #Capacidad = 9, Duracion = 4, Capacidad Departamento = 4
    Departamento("Minas", 7, 5, 2)          #Capacidad = 7, Duracion = 5, Capacidad Departamento = 2
]



max_personas = 500

#Lista de hebras
threads = []



#Se crean las hebras y comenzar simulación
for i in range(max_personas):
    thread = threading.Thread(target=ingreso_Pdl, args=(f"Persona{i+1}",departamentos), name=f"Persona{i+1}")
    threads.append(thread)
    thread.start()



#Esperar a que todas las hebras terminen
for thread in threads:
    thread.join()
