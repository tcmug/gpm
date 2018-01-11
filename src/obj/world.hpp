#ifndef WORLD_HPP
#define WORLD_HPP

#include <vector>
#include <bullet/btBulletDynamicsCommon.h>

namespace ton {
    class world;
}

#include "object_base.hpp"
// #include "particle_array_base.hpp"

#include "../lua/common.hpp"
#include "../utils/observer.hpp"

namespace ton {

class world: public subject {

    public:

        world();
        world(lua_State *L);

        ~world();

        btBroadphaseInterface               *broadphase;
        btDefaultCollisionConfiguration     *collision_configuration;
        btCollisionDispatcher               *dispatcher;
        btSequentialImpulseConstraintSolver *solver;
        btDiscreteDynamicsWorld             *dynamics_world;

        std::vector <object_base*> objects;
        // std::vector <particle_array_base*> particles;

        void lua_init();
        int spawn(lua_State *L);
        int test(lua_State *L);
        int add(lua_State *L);

        LUA_BINDING(world);

};

}

#endif
