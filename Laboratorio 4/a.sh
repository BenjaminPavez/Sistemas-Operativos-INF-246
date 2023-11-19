#!/bin/bash

# Nombre del proceso a buscar
nombre_proceso="intento1.py"

# Número de veces que deseas ejecutar ps lf
num_veces=10000

# Nombre del archivo donde se guardará la salida
archivo_salida="canthebras.txt"

# Obtener el PID del proceso creado por intento1.py
pid=$(pgrep -o -f "$nombre_proceso")

if [ -z "$pid" ]; then
    echo "No se encontró un proceso con el nombre $nombre_proceso."
    exit 1
fi

echo "PID del proceso $nombre_proceso: $pid"

for ((i=1; i<=$num_veces; i++)); do
    echo "Ejecutando ls /proc/$pid/task | wc -l (vez $i de $num_veces)..."
    ls /proc/$pid/task | wc -l >> "$archivo_salida"
    sleep 1  # Pausa de 1 segundo entre ejecuciones (ajusta según tus necesidades)
done

echo "Todas las ejecuciones completadas. La salida se ha guardado en $archivo_salida."
