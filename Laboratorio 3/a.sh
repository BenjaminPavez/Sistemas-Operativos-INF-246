#!/bin/bash

# Número de veces que deseas ejecutar ps lf
num_veces=20

# Nombre del archivo donde se guardará la salida
archivo_salida="procesos.txt"

for ((i=1; i<=$num_veces; i++)); do
    echo "Ejecutando ps lf (vez $i de $num_veces)..."
    ps lf >> "$archivo_salida"
    sleep 1  # Pausa de 1 segundo entre ejecuciones (ajusta según tus necesidades)
done

echo "Todas las ejecuciones completadas. La salida se ha guardado en $archivo_salida."