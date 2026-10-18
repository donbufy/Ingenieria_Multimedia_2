#ifndef TEST_UTILS_H
#define TEST_UTILS_H

#include <fstream>
#include <sstream>
#include <string>
#include <vector>

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

// Normaliza una salida para comparaciones robustas frente a diferencias
// de espacios en blanco no significativas: unifica saltos de línea
// (\r\n -> \n), recorta espacios/tabs al principio y al final de cada
// línea, colapsa espacios/tabs internos consecutivos en uno solo y
// descarta las líneas en blanco resultantes (estén donde estén, no solo
// al principio/final del texto).
static std::string normalizarSalida(const std::string& texto) {
    std::vector<std::string> lineas;
    std::istringstream iss(texto);
    std::string linea;
    while (std::getline(iss, linea)) {
        if (!linea.empty() && linea.back() == '\r') {
            linea.pop_back();
        }

        std::string colapsada;
        bool ultimoEspacio = true; // evita espacios al principio
        for (char c : linea) {
            if (c == ' ' || c == '\t') {
                if (!ultimoEspacio) {
                    colapsada += ' ';
                }
                ultimoEspacio = true;
            } else {
                colapsada += c;
                ultimoEspacio = false;
            }
        }
        while (!colapsada.empty() && colapsada.back() == ' ') {
            colapsada.pop_back();
        }

        if (!colapsada.empty()) {
            lineas.push_back(colapsada);
        }
    }
    std::string resultado;
    for (const std::string& l : lineas) {
        resultado += l;
        resultado += '\n';
    }
    return resultado;
}

// Compara dos salidas de forma exacta salvo por las diferencias de
// espacios en blanco que normalizarSalida() considera insignificantes.
static bool mismaSalida(const std::string& actual, const std::string& esperada) {
    return normalizarSalida(actual) == normalizarSalida(esperada);
}

#endif // TEST_UTILS_H
