#include <cstdio>
#include "rixten.h"

int main() {

    RixtenEngine engine;
    printf("engine init: %d \n", engine.Init());

    printf("hello from launcher\n");

    engine.Destruct();
}