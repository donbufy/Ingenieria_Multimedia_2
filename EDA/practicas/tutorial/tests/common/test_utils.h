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

#endif // TEST_UTILS_H
