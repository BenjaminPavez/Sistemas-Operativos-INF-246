# JFLAGS: Flags con las que se compila el código.
# -g: Compila con información de depuración.
# -Xlint:all: Muestra todos los avisos al compilar.
JFLAGS = -g -Xlint:all

# Especifica el compilador de Java a utilizar
JC = /usr/lib/jvm/java-11-openjdk-amd64/bin/javac

# Especifica la máquina virtual para ejecutar códigos Java
JVM = /usr/lib/jvm/java-11-openjdk-amd64/bin/java

# Nombre de la clase que contiene la función main
MAIN = Main

# En JAVAS se indican los archivos .java en el directorio actual
JAVAS := $(wildcard ./*.java)

# CLASSES son los nombres de los archivos ya compilados, corresponden a los JAVAS pero con la extensión .class
CLASSES := $(JAVAS:.java=.class)

# Indica que al ejecutar el comando make se desean compilar y ejecutar el programa Main
.PHONY: all

# Compila un archivo .java para generar su correspondiente .class, utilizando JC y JFLAGS definidos anteriormente
%.class: %.java
    $(JC) $(JFLAGS) $^

# Compila y ejecuta el programa Main
all: $(CLASSES)
    $(JVM) $(MAIN)

# Elimina todos los archivos compilados
clean:
    rm -f $(CLASSES)
