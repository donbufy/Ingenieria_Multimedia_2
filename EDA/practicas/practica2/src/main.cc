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

    string temp = "";

    while (getline(cin, line)) {
        istringstream iss(line);
        string command;
        if (!(iss >> command)) {
            continue;   // línea vacía
        }

        else if (command == "EXIT") {
            break;
        }

        else if(command == "LOAD") {
        
            temp = "";

            if(!(iss >> temp)){
            
                cout << "INVALIDCMD" << endl;
            
            }
            else{
            
                if(playlist.load(temp)){
                
                    cout << "OK " << temp.size() << endl;
                
                }
                
                else{
                
                    cout << "ERROR" << endl;
                
                }

                
            
            }


            
        
        }
           
        else if(command == "ADD"){
        
        
        
        }
        // TODO: procesa el resto de comandos (LOAD, ADD, DEL, LIST,
        //       PLAY, NEXT, PREV, TOTAL)
        // Recuerda: comando desconocido o argumentos inválidos -> INVALIDCMD
    }

    return 0;
}
