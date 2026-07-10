#include <cstdio>
#include "core/memory/DArr.h"
#include "core/ecs/ecs.h"

struct vec1 {
    int x;
    ~vec1() {
        printf("destructor vec1 have called\n");
    }
};

int main() {

    printf("-----This is testing build of engine-----\n");

    DArr<vec1> pool(20);

    pool.push_back({440});
    pool.set(8, {5870});
    
    printf("push back - %d\n", pool[0].x);
    printf("set - %d\n", pool[8].x);
    printf("set - %d\n", pool[7].x);

    printf("testing ECS\n");

    ECS ecs;

    Entity simpleEntity = ecs.createEntity();
    
    // i have wrong value there
    printf(" simple Entity created index %d, and generation %d \n", 
        simpleEntity.index, simpleEntity.generation);
    // end nullptr there
    printf(" simple Entity getted index %d, and generation %d \n",
        ecs.getEntity(simpleEntity).index, ecs.getEntity(simpleEntity).generation);

    ecs.deleteEntity(simpleEntity);


}