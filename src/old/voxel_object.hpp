#ifndef VOXEL_OBJECT_HPP
#define VOXEL_OBJECT_HPP

#include "object_base.hpp"
#include "../gfx/resource.hpp"
#include "../lua/lua.hpp"
#include "voxels.hpp"

#include <vector>


class voxel_object: public object_base {

	friend class ton::world;

    LUA_BINDING(voxel_object);

	private:

		enum AI_STATE {
			STOPPING,
			ENGAGING
		};

		voxel_mesh *mesh;
		GLuint texture;
		shader *material_shader;

		bool update;
		btCollisionShape *voxel_shape;

		glm::vec3 center_of_mass;

		float update_step;
		AI_STATE ai_state;

		object_base *target;
		glm::vec3 target_coord;

		void plot_course(glm::vec3 target);
		void update_center_of_mass();

		void init(glm::vec3 position, glm::vec3 move);

		// CONSTRUCTORS
		voxel_object(lua_State *L);
		voxel_object(glm::vec3 position, glm::vec3 move);
		virtual ~voxel_object();

		// std::vector <instruction*> mission;

	public:

		int lua_fill_box(lua_State *L);
		int lua_set_target(lua_State *L);

		void update_collision_shape();

		virtual void render(pipeline &pipe) const;
		virtual void step(float time_delta);
		void put(const glm::vec3 &world, int t);

};

#endif
