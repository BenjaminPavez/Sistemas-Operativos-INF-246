import threading
import time
from datetime import datetime
import random

mutex_Pdl = threading.Lock()

class Departamento:
    def __init__(self, nombre, capacidad_fila, duracion_consulta, capacidad_departamento):
        self.nombre = nombre
        self.capacidad_fila = capacidad_fila
        self.duracion_consulta = duracion_consulta
        self.capacidad_departamento = capacidad_departamento
        self.sem_fila = threading.Semaphore(capacidad_fila)
        self.mutex_departamento = threading.Lock()
        self.sem_departamento = threading.Semaphore(capacidad_departamento)

    def Simular_consulta(self):
        time.sleep(self.duracion_consulta)

    def Ingresar_fila(self, persona, nDepto):
        if self.sem_fila.acquire():
            print(f"{persona} ingresa a la fila de {self.nombre}")
            tiempoLlegada = datetime.now().strftime('%H:%M:%S.%f')[:-3]
            try:
                print(f'{persona} intenta entrar a {self.nombre}')
                #Se intenta ingresar al departamento
                self.sem_departamento.acquire()
                print(self.capacidad_departamento,self.sem_departamento._value)
                if self.sem_departamento._value == 0: #Agregar temporizador en caso de que no exista mas gente para llenar la fila
                    print(self.capacidad_fila-self.sem_fila._value, "personas en la fila de", self.nombre, "de un total de",self.capacidad_fila,self.sem_fila._value  )
                    with open(f"Departamento_de_{self.nombre}.txt", "a") as file:
                        file.write(f"{persona}, {tiempoLlegada} ,{datetime.now().strftime('%H:%M:%S.%f')[:-3]}\n")
                    self.Iniciar_departamento(persona, nDepto)

                else:
                    print("***")
                    print(f'Cantidad en la fila {self.capacidad_fila-self.sem_fila._value} son necesarios {self.capacidad_departamento} en el departamento {self.nombre}')
                    print("Debe esperar en la fila ya que esta lleno el depto o no hay los necesarios en la fila")
                    print("***")

            finally:
                self.sem_fila.release()
                self.sem_departamento.release()

    def Iniciar_departamento(self, persona, nDepto):
        print(f"{persona} entra a la consulta de {self.nombre}")
        print(self.capacidad_departamento-self.sem_departamento._value, "personas en el departamento de", self.nombre, "de un total de ",self.capacidad_departamento)
        self.Simular_consulta()
        #self.Salida(persona, nDepto) Aplicar despues

    def Salida(self, persona, nDepto):
        self.mutex_departamento.acquire(blocking=False)
        try:
            print(f"{persona} sale de {self.nombre}")
            if nDepto == 2:
                print(f"{persona} sale de la Universidad")
        finally:
            self.mutex_departamento.release()



def ingreso_Pdl(persona, departamentos):
    Departamento1, Departamento2 = random.sample(departamentos, 2)
    mutex_Pdl.acquire()
    tiempoLlegada = datetime.now().strftime('%H:%M:%S.%f')[:-3]
    try:
        with open("PdLamparas.txt", "a") as file:
            #Si la fila no esta llena se hace ingreso a la fila
            if Departamento1.capacidad_fila-(Departamento1.sem_fila._value-1) >= 0:
                file.write(f"{persona}, {tiempoLlegada}, {Departamento1.nombre}, {datetime.now().strftime('%H:%M:%S.%f')[:-3]}, {Departamento2.nombre} \n")
                print(f"{persona} ingresa a la fila de {Departamento1.nombre}")
                Departamento1.Ingresar_fila(persona, 1)
            else:
                print("Se queda esperando en PdLamparas ya que la fila esta llena")
                mutex_Pdl.release()
    finally:
        mutex_Pdl.release()



departamentos = [
    Departamento("DEFIDER", 6, 5, 1), # Departamento("DEFIDER", 6, 1, 5),
    Departamento("Mecanica", 9, 3, 3), #    Departamento("Mecanica", 9, 4, 4),
    Departamento("Minas", 7, 5, 2), #    Departamento("Minas", 7, 5, 2),
]
"""  Departamento("Matematicas", 20, 9, 10),
    Departamento("Informatica", 8, 5, 2),
    Departamento("Fisica", 15, 7, 5),
    Departamento("Quimica", 6, 4, 3), """
max_personas = 10 #500

threads = []
for i in range(max_personas):
    thread = threading.Thread(target=ingreso_Pdl, args=(f"Persona{i+1}",departamentos))
    threads.append(thread)
    thread.start()

# Esperar a que todas las hebras terminen
for thread in threads:
    thread.join()