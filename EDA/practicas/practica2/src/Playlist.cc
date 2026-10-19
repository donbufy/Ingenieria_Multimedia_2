// Práctica 2 — Reproductor de música
// Implementación del TAD Playlist.
//
// Recuerda: cada nodo se crea con `new` y debe liberarse exactamente
// una vez con `delete`. El destructor, el constructor de copia y el
// operador de asignación deben dejar la lista en un estado coherente.

#include "Playlist.h"

// TODO: implementa aquí los métodos declarados en Playlist.h


ostream& operator<<(ostream& os, const Playlist& p){

    if(p.size() == 0){

        return os;

    }

    else{
    
        Track cansion;

        for(int i = 0; i < p.size(); i++){
            
            cansion = *(p.get(i));

            if(cansion == *p.getCurrent()){
            
                os << "> ";
            
            }
            else{
            
                os << "  ";
            
            }

            os << i + 1 << ". " << cansion.getId() << " (" << cansion.formatDuration() << ") " << cansion.getTitle() << endl;

            
            
        }

        return os;
    
    }



}


void Playlist:: copy(const Playlist &other){

    clear();

    if(other.count > 0){
        
        Playlist:: Node* temporal = other.head;

        Playlist:: Node* nodo2 = new Node(temporal->track);

        Playlist::Node* currente = other.current;

        if(other.count == 1){
        
            

            nodo2->next = nodo2;
            nodo2->prev = nodo2;
            
            head = nodo2;
            tail = nodo2;
            current = nodo2;
            count = 1;
        
        }

        else{

            if(temporal == currente){
                
                current = nodo2;
                
            }

            temporal = temporal->next;
            head = nodo2;
            count = 1;

            

            for(int i = 2; i<=other.count;i++){
                
                
                Playlist::Node* nodo3 = new Node(temporal->track);


                

                nodo2->next = nodo3;
                nodo3->prev = nodo2;

                if(temporal == currente){
                
                    current = nodo3;
                
                }

                if(i == other.count){
                
                    tail = nodo3;
                
                }

                nodo2 = nodo3;
                temporal = temporal->next;
                count++;
                
            
            }

            head->prev = tail;
            tail->next = head;

        }
        
        
    }



}

Playlist::Playlist(): head(nullptr), tail(nullptr), current(nullptr), count(0)
{
}
Playlist::Playlist(const Playlist& other){

    copy(other);

}
Playlist& Playlist::operator = (const Playlist& other){

    copy(other);

    return *this;

}

Playlist::~Playlist(){

    clear();

}

int Playlist::size() const{

    return count;

}

bool Playlist:: empty() const{

    if(count == 0){
    
        return true;
    
    }

    else{
    
        return false;
    
    }

}

bool Playlist:: add(const Track& t ){



    if(t.getId().empty()){
    
        return false;
    
    }
    
    if(count == 0){
    
        Playlist::Node* cansion = new Node(t);

        cansion->next = cansion;

        cansion->prev = cansion;

        head = cansion;
        tail = cansion;
        current = cansion;
        count = 1;

        return true;

    }

    Playlist::Node* temporal = head;
    
    for(int i = 0; i< count; i++){
    
        if(temporal->track.getId() == t.getId()){
        
            return false;
        
        }
        if(i != count -1 ){
        
            temporal = temporal->next;
        
        }
        

    }

    

    Playlist::Node* cansion = new Node(t);

    tail->next = cansion;

    cansion->prev = tail;

    cansion->next = head;

    tail = cansion;

    head->prev = cansion;



    count++;

    return true;

}


bool Playlist:: insertAt(int pos, const Track& t){

    
    if(pos == count){
    
        return add(t);
    
    }

    Node* temporal = head;
    
    Node* cansion= head;

    for(int i = 0; i < count; i++){
    

        if(t.getId() == temporal->track.getId()){
        
            return false;
        
        }

        if(i == pos){
        
            cansion = temporal;
        
        }
        
        if(i != count){
        
            temporal = temporal->next;
        
        }

    }

    Node* nueva = new Node(t);

    nueva->prev = cansion->prev;
    nueva->next = cansion;

    (cansion->prev)->next = nueva;
    cansion->prev = nueva;
    count++;

    if(pos == 0){
    
        head = cansion;
    
    }
    return true;
}

