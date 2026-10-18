// Práctica 1 — Catálogo de Recursos
// Implementación del TAD Catalog.

#include "Catalog.h"

// TODO: implementa aquí los métodos declarados en Catalog.h
Catalog::Catalog(){



}
    Catalog::Catalog(const Catalog& other) : data(other.data){

    }




    Catalog& Catalog :: operator=(const Catalog& other){

        data = other.data;

        return *this;

    }
    Catalog :: ~Catalog(){



    }
    bool Catalog :: add(const Resource& r){



    if(r.getId().empty()){

        return false;

    }

    if(!findById(r.getId()).getId().empty()){

        return false;

    }
    for(Resource t : data){

        if (r == t){

            return false;

        }

    }

    data.push_back(r);

    return true;

    }

Resource Catalog :: findById(const string& id) const{

    int u = -1;

   for(int i = 0; i < static_cast<int>(data.size()); i++){

        if(data[i].getId() == id){

            u = i;

        }

   }

    if(u >= 0){

        return data[u];

    }

    else{

        Resource r1;

        return r1;

    }


}


bool Catalog :: removeById(const string& id){

    if(findById(id).getId().empty()){

        return false;

    }

    else{

        int indice = -1;

        for(int i = 0; i < static_cast<int>(data.size()); i++){

            if(data[i].getId() == id){

                indice = i;

            }

        }

        if(indice >= 0){

            data.erase(data.begin() + indice);

            return true;

        }
        else{

            return false;

        }

    }

}

vector<Resource> Catalog :: findByTag(const string& tag) const{

    vector<Resource> r1 = data;
    vector<Resource> r2;

    string texto = tag;

    for (char& c : texto){



        if(isalpha(c) && isupper(c)){

            c = tolower(c);

        }


    }

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

        vector<Resource> r3;
        return r3;

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
    //fin de normalizar

    for(int i = 0 ; i < static_cast<int>(r1.size()); i++){

        for(int j = 0; j < static_cast<int>(r1[i].getTags().size()); j++){

            if(r1[i].getTags()[j] == texto){

                r2.push_back(r1[i]);

                j = static_cast<int>(r1[i].getTags().size() -1 );

            }

        }

    }

    vector<Resource> r3;

    if(r2.size() > 0){

        Resource recurso = r2[0];

        int z = 0;

        while(r2.size() >= 1){

            for(size_t i = 0; i < r2.size(); i++){

                if(r2[i] < recurso){

                    recurso = r2[i];

                    z = static_cast<int>(i);

                }

            }

            r3.push_back(recurso);

            r2.erase(r2.begin() + z);
            if(r2.size()>0){

                recurso = r2[0];
                z = 0;
            }

        }


    }





    return r3;

}
    int Catalog::size() const{

        int numero = 0;

        numero = static_cast <int>(data.size());

        return numero;

    }

    bool Catalog :: loadFromFile(const string& filename){

        ifstream fichero(filename);

        if(!fichero.is_open()){

            return false;

        }

        if(size() > 0){

            for(Resource recurso : data){

                removeById(recurso.getId());

            }

        }

        string linea;

        string id;
        string year;
        string title;
        string tags;
        string tag;

        while(getline(fichero, linea)){

            stringstream ss(linea);


            if(getline(ss,id,'|') && getline(ss, year, '|') && getline(ss, title, '|')){

                if(!id.empty()){

                    

                    getline(ss,tags);
                    Resource r1(id,stoi(year),title);

                    int senyal_mala = 0;

                    if(!tags.empty()){

                        stringstream sss(tags);


                        int senyal_buena = 0;

                        for(char c : tags){

                            if (c == '|'){

                                senyal_mala = -1;

                            }

                            if (c == ','){

                                senyal_buena = -1;

                            }

                        }
                        if(senyal_buena == -1){

                            while(getline(sss,tag,',')){

                                r1.addTag(tag);

                            }
                        }
                    }

                    if(senyal_mala != -1){

                        add(r1);

                    }

                }



            }



        }

        return true;


    }
