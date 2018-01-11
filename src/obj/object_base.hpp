#ifndef CORE_OBJECT_HPP
#define CORE_OBJECT_HPP

class object_base;

struct btDefaultMotionState;
class btRigidBody;
class btCollisionShape;
class pipeline;


#include "common.hpp"
#include "../gfx/shader.hpp"
#include "world.hpp"

class object_base {

	friend class ton::world;

	public:

		ton::world *the_world;

		glm::mat4x4 orientation;
		glm::vec3 world_position;
		glm::quat rotation;

		shader *material_shader;

		btDefaultMotionState *motion_state;
		btRigidBody          *rigid_body;
		btCollisionShape     *collision_shape;

		object_base();
		virtual ~object_base();

	public:


		virtual void update_collision_shape();
		virtual void render(pipeline &pipe) const;
		virtual void step(float time_delta);

		glm::vec3 get_world_position() const;
		glm::vec3 get_forward_direction() const;
		glm::vec3 get_up_direction() const;
		glm::vec3 get_right_direction() const;

		virtual void apply_local_force(const glm::vec3 &force, const glm::vec3 &pos);
		virtual void apply_local_impulse(const glm::vec3 &imp, const glm::vec3 &pos);

		virtual void enter_world(ton::world *new_world);
		virtual void leave_world();

  //       LUA_BINDING(object_base);

		// int getProperty(lua_State* L) {
		// 	lua_pushnumber(L, 1);
		// 	return 1;
		// }

		// int setProperty(lua_State* L) {
		// 	// lua_tonumber(L,-1);
		// 	return 0;
		// }

};



#endif