bool Playlist:: removeById(const string& id){

    if(count == 0 || id.empty() || findById(id) == nullptr){
    
        return false;
    
    }

    Node* temporal = head;

    Node* cansion = nullptr;

    for(int i = 0; i < count; i++){
    
        if(temporal->track.getId() == id){
        
            cansion = temporal;
        
        }

        if(i != count -1){
        
            temporal = temporal->next;
        
        }

    }

    temporal = cansion->prev;

    (cansion->prev)->next = cansion->next;

    (cansion->next)->prev = cansion->prev;


    if(cansion == head){
    
        head = cansion->next;
    
    }

    if(cansion == tail){
    
        tail = cansion->prev;
    
    }

    if (cansion == current){
    
        if(cansion->next == head){
        
            current = cansion->prev;
        
        }
        else{
        
            current = cansion->next;
        
        }
    
    }

    delete cansion;
    
    if(findById(id) == nullptr){
    
        return true;
    
    }
    else{
    
        return false;
    
    }
        
    
}

bool Playlist:: load(const string& filename){

    ifstream fichero(filename);

    if(!fichero.is_open()){
    
        return false;
    
    }
    clear();

    string linea = "";

    while(getline(fichero,linea)){
        

        string id = "";
        string duration = "";
        string title = "";
        int duracion = -1;

        stringstream ss(linea);

        if(getline(ss,id,'|') && getline(ss,duration,'|') && getline(ss,title)){
        
            try{ 
                
                duracion = stoi(duration);

            
            }
            catch(const invalid_argument&){
            
                id = empty();

            }
            catch(const out_of_range&){
            
                id = empty();

            }

            if(!id.empty() && duracion >= 0){
            
                Track track(id,title,duracion);
                add(track);
                if(track == head->track){
                
                    current = head;
                
                }
            }
        }
    
    }


    return true;

}

int Playlist:: merge(const Playlist& other){

    int antes = count;

    if(other.count != 0){
    
        int cuenta_other = other.count;

            Node* temp = other.head;

        for(int i = 0; i< cuenta_other; i++){
        
            add(temp->track);

            if (i != cuenta_other -1){

                temp = temp->next;
        
            }

        }
    
    }

    return (count - antes);

}

void Playlist::clear(){

    if (count > 0){
    
        Playlist:: Node* temporal = head->next;

        while(count > 0){
            
            delete head;
            count --;
            if(count > 0){

                head = temporal;

                if(count > 1){

                    temporal = head->next;

                }

            }

        }
    
    }

}

const Track* Playlist:: get(int pos) const{

    if(count == 0 || pos >= count || pos < 0){
    
        return nullptr;
    
    }
    

    Node* temp = head;

    for(int i = 0; i <= pos; i++){
    
        if(i == pos){
        
            return &(temp->track);
        
        }
        else{
        
            temp = temp->next;
        
        }
    
    }

    return nullptr;

}





const Track* Playlist:: findById(const string& id) const{

    if(count == 0 || id.empty()){
    
        return nullptr;
    
    }

    Node* temporal = head;

    for(int i = 0; i < count; i++){
    
        if(temporal->track.getId() == id){
        
            return &(temporal->track);
        
        }

        if(i != count -1){
        
            temporal = temporal->next;
        
        }


    }

    return nullptr;
}

const Track* Playlist:: getCurrent() const{

    if(current != nullptr){
    
        return &(current->track);
    
    }
    else{
    
        return nullptr;
    
    }

}

bool Playlist:: next(){

    if(count == 0){
    
        return false;
    
    }
    
    else{
    
        current = current->next;

        return true;
    
    }

}

bool Playlist:: prev(){

    if(count == 0){
    
        return false;
    
    }

    else{
    
        current = current->next;
        
        return true;
        
    }

}

int Playlist:: totalDuration() const{

    if(count == 0){
    
        return 0;
    
    }

    Node* temporal = head;

    int segundos;

    while(!(temporal->track == tail->track)){
    
        segundos += temporal->track.getDuration();
        
        temporal = temporal->next;
    
    }

    segundos+= tail->track.getDuration();

    return segundos;

}
