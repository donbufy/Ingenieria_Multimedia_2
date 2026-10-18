#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "common/doctest.h"

TEST_CASE("Z04: variable_no_inicializada"){

    int numero = -5;

    bool es_positivo;

    if(numero > 0){
    
        es_positivo = true;

    }

    CHECK(es_positivo == false);

}
