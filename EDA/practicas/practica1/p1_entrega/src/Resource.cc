// Práctica 1 — Catálogo de Recursos
// Implementación de la clase Resource.

#include "Resource.h"
using namespace std;
// TODO: implementa aquí los métodos declarados en Resource.h

string Resource :: normalizarTag(const string & tag)const{

    string texto = tag;
    
    if(texto.empty() == true){
    
        return(texto);

    }

    for (char& c : texto){
        
        

        if(isalpha(c) && isupper(c)){
            
            c = tolower(c);
            
        }

            
    }

        //Borrar espacios al principio y al final

    int indice = -1;
    size_t i = 0;
        
    while(i < texto.size() && texto[i] == ' '){
        
        indice++;
        i++;
        
    }

    if (indice >= 0){
        
        texto.erase(0, indice  + 1);
        
    }
    if(texto.size() == 0){
    
        return texto;
    
    }
    int u = static_cast<int>(texto.size()) - 1;
    indice = -1;
    

    while(u >= 0 && texto[u] == ' '){
        
        indice++;
        u--;
        
    }

    if (indice >= 0){
        
        texto.erase(texto.size() - indice - 1, indice  + 1);
        
    }

    return texto;

}

ostream& operator<<(ostream& os, const Resource& r){

    os << r.getId() << " " << "(" << r.getYear() << ")" << " " << r.getTitle() << " " << "[";

   
    
    if(r.getTags().empty() == false){

        for(string etiqueta : r.getTags()){
        
            os << etiqueta;

            if(etiqueta != r.getTags()[r.getTags().size() - 1]){
            
                os << ",";
            
            }
        
        }
    
    }

    os << "]";
    
    return os;

}



Resource::Resource():

    id(""),
    title(""),
    year(0)

    {
    }
Resource::Resource(const string& id, int year, const string& title):

    id(id),
    title(title),

    year(year)
    

    {
    }

Resource::Resource(const Resource& other):

    id(other.getId()),
    title(other.getTitle()),
    year(other.getYear()),
    
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

texto = normalizarTag(texto);

    if(texto.empty() == false){
    
    
        

        for (char& c : texto){
        
            if(!isalpha(c) && !isdigit(c) && c != ' '){
            
                return false;

            }
            
        }

        //Borrar espacios al principio y al final
        //insertar manteniendo orden lexico

        bool ordenado = true;
        int indice = 0;

        if (tags.size() > 0){
            
            
            for(int l = 0; l< static_cast<int>(tags.size()); l++){
            
                if(texto == tags[l]){
                
                    return false;
                
                }

            }


            for(int orden = 1; orden < static_cast<int>(tags.size()) ; orden++){
            
                if(tags[orden -1] > tags[orden]){
                
                    ordenado = false;
                
                }
            
            }

            if(ordenado == true){

                for(size_t i = 0; i < tags.size(); i++){
                
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
                
                while(static_cast<int>(tags1.size()) > 0){

                    for(int z = 0; z < static_cast<int>(tags1.size()); z++){
                    
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

bool Resource :: hasTag(const string& tag) const{

    string texto = tag;
    
    for (char& c : texto){
        
        

        if(isalpha(c) && isupper(c)){
            
            c = tolower(c);
            
        }

            
    }

    if(texto.empty() == true){
    
        return false;

    }

    for(size_t i = 0; i< tags.size(); i++){
    
        if(tags[i] == texto){
        
            return true;
        
        }
    
    }

    return false;

}

bool Resource :: removeTag(const string& tag){

    string texto = tag;

    for (char& c : texto){
        
        

        if(isalpha(c) && isupper(c)){
            
            c = tolower(c);
            
        }

            
    }

    if(texto.empty()){
    
        return false;
    
    }

    if(!hasTag(texto)){
    
        return false;
    
    }



    size_t j = 0;
    
    int i = 0;
   
    while ( i <= 0 && j < tags.size()){

        if(tags[j] == texto){
        
            i++;

        }

        else{
        
        j++;

        }
        
    }

    if (i == 0){
    
        return false;
    
    }

    tags.erase(tags.begin() + j);


    return true;


}

 bool Resource :: operator==(const Resource& other) const{

    if(getId() == other.getId()){
    
        return true;
    
    }
    else{
    
        return false;
    
    }

}

bool Resource :: operator<(const Resource& other) const{

    if(getId() < other.getId()){
    
        return true;
    
    }

    else{
    
        return false;
    
    }

}
