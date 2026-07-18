#include <cstdio>

#include "core/ecs/ecsManager.h"

struct vec1 {
    int x;
};

struct Vec1System : ISystem {
    void Update(ecsManager& handle, float dt) override;
};

int main() {

    printf("-----This is testing build of engine-----\n");


    printf("-------------------------------- testing ECS --------------------------------\n");

    ecsManager ecs;
    Vec1System vec1Sys;

    ecs.createPool<vec1>();
    Entity simple = ecs.createEntity();
    ecs.createComponent<vec1>(simple, vec1{5346});
    
    ecs.RegisterSystem(vec1Sys);

    ecs.Update();
}

void Vec1System::Update(ecsManager& handle, float dt) {
    auto vec1Comps = handle.getPool<vec1>().dense_components;

    for(int i = 0; i < vec1Comps.size(); i++) {
        printf("vec1Components %d\n", vec1Comps[i].x);
    }

}
