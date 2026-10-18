#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main(){

    string texto = "  hola";
    cout << texto<< texto<< endl;

    int indice = -1;
        int i = 0;
        
        while(i < texto.size() && texto[i] == ' '){
        
            indice++;
            i++;
        
        }

        if (indice >= 0){
        
            texto.erase(0, indice  + 1);
        
        }

        cout<<texto << texto<< endl;

        i = texto.size() - 1;
        indice = -1;

        while(i >= 0 && texto[i] == ' '){
        
            indice++;
            i--;
        
        }

        if (indice >= 0){
        
            texto.erase(texto.size() - indice - 1, indice  + 1);
        
        }

        cout << texto << texto;


        string texto1 = "avion";

        string texto2 = "barco";

        string texto3 = "jardin";

        vector <string> tags; 

        tags.push_back(texto1);
        tags.push_back(texto2);
        tags.push_back(texto3);
        indice = 0;

        if (tags.size() > 0){
        
            for(i = 0; i < tags.size(); i++){
            
                if(texto > tags[i]){
                
                    indice++;
                
                }
                else{
                    
                    i = tags.size() - 1;
                
                }
            
            }

            tags.insert(tags.begin() + indice, texto);

            for( const string& tag : tags ){
            
                cout << tag<< " ";
            
            }
        
        }
        


    return 0;

}
