
#include "../gfx/pipeline.hpp"

#include "voxel_object.hpp"
#include "../core/resource.hpp"
#include "../core/vertex_array.hpp"

#include <iostream>

#include "voxels.hpp"

#include <bullet/btBulletDynamicsCommon.h>


const char *voxel_object::className = "_voxel_object";
const Luna <voxel_object>::FunctionType voxel_object::methods[] = {
    {"fill_box", &voxel_object::lua_fill_box},
    {"set_target", &voxel_object::lua_set_target},
    {0}
};

const Luna <voxel_object>::PropertyType voxel_object::properties[] = {
    {0}
};



int voxel_object::lua_fill_box(lua_State *L) {

    glm::vec3 position;
    position.x = lua_tonumber(L, -7);
    position.y = lua_tonumber(L, -6);
    position.z = lua_tonumber(L, -5);

    glm::vec3 extent;
    extent.x = lua_tonumber(L, -4);
    extent.y = lua_tonumber(L, -3);
    extent.z = lua_tonumber(L, -2);

    int type = lua_tonumber(L, -1);

	this->mesh->fill_box(
		position.x, position.y, position.z,
		extent.x, extent.y, extent.z,
		type
	);

	this->update = true;
	this->update_collision_shape();

	return 0;
}


int voxel_object::lua_set_target(lua_State *L) {

    object_base *obj = Luna<voxel_object>::check(L, -1);

    if (obj) {
    	this->target = obj;
    }

	return 0;
}



voxel_object::voxel_object(lua_State *L) {

    glm::vec3 position;
    position.x = lua_tonumber(L, -6);
    position.y = lua_tonumber(L, -5);
    position.z = lua_tonumber(L, -4);

    glm::vec3 move;
    move.x = lua_tonumber(L, -3);
    move.y = lua_tonumber(L, -2);
    move.z = lua_tonumber(L, -1);

	init(position, move);
}


voxel_object::voxel_object(glm::vec3 position, glm::vec3 move) {
	init(position, move);
	this->ai_state = STOPPING;
}


voxel_object::~voxel_object() {
	delete mesh;
	delete motion_state;
	delete collision_shape;
	delete voxel_shape;
	delete rigid_body;
	printf("deleted vo\n");
}


void voxel_object::init(glm::vec3 position, glm::vec3 move) {

	this->target = 0;
	this->update_step = 0;
	this->the_world = 0;
	this->collision_shape = 0;
	this->voxel_shape = 0;
	this->update = true;

	this->mesh = new voxel_mesh();
	//this->mesh->fill_box(4, 4, 4, 3, 3, 3, 1);

	this->mesh->fill_box(
		(this->mesh->size / 2) - 1,
		(this->mesh->size / 2) - 1,
		(this->mesh->size / 2) - 1,
		1, 1, 1,
		7);

	this->texture = resource_load_texture("terrain.png");
	this->material_shader = resource_load_shader(
		"shaders/solid.vert",
		"shaders/solid.geo",
		"shaders/solid.frag"
	);

	this->orientation = glm::mat4x4(1.0f);
	this->world_position = position;

	this->motion_state = new btDefaultMotionState(btTransform(
		btQuaternion(0, 0, 0, 1),
		btVector3(this->world_position.x, this->world_position.y, this->world_position.z)
	));

	this->collision_shape = new btCompoundShape();
	this->voxel_shape = new btBoxShape(btVector3(0.5, 0.5, 0.5));

	this->update_collision_shape();

	btScalar mass = this->mesh->solid_count;
	btVector3 inertia(0, 0, 0);
	this->collision_shape->calculateLocalInertia(mass, inertia);

	btRigidBody::btRigidBodyConstructionInfo info(
		mass,
		this->motion_state,
		this->collision_shape,
		inertia
	);
	info.m_restitution = 0.7f;

	this->rigid_body = new btRigidBody(info);
	this->rigid_body->setUserPointer(this);

	// Fix to XZ plane, rotation around Y
	this->rigid_body->setLinearFactor(btVector3(1,0,1));
	this->rigid_body->setAngularFactor(btVector3(0,1,0));

}


