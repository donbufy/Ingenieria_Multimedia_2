#include <iostream>
#include "Resource.h"
#include <vector>
int main(){

Resource prueba;

prueba.addTag("Hola");
vector<string> tag;
tag = prueba.getTags();

for(string c: tag){

    cout << c << endl;

}



return 0;
}