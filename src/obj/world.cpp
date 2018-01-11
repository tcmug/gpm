
#include "world.hpp"
#include "object_base.hpp"


using ton::world;

// const char *world::lua_class_name = "_world";
// const lua_binding<world>::reg_type world::lua_method_register[] = {
//   { "spawn", &world::spawn },
//   { "add", &world::add },
//   { "test", &world::test },
//   { 0 }
// };


// implementation
const char *world::className = "_world";
const Luna <world>::FunctionType world::methods[] = {
    {"test", &world::test},
    {"spawn", &world::spawn},
    {0}
};

const Luna <world>::PropertyType world::properties[] = {
    {0}
};



void collision_handler(btDynamicsWorld *world, btScalar timeStep);


world::world() {

    broadphase = new btDbvtBroadphase();
    collision_configuration = new btDefaultCollisionConfiguration();
    dispatcher = new btCollisionDispatcher(collision_configuration);
    solver = new btSequentialImpulseConstraintSolver;
    dynamics_world = new btDiscreteDynamicsWorld(
        dispatcher,
        broadphase,
        solver,
        collision_configuration
    );
    dynamics_world->setGravity(btVector3(0, 0, 0));

    dynamics_world->setInternalTickCallback(collision_handler);
    // void* worldUserInfo=0,bool isPreTick=false);

    printf("world created\n");
}


world::world(lua_State *L) {
    printf("Not implemented\n");
    exit(1);
}


world::~world() {

    for (object_base *obj : objects) {
        delete obj;
    }

    delete dynamics_world;
    delete solver;
    delete dispatcher;
    delete collision_configuration;
    delete broadphase;

    printf("world destroyed\n");
}


int world::spawn(lua_State *L) {

//     glm::vec3 position;
//     position.x = lua_tonumber(L, -6);
//     position.y = lua_tonumber(L, -5);
//     position.z = lua_tonumber(L, -4);

//     glm::vec3 move;
//     move.x = lua_tonumber(L, -3);
//     move.y = lua_tonumber(L, -2);
//     move.z = lua_tonumber(L, -1);

//     voxel_object *obj = new voxel_object(position, move);
//     Luna <voxel_object>::push(L, obj);
//     obj->enter_world(this);

//     this->objects.push_back(obj);

    return 1;
}



int world::test(lua_State *L) {
    printf("XX top at %i\n", lua_gettop(L));
}


void world::lua_init() {

    lua_State *L = ::lua.get_state();

    // lua_newtable(L);
    // world **a = (world**)lua_newuserdata(L, sizeof(this));
    // *a = this;
    // lua_setfield(L, -2, "_this");

    // lua_pushcfunction(L, world_lua_callback);
    // lua_setfield(L, -2, "hello");

    // lua_pushcfunction(L, world_lua_callback);
    // lua_setfield(L, -2, "spawn");

    printf("top at %i >>\n", lua_gettop(L));

    LUA_BINDING_REGISTER(L, world);
    // LUA_BINDING_REGISTER(L, voxel_object);

    Luna <world>::push(L, this);
    lua_setglobal(L, "world");

    ::lua.dofile("scripts/init.lua");

    printf("<< top at %i\n", lua_gettop(L));
}