float get_acceleration_time(float distance, float acceleration) {
	return sqrt(distance / (acceleration * 0.5));
}


void voxel_object::plot_course(glm::vec3 target) {

	glm::vec3 direction_normal = target - this->get_world_position();
	float distance = glm::length(direction_normal);
	direction_normal = glm::normalize(direction_normal);

	glm::vec3 fwd = this->get_forward_direction();
	glm::vec3 rgt = this->get_right_direction();
	glm::vec3 up = this->get_up_direction();

	float fwd_dot = glm::dot(fwd, direction_normal);
	float rgt_dot = glm::dot(rgt, direction_normal);
	float up_dot = glm::dot(up, direction_normal);

	bool is_front = fwd_dot > 0;
	bool is_right = rgt_dot < 0;
	bool is_above = up_dot > 0;


	std::cout << "distance to target: " << distance << std::endl;
	std::cout << "fwd_dot: " << fwd_dot << std::endl;
	std::cout << "rgt_dot: " << rgt_dot << std::endl;
	std::cout << "up_dot: " << up_dot << std::endl;

/*
		const float mass = this->mesh->solid_count;
		const float F = mass * 10;
		const float r = 5;
		const float angle_to_turn = 90;
		const float D = 2 * 3.14159265359 * r * (angle_to_turn / 360.0);

		float a = F * this->rigid_body->getInvMass();
		float t1 = get_acceleration_time(D / 2, a);
		float t2 = get_acceleration_time(D / 2, a);

		instruction_forces *fo = new instruction_forces();
		fo->add_force(instruction_force(t1, glm::vec3(-r, 0, 0),  glm::vec3(0, 0, F)));
		fo->add_force(instruction_force(t1, glm::vec3(r, 0, 0),  glm::vec3(0, 0, -F)));
		this->mission.push_back(fo);

		fo = new instruction_forces();
		fo->add_force(instruction_force(t2, glm::vec3(-r, 0, 0),  glm::vec3(0, 0, -F)));
		fo->add_force(instruction_force(t2, glm::vec3(r, 0, 0),  glm::vec3(0, 0, F)));
		this->mission.push_back(fo);


	const float mass = this->mesh->solid_count;
	const float F = mass * 10;
	const float r = 5;
	const float angle_to_turn = 90;
	const float D = 10;

	float a = F * this->rigid_body->getInvMass();
	float t1 = get_acceleration_time(D / 2, a);
	float t2 = get_acceleration_time(D / 2, a);

	std::cout << "F: " <<  F << " D: " << D << " a: " << a << " mass: " << mass << " t: " << t1 << std::endl;
		// D / 2 = (A * t ^ 2) / 2

	std::cout << "AT: " << this->get_world_position().z << std::endl;
	instruction_forces *fo = new instruction_forces();
	fo->add_force(instruction_force(t1, glm::vec3(0, 0, 0),  glm::vec3(0, 0, F)));
	this->mission.push_back(fo);

	fo = new instruction_forces();
	fo->add_force(instruction_force(t2, glm::vec3(0, 0, 0),  glm::vec3(0, 0, -F)));
	this->mission.push_back(fo);
*/

}

