// Práctica 2 — Reproductor de música
// Implementación de la clase Track.

#include "Track.h"
using namespace std;

ostream&  operator<<(ostream& os, const Track& t){

    os << t.getId() << " (" << t.formatDuration() << ") " << t.getTitle();

    return os;

}


Track::Track(): id(""), title(""), duration(0){}

Track::Track(const string& id, const string& title, const int duration){

    this->id = id;
    this->title = title;

    int duracion = duration;

    if(duration < 0){
    
        duracion = 0;
    
    } 

    this->duration = duracion;

}

Track::Track(const Track& other): id(other.getId()), title(other.getTitle()), duration(other.getDuration()){}



Track& Track::operator=(const Track& other){

    id = other.id;
    title = other.title;
    duration = other.duration;

    return *this;

}

string Track::getId() const{

    return id;

}
string Track::getTitle() const{

    return title;

}
int Track::getDuration() const{

    return duration;

}
string Track::formatDuration() const{

    int minutos = 0;
    int segundos =  duration;
    int duracion =  duration;

    while(duracion >= 60){
    
        minutos ++;
        segundos = segundos - 60;
        duracion -= 60;
    
    }

    string resultado = to_string(minutos) + ":";

    if (segundos >= 10){
    
        resultado += to_string(segundos);
    
    }

    else{
    
        resultado += '0' + to_string(segundos);
    
    }

    

    return resultado;

}
    bool Track:: operator ==(const Track& other){

        if(!id.empty() && !other.getId().empty()){
        
            if(id == other.getId()){
            
                return true;
            
            }
        
        }
        
        return false;
        
    }

