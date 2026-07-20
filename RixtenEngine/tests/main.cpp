#include <cstdio>

#include "core/ecs/ecsManager.h"
#include "utils/logger.h"

struct vec1 {
    int x;
};

struct Vec1System : ISystem {
    void Update(ecsManager& handle, float dt) override;
};

int main() {

    LOG_DEBUG("Test logging");
    LOG_INFO("Test logging");
    LOG_WARN("Test logging");
    LOG_ERROR("Test logging");
    LOG_FATAL("Test logging");

    ecsManager ecs;
    Vec1System sys;

    Entity simple = ecs.createEntity();
    Entity simple1 = ecs.createEntity();
    ecs.createPool<vec1>();
    ecs.createComponent(simple, vec1{5632});
    ecs.createComponent(simple1, vec1{5523632});

    ecs.RegisterSystem(sys);

    ecs.Update();

    ecs.removeEntity(simple);

    ecs.Update();

    Entity simple2 = ecs.createEntity();

    ecs.createComponent(simple1, vec1{457823});

    ecs.Update();
}

void Vec1System::Update(ecsManager& handle, float dt) {
    auto& vec1Pool = handle.getPool<vec1>()->getComponents();
    
    for(int i = 0; i < vec1Pool.size(); i++) {
        LOG_INFO("vec 1 pool: ", i, " ", vec1Pool[i].x);
    }
}