void voxel_object::step(float time_delta) {

	this->rigid_body->setDamping(0, 0.9);

	if (this->target) {

		static bool d = false;
		if (!d) {
			d = true;
/*
			for (int i = 0; i < 4; i++) {
				const float mass = this->mesh->solid_count;
				const float F = mass * 10;
				const float r = 5;
				const float angle_to_turn = 90;
				const float D = 2 * 3.14159265359 * r * (angle_to_turn / 360.0);

				float a = F * this->rigid_body->getInvMass();
				float t1 = get_acceleration_time(D / 2, a);
				float t2 = get_acceleration_time(D / 2, a);

				std::cout << "F: " <<  F << " D: " << D << " a: " << a << " mass: " << mass << " t: " << t1 << std::endl;
	 			// D / 2 = (A * t ^ 2) / 2

				std::cout << "AT: " << this->get_world_position().z << std::endl;
				instruction_forces *fo = new instruction_forces();
				fo->add_force(instruction_force(t1, glm::vec3(-r, 0, 0),  glm::vec3(0, 0, F)));
				fo->add_force(instruction_force(t1, glm::vec3(r, 0, 0),  glm::vec3(0, 0, -F)));
				this->mission.push_back(fo);

				fo = new instruction_forces();
				fo->add_force(instruction_force(t2, glm::vec3(-r, 0, 0),  glm::vec3(0, 0, -F)));
				fo->add_force(instruction_force(t2, glm::vec3(r, 0, 0),  glm::vec3(0, 0, F)));
				this->mission.push_back(fo);
			}

			const float mass = this->mesh->solid_count;
			const float F = mass * 10;
			const float r = 5;
			const float angle_to_turn = 90;
			const float D = 10;

			float a = F * this->rigid_body->getInvMass();
			float t1 = get_acceleration_time(D / 2, a);
			float t2 = get_acceleration_time(D / 2, a);

			std::cout << "F: " <<  F << " D: " << D << " a: " << a << " mass: " << mass << " t: " << t1 << std::endl;
 			// D / 2 = (A * t ^ 2) / 2

			std::cout << "AT: " << this->get_world_position().z << std::endl;
			instruction_forces *fo = new instruction_forces();
			fo->add_force(instruction_force(t1, glm::vec3(0, 0, 0),  glm::vec3(0, 0, F)));
			this->mission.push_back(fo);

			fo = new instruction_forces();
			fo->add_force(instruction_force(t2, glm::vec3(0, 0, 0),  glm::vec3(0, 0, -F)));
			this->mission.push_back(fo);
*/

		}

		this->update_step += time_delta;

		glm::vec3 position = this->get_world_position();
		glm::vec3 target_pos = this->target->get_world_position();
		glm::vec3 temp = target_pos - position;
		glm::vec3 dir = glm::normalize(temp);
		float distance = glm::length(temp);

		glm::vec3 fwd = this->get_forward_direction();
		glm::vec3 rgt = this->get_right_direction();
		glm::vec3 up = this->get_up_direction();

		float fwd_dot = glm::dot(fwd, dir);
		float rgt_dot = glm::dot(rgt, dir);
		float up_dot = glm::dot(up, dir);

		bool is_front = fwd_dot > 0;
		bool is_right = rgt_dot < 0;
		bool is_above = up_dot > 0;

		glm::vec3 to_turn(0, 0, 0);

		btVector3 rot = this->rigid_body->getAngularVelocity();
		btVector3 vel = this->rigid_body->getLinearVelocity();

		vel = this->rigid_body->getWorldTransform().getBasis().inverse() * vel;

		if (distance > 0) {
			this->ai_state = ENGAGING;
		} else {
			this->ai_state = STOPPING;
		}

		const float EPSILON = 0.1;

		switch (this->ai_state) {
			case STOPPING:
				// if (rot.length() > 0.1) {
				// 	if (rot.getY() > 0) {
				//         this->apply_local_force(glm::vec3(-5000, 0, 0), glm::vec3(0, 0, 10));
				//         this->apply_local_force(glm::vec3(5000, 0, 0), glm::vec3(0, 0, -10));
				// 	} else if (rot.getY() < 0) {
				//         this->apply_local_force(glm::vec3(-5000, 0, 0), glm::vec3(0, 0, -10));
				//         this->apply_local_force(glm::vec3(5000, 0, 0), glm::vec3(0, 0, 10));
				// 	}
				// }
				// else {
				// 	if (vel.getZ() > 0) {
				// 		this->apply_local_force(
				//             glm::vec3(-vel.getX()*1000, 0, -vel.getZ()*1000),
				//             glm::vec3(0, 0, 0)
				//         );
				// 	} else if (vel.getZ() < 0) {
				// 		this->apply_local_force(
				//             glm::vec3(-vel.getX()*1000, 0, -vel.getZ()*1000),
				//             glm::vec3(0, 0, 0)
				//         );
				// 	}
				// }
				// std::cout << "stoooopit" << std::endl;
			break;

			case ENGAGING:
				while (this->mission.size() > 0 &&
					!((*this->mission.begin())->apply(this, time_delta))) {
					delete (*this->mission.begin());
					this->mission.erase(this->mission.begin());
				}


				/*if (fwd_dot < 1 - EPSILON) {
					if (is_right &&) {

				        this->apply_local_force(
				            glm::vec3(-8000, 0, 0),
				            glm::vec3(0, 0, 10)
				        );
				        this->apply_local_force(
				            glm::vec3(8000, 0, 0),
				            glm::vec3(0, 0, -10)
				        );

						//this->rigid_body->applyTorque(btVector3(0, -100000, 0));
					} else {
						//this->rigid_body->applyTorque(btVector3(0, 100000, 0));
				        this->apply_local_force(
				            glm::vec3(8000, 0, 0),
				            glm::vec3(0, 0, 10)
				        );
				        this->apply_local_force(
				            glm::vec3(-8000, 0, 0),
				            glm::vec3(0, 0, -10)
				        );

					}
				}*/
				//this->rigid_body->setAngularVelocity(rot);

				// if (abs(rot.getY()) < 0.1) {

				// 	if (is_above && rot.getX() > -1) {
				//         this->apply_local_force(
				//             glm::vec3(0, -5000, 0),
				//             glm::vec3(0, 0, 10)
				//         );
				//         this->apply_local_force(
				//             glm::vec3(0, 5000, 0),
				//             glm::vec3(0, 0, -10)
				//         );

				// 	} else if (rot.getX() < 1) {

				//         this->apply_local_force(
				//             glm::vec3(0, 5000, 0),
				//             glm::vec3(0, 0, 10)
				//         );
				//         this->apply_local_force(
				//             glm::vec3(0, -5000, 0),
				//             glm::vec3(0, 0, -10)
				//         );
				//   	}
			 //    }

			break;
		}

		if (this->update_step > 2) {

			this->plot_course(this->target->get_world_position());

			std::string where;
			where += std::to_string(this->get_world_position().z);
			where += " ";
			if (is_front) {
				where += "FRONT ";
			} else {
				where += "BEHIND ";
			}

			if (is_right) {
				where += "RIGHT ";
			} else {
				where += "LEFT ";
			}

			if (is_above) {
				where += "ABOVE ";
			} else {
				where += "BELOW ";
			}


			float radians = acos(fwd_dot);

			// Write a queue of instructions called "mission" such as
			// 	 - apply Z thrust +X for x seconds
			// 	 - wait for D seconds
			// 	 - apply Z thrust -X for x seconds
			// 	 in any event that breaks "mission" => recalculate
			std::cout << where << std::endl;

			// btVector3 tor = this->rigid_body->getTotalTorque();
			// std::cout << adj.x << " " << adj.y << " " << adj.z << " mass:" << this->rigid_body->getInvMass() << std::endl;
			// std::cout << glm::dot(fwd, dir) << " - right - " << glm::dot(rgt, dir) << std::endl;
			// std::cout << dir.x << " " << dir.y << " " << dir.z << " / " << fwd.x << " " << fwd.y << " " << fwd.z << std::endl;
			this->update_step -= 2;


    // if (selected_object && keystate[0]) {
    //     selected_object->apply_local_force(
    //         glm::vec3(-5000, 0, 0),
    //         glm::vec3(0, 0, 10)
    //     );
    //     selected_object->apply_local_force(
    //         glm::vec3(5000, 0, 0),
    //         glm::vec3(0, 0, -10)
    //     );
    // }

    // if (selected_object && keystate[1]) {
    //     selected_object->apply_local_force(
    //         glm::vec3(-5000, 0, 0),
    //         glm::vec3(0, 0, -10)
    //     );
    //     selected_object->apply_local_force(
    //         glm::vec3(5000, 0, 0),
    //         glm::vec3(0, 0, 10)
    //     );
    // }


		}
	}
}

