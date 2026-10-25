// Cabecera de precompilación: reproduce el estado con el que las
// pruebas incluyen doctest.h para poder generar doctest_pch.h.gch
// y evitar reparsear la librería en cada prueba.
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "common/doctest.h"
