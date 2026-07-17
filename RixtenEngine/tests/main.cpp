#include <cstdio>

#include "core/ecs/componentPool.h"
#include "core/ecs/entity.h"
#include "core/memory/DArr.h"

struct vec1 {
    int x;
};

int main() {

    printf("-----This is testing build of engine-----\n");

    DArr<vec1> pool(20);

    pool.push_back({440});
    pool.set(0, {5870});
    
    printf("push back - %d\n", pool[0].x);
    //printf("set - %d\n", pool[8].x);
    //printf("set - %d\n", pool[7].x);

    printf("-------------------------------- testing ECS --------------------------------\n");

    ECS ecs;

    Entity simpleEntity = ecs.createEntity();
    Entity simpleEntity1 = ecs.createEntity();

    printf("simple entity = %d, %d \n", simpleEntity.index, simpleEntity.generation);
    printf("simple entity1 = %d, %d \n", simpleEntity1.index, simpleEntity1.generation);

    ComponentPool<vec1> vec1pool;

    vec1pool.addComponent(simpleEntity, {59});
    vec1pool.addComponent(simpleEntity1, {48});

    printf("vec1 component simpleEntity %d\n", vec1pool.getComponent(simpleEntity).x);
    printf("vec1 component simpleEntity1 %d\n", vec1pool.getComponent(simpleEntity1).x);

    vec1pool.removeComponent(simpleEntity);

    printf("vec1 component simpleEntity %d\n", vec1pool.getComponent(simpleEntity).x);
    printf("vec1 component simpleEntity1 %d\n", vec1pool.getComponent(simpleEntity1).x);

    ecs.deleteEntity(simpleEntity);

}