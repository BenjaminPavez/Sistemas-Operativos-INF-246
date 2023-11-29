#!/bin/bash

#se ejecutan 20 veces ps lf para ver los procesos
num_veces=20

archivo_salida="procesos.txt"

for ((i=1; i<=$num_veces; i++)); do
    echo "Ejecutando ps lf (vez $i de $num_veces)..."
    ps lf >> "$archivo_salida"
    sleep 1
done

echo "Todas las ejecuciones completadas. La salida se ha guardado en $archivo_salida."
