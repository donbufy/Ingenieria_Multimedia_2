// Práctica 2 — Reproductor de música
// Aplicación de línea de comandos (Sección 6 del enunciado).

#include <iostream>
#include <sstream>
#include <string>

#include "Playlist.h"

using namespace std;

int main() {
    Playlist playlist;
    string line;

    while (getline(cin, line)) {
        istringstream iss(line);
        string command;
        if (!(iss >> command)) {
            continue;   // línea vacía
        }

        if (command == "EXIT") {
            break;
        }
        (void)playlist;   // TODO: elimina esta línea al empezar a usar el objeto

        // TODO: procesa el resto de comandos (LOAD, ADD, DEL, LIST,
        //       PLAY, NEXT, PREV, TOTAL)
        // Recuerda: comando desconocido o argumentos inválidos -> INVALIDCMD
    }

    return 0;
}
