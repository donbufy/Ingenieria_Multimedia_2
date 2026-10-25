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

            iss >> temp;

            if(temp.empty()){
            
                cout << "INVALIDCMD" << endl;
                
            
            }
            else{
            
                if(playlist.load(temp)){
                
                    cout << "OK " << playlist.size() << endl;
                
                }
                
                else{
                
                    cout << "ERROR" << endl;
                
                }

                
            
            }


            
        
        }
           
        else if(command == "ADD"){
            iss >> ws;
            string id = "";
            string duration = "";
            string title = "";
            int duracion = -1;

            bool senyal = true;
            
            if(getline(iss, id, ' ') && getline(iss, duration, ' ') && getline(iss,title, ' ')){

                try {
                
                duracion = stoi(duration);
                
                }catch(const invalid_argument&){
                
                    senyal = false;

                }
            
                
            }

            
            else{
            
                senyal = false;
            
            }

            if(playlist.findById(id)){
            
                senyal = false;
            
            }

            if(duracion < 0 && senyal){
                
                senyal = false;

            
            }

            if(!title.empty()){
            
                for(char &c: title){
                
                   
                    if(c == '_'){

                        c = ' ';

                    }
                
                }
            }
            if(senyal){
            
                Track cansion(id, title, duracion);
                
                playlist.add(cansion);

                cout << "OK" << endl;

            }
            else{
                
                if(playlist.findById(id)){
            
                    cout << "DUPLICATE" << endl;
            
                }
                else{

                    cout << "INVALIDCMD" << endl;
                    
                }
            
            }

        }

        else if(command == "DEL"){
        
            temp = "";

            iss >> temp;
            if(temp.empty()){
            
                cout << "INVALIDCMD" <<endl;
            
            }
            
            else{
            
                if(playlist.removeById(temp)){
                
                    cout << "OK" << endl;
                
                }
                else{
                
                    cout << "NOTFOUND" << endl;
                
                }
            
            }
        
        }

        else if(command == "LIST"){
            
                
            if(playlist.size() == 0){
            
                cout << "SIZE " << 0 << endl;
            
            }
            else{
            
                cout <<  "SIZE " << playlist.size() << endl << playlist;
            
            }
        
            
        
        }

        else if(command == "PLAY"){
        
            temp = "";

            
            
            
            if(playlist.size() == 0){
            
                cout << "EMPTY" << endl;
            
            }
            else{
                
                

                cout << (*playlist.getCurrent()) << endl;
            
            }
            
            
        
        }

        else if(command == "NEXT"){
            
            temp = "";

            if(playlist.size() == 0){
            
                cout << "EMPTY" << endl;
            
            }
            else{
            
                playlist.next();

                cout << playlist;
            
            }
        
        
            
        
        }

        else if(command == "PREV"){
            
            temp = "";

            iss >> temp;

            

        
        
            if(playlist.size() == 0){
                
                cout << "EMPTY" << endl;
                
            }
            else{
                
                playlist.prev();

                cout << playlist;
                
            }
        
        
            
        
        }

        else if(command == "TOTAL"){
            
            temp = "";

            
            
            
            
                cout << playlist.totalDuration() << endl;
            
            
        
        }
        else{
        
            cout << "INVALIDCMD" << endl;
        
        }


        // TODO: procesa el resto de comandos (LOAD, ADD, DEL, LIST,
        //       PLAY, NEXT, PREV, TOTAL)
        // Recuerda: comando desconocido o argumentos inválidos -> INVALIDCMD
    }

    return 0;
}
