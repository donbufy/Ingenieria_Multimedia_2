#!/bin/bash

# Script para preparar la entrega del proyecto
# Este script comprime el código fuente y los archivos necesarios en un archivo tgz
# Uso: ./entrega.sh

FILES="AUTHOR.txt src/Track.h src/Track.cc src/Playlist.h src/Playlist.cc src/main.cc"

clear

# Comprueba si todos los archivos necesarios existen
for file in $FILES; do
    if [ ! -f "$file" ]; then
        echo ""
        echo "❌ El archivo '$file' no existe. Por favor, asegúrate de que todos los archivos necesarios estén presentes antes de ejecutar este script."
        echo ""
        exit 1
    fi
done

# Comprueba si AUTHOR.txt tiene contenido
if [ ! -s AUTHOR.txt ]; then
    echo ""
    echo "❌ El archivo AUTHOR.txt está vacío. Por favor, añade tu nombre completo y DNI."
    echo ""
    exit 1
fi

# Variables
NOMBRE_PROYECTO="p2"
ARCHIVO_ENTREGA="${NOMBRE_PROYECTO}_entrega.tgz"

# Limpiar entregas anteriores
if [ -f "$ARCHIVO_ENTREGA" ]; then
    rm "$ARCHIVO_ENTREGA"
fi

# Crear el archivo tgz incluyendo AUTHOR.txt y src/
echo ""
echo "📦 Creando el archivo de entrega..."
COPYFILE_DISABLE=1 tar --exclude='._*' --exclude='*/._*' -czf "$ARCHIVO_ENTREGA" AUTHOR.txt src/
echo ""
echo "✅ Archivo de entrega creado: $ARCHIVO_ENTREGA"
echo ""
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
echo "  Recuerda subir el archivo al servidor de prácticas"
echo "  URL: https://pracdlsi.dlsi.ua.es"
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
echo ""
