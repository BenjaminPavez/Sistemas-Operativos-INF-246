#!/bin/bash
# PARA DAR PERMISOS A BASH: chmod +x ViewThreads.sh

#Nombre del proceso a buscar
nombre_proceso="Laboratorio_4.py"

#Numero de veces que se ejecuta ps lf
num_veces=100

archivo_salida="canthebras.txt"

#Se obtiene el PID del proceso
pid=$(pgrep -o -f "$nombre_proceso")

if [ -z "$pid" ]; then
    echo "No se encontró un proceso con el nombre $nombre_proceso."
    exit 1
fi

echo "PID del proceso $nombre_proceso: $pid"

for ((i=1; i<=$num_veces; i++)); do
    echo "Ejecutando ls /proc/$pid/task | wc -l (vez $i de $num_veces)..."
    ls /proc/$pid/task | wc -l >> "$archivo_salida"
    sleep 1
done

echo "Todas las ejecuciones completadas. La salida se ha guardado en $archivo_salida."
