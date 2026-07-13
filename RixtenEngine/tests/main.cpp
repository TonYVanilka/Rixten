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
    pool.set(0, {5870});
    
    printf("push back - %d\n", pool[0].x);
    //printf("set - %d\n", pool[8].x);
    //printf("set - %d\n", pool[7].x);

    printf("--------------------------------testing ECS\n");

    ECS ecs;

    Entity simpleEntity = ecs.createEntity();
    Entity simpleEntity1 = ecs.createEntity();
    Entity simpleEntity2 = ecs.createEntity();

    printf(" simple Entity created index %d, and generation %d \n", 
        simpleEntity.index, simpleEntity.generation);
    printf(" simple Entity1 created index %d, and generation %d \n",
           simpleEntity1.index, simpleEntity1.generation);
    printf(" simple Entity2 created index %d, and generation %d \n",
           simpleEntity2.index, simpleEntity2.generation);
    
    ecs.deleteEntity(simpleEntity);

    printf(" simple Entity getted index %d, and generation %d \n",
        ecs.getEntity(simpleEntity).index, ecs.getEntity(simpleEntity).generation);
    printf(" simple Entity1 getted index %d, and generation %d \n",
           ecs.getEntity(simpleEntity1).index, ecs.getEntity(simpleEntity1).generation);
    printf(" simple Entity2 getted index %d, and generation %d \n",
           ecs.getEntity(simpleEntity2).index, ecs.getEntity(simpleEntity2).generation);


}