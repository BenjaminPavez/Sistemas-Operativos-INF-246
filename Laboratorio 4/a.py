import threading
import time
import random
from datetime import datetime, timedelta
mutex_pdlamparas = threading.Lock()
sem_pdlamparas = threading.Semaphore(1)

class Departamento:
    def __init__(self, nombre, capacidad_fila, duracion_consulta, capacidad_departamento):
        self.nombre = nombre
        self.capacidad_fila = capacidad_fila
        self.duracion_consulta = duracion_consulta
        self.capacidad_departamento = capacidad_departamento
        self.mutex_fila = threading.Lock()
        self.sem_fila = threading.Semaphore(capacidad_fila)
        self.mutex_departamento = threading.Lock()
        self.sem_departamento = threading.Semaphore(capacidad_departamento)

    def simular_consulta(self):
        time.sleep(self.duracion_consulta)

    def ingresar_fila(self, persona, nDepto):
        with self.mutex_fila:
            iniciofila = time.strftime('%H:%M:%S')
            with open(f"Departamento_de_{self.nombre}.txt", "a") as file:
                # Espera hasta que haya espacio disponible en la fila
                while self.sem_fila._value >= self.capacidad_fila:
                    pass
                file.write(f"{persona}, {iniciofila}, {time.strftime('%H:%M:%S')}, {nDepto} \n")
                self.sem_fila.release()  # Mover la liberación del semáforo al final
                self.ingresar_departamento(persona, nDepto)

    def ingresar_departamento(self, persona, nDepto):
        # Espera hasta que haya suficientes personas en la fila o no hay más personas
        while self.sem_fila._value > 0 and (self.sem_fila._value < self.capacidad_departamento):
            pass
        self.iniciar_departamento(persona)



    
    #ingreso al departamento
    def iniciar_departamento(self, persona):
        self.sem_departamento.acquire()
        with self.mutex_departamento:
            self.simular_consulta()
        self.sem_departamento.release()

# Función para simular el recorrido de una persona
def ingresar_patio(persona, departamentos):
    departamento1, departamento2 = random.sample(departamentos, 2)
    sem_pdlamparas.acquire()
    # Sección crítica para registrar el ingreso al Patio de las Lamparas
    with mutex_pdlamparas:
        with open("PdLamparas.txt", "a") as file:
            tiempoInicio = time.strftime('%H:%M:%S')
            file.write(f"{persona}, {time.strftime('%H:%M:%S')}, {departamento1.nombre},")

            if departamento1.sem_fila.acquire(blocking=False):
                file.write(f" {datetime.strptime(tiempoInicio, '%H:%M:%S').strftime('%H:%M:%S')}, {departamento2.nombre}\n")
                departamento1.ingresar_fila(persona, 1)
            
            # 2 departamento que ingresa
            if departamento2.sem_fila.acquire(blocking=False):
                departamento2.ingresar_fila(persona, 2)


        # Mover la liberación del semáforo al final de la sección crítica
    sem_pdlamparas.release()
    with open("Salida.txt", "a") as file:
        file.write(f"{persona}, {time.strftime('%H:%M:%S')}\n")


# Ejemplo de uso con hebras
departamentos = [
    Departamento("Matematicas", 20, 9, 10),
    Departamento("Informatica", 8, 5, 2),
    Departamento("Fisica", 15, 7, 5),
    Departamento("Quimica", 6, 4, 3),
    Departamento("DEFIDER", 6, 1, 5),
    Departamento("Mecanica", 9, 4, 4),
    Departamento("Minas", 7, 5, 2),
]

threads = []
max_personas = 50  # Cantidad de hebras/personas

for i in range(max_personas):
    thread = threading.Thread(target=ingresar_patio, args=(f"Persona{i+1}", departamentos))
    threads.append(thread)
    thread.start()

# Esperar a que todas las hebras terminen
for thread in threads:
    thread.join()
