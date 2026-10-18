// Práctica 1 — Catálogo de Recursos
// Aplicación de línea de comandos (Sección 6 del enunciado).
//
// Lee comandos de la entrada estándar y escribe el resultado
// en la salida estándar, sin prompts ni mensajes adicionales.

#include <iostream>
#include <sstream>
#include <string>

#include "Catalog.h"

using namespace std;

int main() {
    Catalog catalog;
    string line;
    string parametro_temporal;

    while (getline(cin, line)) {
        istringstream iss(line);
        string command;
        if (!(iss >> command)) {
            continue;   // línea vacía
        }

        else if (command == "EXIT") {
            break;
        }

        else if (command == "LOAD"){

            

            iss >> parametro_temporal;
            if (parametro_temporal.empty()){
            
                cout << "INVALIDCMD" << endl;
            
            }
            else{
            bool resultado = catalog.loadFromFile(parametro_temporal);
            int numero = catalog.size();
            if(resultado == true){
               
                cout << "OK " << numero <<endl;
            
            }
            else{
            
                cout << "ERROR" << endl;
            
            }
        }
        
    }

        else if(command == "ADD"){
        
            iss >> parametro_temporal;

            if(!catalog.findById( parametro_temporal).getId().empty()){
            
                cout << "DUPLICATE"<< endl;
            
            }
            else{
            
                string year, title, tags;

                 iss >> year;

                 iss >> title;

                 iss >> tags;
                int senal3 = 0;
                if(year.empty() || title.empty() || tags.empty()){

                    cout << "INVALIDCMD" << endl;
                    senal3 = -3;
                }

                else{
                    int senal2 = 0;
                    try{
                    
                        stoi(year);
                        
                    } catch (...){
                    
                        cout <<"INVALIDCMD"<<endl;
                        senal2 = -3;
                    }
                    for(char &c : title){
                    
                        if(c == '_'){
                        
                            c = ' ';
                        
                        }
                    
                    }
                    if (senal2 == 0 && senal3 == 0) {
                        
                        Resource r1(parametro_temporal, stoi(year), title);

                        
                        
                        int senal = 0;

                        

                            if(tags != "-"){
                            
                            
                            
                            
                                for(char &c  : tags){
                                
                                    if(c == ','){
                                            
                                        c = ' ';

                                        senal++;
                                        
                                    }
                                    
                                }
                                if(senal > 0){

                                    stringstream tag(tags);

                                    parametro_temporal = "";

                                    for(int i = 0; i <= senal; i++){
                                        
                                        
                                        tag >> parametro_temporal;

                                        r1.addTag(parametro_temporal);
                                        
                                    }

                                
                                }

                                else{
                                
                                    r1.addTag(tags);
                                
                                }

                            }
                        
                            

                            if(catalog.add(r1)){
                            
                                cout << " OK" <<endl;
                            
                            }
                            else{
                            
                                
                            
                        }


                    }
                    
                    
                
                }

                

            
            }
            
            
        }

        else if(command == "DEL"){
            
            parametro_temporal = "";
            iss >> parametro_temporal;

            if(parametro_temporal.empty()){
            
                cout << "INVALIDCMD" << endl;
            
            }
            else{

                if(catalog.removeById(parametro_temporal)){
                
                    cout << "OK" << endl;
                
                }

                else{
                
                    cout << "NOTFOUND" << endl;
                
                }

            }
        
        }

        else if(command == "GET"){
        
            iss >> parametro_temporal;
            if(parametro_temporal.empty()){
            
                cout << "INVALIDCMD" << endl;
            
            }
            else{
                Resource r1 = catalog.findById(parametro_temporal);

                if(r1.getId().empty()){
                
                    cout << "NOTFOUND" << endl;
                
                }
                else{
                
                    cout << r1 << endl;
                
                }

            }
        
        }

        else if(command == "FINDTAG"){
        
            parametro_temporal = "";

            iss >> parametro_temporal;

            if (parametro_temporal.empty()){
            
                cout << "INVALIDCMD" << endl;
            
            }

            else{
                vector<Resource> recursos= catalog.findByTag(parametro_temporal);
                
                
                
                    
                cout << "COUNT " << recursos.size() << endl;

                for(size_t i = 0; i<recursos.size(); i++){
                
                    cout << recursos[i];                
                    cout << endl;
                    
                }

            }
            
        }

        else{
        
            cout << "INVALIDCMD" << endl;
        
        }

        // TODO: procesa el resto de comandos (LOAD, ADD, DEL, GET, FINDTAG)
        // Recuerda: si el comando no existe o faltan argumentos -> INVALIDCMD
    }

    return 0;
}
