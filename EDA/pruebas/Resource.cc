// Práctica 1 — Catálogo de Recursos
// Implementación de la clase Resource.

#include "Resource.h"
using namespace std;
// TODO: implementa aquí los métodos declarados en Resource.h
Resource::Resource():

    id(""),
    title(""),
    year(0)

    {
    }
Resource::Resource(const string& id, int year, const string& title):

    id(id),
    year(year), 
    title(title)

    {
    }

Resource::Resource(const Resource& other):

    id(other.getId()),
    year(other.getYear()),
    title(other.getTitle()),
    tags(other.tags)

    {
    }

Resource& Resource::operator=(const Resource& other){

    id = other.id;
    title = other.title;
    year = other.year;
    tags = other.tags;
    return *this;

}

Resource::~Resource(){
}

string Resource :: getId() const{

    return id;

}

    string Resource :: getTitle() const{
    
        return title;
    
    }

    int Resource :: getYear() const{
    
        return year;
    
    }

vector<string> Resource :: getTags() const{

    return tags;

}

bool Resource :: addTag(const string& tag){

string texto = tag;

    if(texto.empty() == false){
        
        

        for (char& c : texto){
        
            if(!isalpha(c) && !isdigit(c) && c != ' '){
            
                return false;

            }

            if(isalpha(c) && isupper(c)){
            
                c = tolower(c);
            
            }

            
        }

        //Borrar espacios al principio y al final

        int indice = -1;
        int i = 0;
        
        while(i < texto.size() && texto[i] == ' '){
        
            indice++;
            i++;
        
        }

        if (indice >= 0){
        
            texto.erase(0, indice  + 1);
        
        }

        i = texto.size() - 1;
        indice = -1;

        while(i >= 0 && texto[i] == ' '){
        
            indice++;
            i--;
        
        }

        if (indice >= 0){
        
            texto.erase(texto.size() - indice - 1, indice  + 1);
        
        }

        if (texto.size() == 0){
        
            return false;
        
        }
        //insertar manteniendo orden lexico

        bool ordenado = true;
        indice = 0;

        if (tags.size() > 0){
            
            
            for(int l = 0; l< tags.size(); l++){
            
                if(texto == tags[l]){
                
                    return false;
                
                }

            }


            for(int orden = 1; orden < tags.size() ; orden++){
            
                if(tags[orden -1] > tags[orden]){
                
                    ordenado = false;
                
                }
            
            }

            if(ordenado == true){

                for(i = 0; i < tags.size(); i++){
                
                    if(texto > tags[i]){
                    
                        indice++;
                    
                    }
                    else{
                        
                        i = tags.size() - 1;
                    
                    }
                
                }

                tags.insert(tags.begin() + indice, texto);

            }

            else{
                tags.push_back(texto);

                vector<string> tags1 = tags;
                vector<string> tags2;

                string etiqueta = tags1[0];
                indice = 0;
                int indice1 = 0;
                
                while(tags1.size() > 0){

                    for(int z = 0; z < tags1.size(); z++){
                    
                        if(etiqueta != tags1[z] && tags1[z] < etiqueta){
                        
                            etiqueta = tags1[z];
                            indice1 = z;
                        
                        }
                    
                    }
                    if(tags1.size() == 1){
                    
                        indice1 = 0;
                    
                    }
                    
                    tags2.insert(tags2.begin() + indice, etiqueta);
                    indice ++;
                    tags1.erase(tags1.begin() + indice1);

                    if(tags1.size() > 0){
                        etiqueta = tags1[0];
                    
                    }

                }

                tags = tags2;

            }

        }

        else{
        
            tags.push_back(texto);
        
        }

    }

    else{
    
        return false;
    
    }

    return true;


}


