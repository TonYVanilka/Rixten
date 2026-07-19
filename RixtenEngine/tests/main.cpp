#include <cstdio>

#include "core/ecs/ecsManager.h"

struct vec1 {
    int x;
};

struct vec2 {
    int x;
};

struct Vec1System : ISystem {
    void Update(ecsManager& handle, float dt) override;
};

struct Vec2System : ISystem {
    void Update(ecsManager& handle, float dt) override;
};

int main() {

    printf("-----This is testing build of engine-----\n");

    printf("-------------------------------- testing ECS --------------------------------\n");

    ecsManager ecs;
    Vec1System vec1Sys;
    Vec2System vec2Sys;

    printf("pool index %d \n", ecs.createPool<vec1>());
    ecs.createPool<vec2>();

    Entity simple = ecs.createEntity();
    Entity simple1 = ecs.createEntity();

    ecs.createComponent<vec1>(simple, vec1{1});
    ecs.createComponent<vec1>(simple1, vec1{2});

    ecs.createComponent<vec2>(simple1, vec2{2});

    ecs.RegisterSystem(vec1Sys);
    ecs.RegisterSystem(vec2Sys);

    ecs.Update();
}

void Vec1System::Update(ecsManager& handle, float dt) {
    auto& vec1Comps = handle.getPool<vec1>()->getComponents();

    for(int i = 0; i < vec1Comps.size(); i++) {
        printf("vec1Components %d\n", vec1Comps[i].x);
    }
}

void Vec2System::Update(ecsManager& handle, float dt) {
    auto& vec2Comps = handle.getPool<vec2>()->getComponents();
    auto& vec1Comps = handle.getPool<vec1>()->getComponents();

    for (int i = 0; i < vec2Comps.size(); i++) {
        vec2Comps[i].x += vec1Comps[i].x;
        printf("vec2Components %d\n", vec2Comps[i].x);
    }
}