void voxel_object::update_collision_shape() {

	if (!this->update)
		return;

	this->update_center_of_mass();

	unsigned long start_time = SDL_GetTicks();

	btVector3 center;

	btCompoundShape *compound_shape = dynamic_cast<btCompoundShape*>(this->collision_shape);
	compound_shape->removeChildShape(this->voxel_shape);

	for (int z = 0; z < this->mesh->size; z++) {
		for (int y = 0; y < this->mesh->size; y++) {
			for (int x = 0; x < this->mesh->size; x++) {
				if (this->mesh->_get(x, y, z) > 0) {
					btTransform t;  //position and rotation
					t.setIdentity();
					t.setOrigin(btVector3(
						x - this->center_of_mass.x + 0.5f,
						y - this->center_of_mass.y + 0.5f,
						z - this->center_of_mass.z + 0.5f
					));
					compound_shape->addChildShape(t, this->voxel_shape);
				}
			}
		}
	}

	if (this->the_world) {
		this->the_world->dynamics_world->removeRigidBody(this->rigid_body);
		delete this->rigid_body;

		btScalar mass = this->mesh->solid_count;
		btVector3 inertia(0, 0, 0);
		this->collision_shape->calculateLocalInertia(mass, inertia);

		btRigidBody::btRigidBodyConstructionInfo info(
			mass,
			this->motion_state,
			this->collision_shape,
			inertia
		);
		info.m_restitution = 0.7f;

		this->rigid_body = new btRigidBody(info);
		this->rigid_body->setUserPointer(this);

		// Fix to XZ plane, rotation around Y
		this->rigid_body->setLinearFactor(btVector3(1,0,1));
		this->rigid_body->setAngularFactor(btVector3(0,1,0));

		this->the_world->dynamics_world->addRigidBody(this->rigid_body);
		std::cout << "updated inertia: " << mass << std::endl;
	}
   // printf("Collision shape generation time: %u\n", SDL_GetTicks() - start_time);

    this->update = false;

}

