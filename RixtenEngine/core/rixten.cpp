#include "rixten.h"
#include <cstdio>

RixtenEngine::RixtenEngine() {
    printf("Rixten engine created\n");
};

RixtenEngine::~RixtenEngine() {
    printf("Rixten engine destructed");
};

bool RixtenEngine::Init() {
    return true;
};

void RixtenEngine::Destruct() {
    printf("Rixten destructed");
};