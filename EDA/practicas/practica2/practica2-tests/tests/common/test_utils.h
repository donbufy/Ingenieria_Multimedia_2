#ifndef TEST_UTILS_H
#define TEST_UTILS_H

#include <fstream>
#include <sstream>
#include <string>

// Lee el contenido completo de un fichero
static std::string leerFichero(const std::string& ruta) {
    std::ifstream f(ruta.c_str());
    std::ostringstream oss;
    oss << f.rdbuf();
    return oss.str();
}

// Escribe contenido en un fichero
static void escribirFichero(const std::string& ruta, const std::string& contenido) {
    std::ofstream f(ruta.c_str());
    f << contenido;
}

// Normaliza los finales de línea (\r\n -> \n). No recorta ni colapsa
// espacios: en esta práctica el marcador de dos caracteres al principio
// de cada línea ("  " o "> ") es parte del formato exigido, así que una
// comparación que ignorase espacios en blanco dejaría de detectar un
// operator<< incorrecto.
static std::string normalizarSalida(const std::string& texto) {
    std::string resultado;
    resultado.reserve(texto.size());
    for (std::size_t i = 0; i < texto.size(); ++i) {
        if (texto[i] == '\r' && i + 1 < texto.size() && texto[i + 1] == '\n') {
            continue;
        }
        resultado += texto[i];
    }
    return resultado;
}

// Compara dos salidas de forma exacta salvo por el estilo de fin de línea.
static bool mismaSalida(const std::string& actual, const std::string& esperada) {
    return normalizarSalida(actual) == normalizarSalida(esperada);
}

#endif // TEST_UTILS_H