void voxel_object::update_center_of_mass() {

	this->center_of_mass = glm::vec3(0, 0, 0);
	int total = 0;
	for (int i = 0; i < this->mesh->size; i++) {
		for (int j = 0; j < this->mesh->size; j++) {
			for (int k = 0; k < this->mesh->size; k++) {
				if (this->mesh->get(i, j, k) > 0) {
					this->center_of_mass.x += i + 0.5f;
					this->center_of_mass.y += j + 0.5f;
					this->center_of_mass.z += k + 0.5f;
					total++;
				}
			}
		}
	}

	this->center_of_mass /= total;

}


void voxel_object::render(pipeline &pipe) const {

	this->material_shader->enable();

	glBindAttribLocation(shader::active->program, 0, "vertex");
	glBindAttribLocation(shader::active->program, 1, "normal");
	glBindAttribLocation(shader::active->program, 2, "uv");
	glUniform1i(shader::active->var_loc("diffuse_texture"), 1);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, this->texture);

	if (pipe.camera)
		pipe.camera->gl_bind();

	btTransform trans;
	this->motion_state->getWorldTransform(trans);

	glm::vec3 pos(
		trans.getOrigin().getX(),
		trans.getOrigin().getY(),
		trans.getOrigin().getZ()
	);

	btQuaternion rot = trans.getRotation();

	glm::quat rota(rot.getW(), rot.getX(), rot.getY(), rot.getZ());

	glm::mat4x4 rotate = glm::mat4_cast(rota);
	glm::mat4x4 scale(1.0f);
	glm::mat4x4 translate = glm::translate(glm::mat4x4(1.0f), pos);

	glm::mat4x4 center = glm::translate(
		glm::mat4x4(1.0f),
		glm::vec3(
			-this->center_of_mass.x,
			-this->center_of_mass.y,
			-this->center_of_mass.z
		)
	);

	glm::mat4x4 model = translate * rotate * center * scale;

	gl_uniform("model", model);

	this->mesh->render();

	this->material_shader->disable();

}


void voxel_object::put(const glm::vec3 &world, int t) {
	glm::vec3 p(
		(world.x + this->center_of_mass.x) + 0.5f,
		(world.y + this->center_of_mass.y) + 0.5f,
		(world.z + this->center_of_mass.z) + 0.5f
	);

	this->mesh->put(p.x, p.y, p.z, t);
	this->update = true;
}




