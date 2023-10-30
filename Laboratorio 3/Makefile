# JFLAGS: Flags con las que se compila el código.
# -g: Compila con información de depuración.
# -Xlint:all: Muestra todos los avisos al compilar.
JFLAGS = -g -Xlint:all

# Especifica el compilador de Java a utilizar
JC = javac

# Especifica la máquina virtual para ejecutar códigos Java
JVM = java

# Nombre de la clase que contiene la función main
MAIN = LeerArchivosDeDirectorio

# En JAVAS se indican los archivos .java en el directorio actual
JAVAS := $(wildcard ./*.java)

# CLASSES son los nombres de los archivos ya compilados, corresponden a los JAVAS pero con la extensión .class
CLASSES := $(JAVAS:.java=.class)

# Indica que al ejecutar el comando make o make all se desean compilar todos los .java
all: $(CLASSES)

# Permite ejecutar:
# make all (compila todos los programas)
# make clean (elimina los archivos .class del directorio)
# make run (ejecuta el programa)
.PHONY: all clean run

# Compila un archivo .java para generar su correspondiente .class, utilizando JC y JFLAGS definidos anteriormente
%.class: %.java
	$(JC) $(JFLAGS) $^

# Ejecuta el programa principal. Si las clases no están compiladas, las compila automáticamente.
run: $(MAIN).class
	$(JVM) $(MAIN)

# Elimina todos los archivos compilados
clean:
	rm -f $(CLASSES)