void collision_handler(btDynamicsWorld *world, btScalar timeStep) {

 // if (world)
 //   world->performDiscreteCollisionDetection();
    int i;
    ///one way to draw all the contact points is iterating over contact manifolds / points:
    int numManifolds = world->getDispatcher()->getNumManifolds();
    for (i = 0; i < numManifolds; i++) {
        btPersistentManifold* contactManifold = world->getDispatcher()->getManifoldByIndexInternal(i);
        const btCollisionObject* obA = static_cast<const btCollisionObject*>(contactManifold->getBody0());
        const btCollisionObject* obB = static_cast<const btCollisionObject*>(contactManifold->getBody1());
        int numContacts = contactManifold->getNumContacts();

        // SEGFAULT BUG CAUSED HERE.

        // event e;
        // theapp *app = dynamic_cast<theapp*>(app_base::current());

        // if (app) {
        //     app->get_world()->notify(e);
        // }
        return;
/*
        for (int j=0;j<numContacts;j++) {

            btManifoldPoint& contactPoint = contactManifold->getContactPoint(j);

            glm::vec3 va(-contactPoint.m_normalWorldOnB.getX(),-contactPoint.m_normalWorldOnB.getY(),-contactPoint.m_normalWorldOnB.getZ());
            glm::vec3 vb(contactPoint.m_normalWorldOnB.getX(),contactPoint.m_normalWorldOnB.getY(),contactPoint.m_normalWorldOnB.getZ());

            object_base *_voa = static_cast<object_base*>(obA->getUserPointer());
            object_base *_vob = static_cast<object_base*>(obB->getUserPointer());

            voxel_object *voa = dynamic_cast<voxel_object*>(_voa);
            voxel_object *vob = dynamic_cast<voxel_object*>(_vob);
            std::cout << contactPoint.m_appliedImpulse  << std::endl;

            if (voa && vob && contactPoint.m_appliedImpulse > 0.0001) {
                if (voa) {
                    voxel_object *vo = voa;
                    btVector3 bp = contactPoint.getPositionWorldOnA();
                    bp = vo->rigid_body->getCenterOfMassTransform().inverse()(bp);

                    const int size = 2;
                    for (int z = -size; z < (size + 1); z++)
                        for (int y = -size; y < (size + 1); y++)
                            for (int x = -size; x < (size + 1); x++) {
                                glm::vec3 p(bp.getX()+x, bp.getY()+y, bp.getZ()+z);
                                vo->put(p, 0);
                            }

                }

                if (vob) {
                    voxel_object *vo = vob;
                    btVector3 bp = contactPoint.getPositionWorldOnB();
                    bp = vo->rigid_body->getCenterOfMassTransform().inverse()(bp);

                    const int size = 2;
                    for (int z = -size; z < (size + 1); z++)
                        for (int y = -size; y < (size + 1); y++)
                            for (int x = -size; x < (size + 1); x++) {
                                glm::vec3 p(bp.getX()+x, bp.getY()+y, bp.getZ()+z);
                                vo->put(p, 0);
                            }

                }

                //
            }


            // btScalar angleX = normal.angle(btVector3(1,0,0));
            // btScalar angleY = normal.angle(btVector3(0,1,0));
            // btScalar angleZ = normal.angle(btVector3(0,0,1));

            // btScalar impulseX = contactPoint.m_appliedImpulse*cos(angleX);
            // btScalar impulseY = contactPoint.m_appliedImpulse*cos(angleY);
            // btScalar impulseZ = contactPoint.m_appliedImpulse*cos(angleZ);

            // btScalar forceX = impulseX/(timeStep);
            // btScalar forceY = impulseY/(timeStep);
            // btScalar forceZ = impulseZ/(timeStep);

            //std::cout << va.x << " " << va.y << " "  << va.z << " /" << vb.x << " " << vb.y<< " "  << vb.z<< " "  << std::endl;
            //void *ptr = this->rigid_body->setUserPointer
            //printf("Force: %8.6f %8.6f %8.6f %8.6f \n",(float)timeStep,forceX,forceY,forceZ);
        }
        */
    }
    //std::cout << "DONE" << std::endl;
    // //std::cout << "Hit" << std::endl;
    // btVector3 a = cp.getPositionWorldOnA();
    // btVector3 b = cp.getPositionWorldOnB();

    // glm::vec3 av(a.getX(), a.getY(), a.getZ());
    // glm::vec3 bv(b.getX(), b.getY(), b.getZ());

    // glm::vec3 d = bv - av;

    // if (cp.getLifeTime() <= 1) {
    //  std::cout << cp.getAppliedImpulse() << std::endl;
    // }

    // return false;
}


