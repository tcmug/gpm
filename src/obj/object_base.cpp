
#include "../gfx/pipeline.hpp"

#include "object_base.hpp"
#include "../gfx/vertex_array.hpp"
#include "../gfx/resource.hpp"

#include <iostream>
#include <bullet/btBulletDynamicsCommon.h>

glm::vec3 triangle[3] = {
	{0.0f, 1.0f, 0.0f},
	{1.0f,-1.0f,0.0f},
	{-1.0f,-1.0f,0.0f}
};

GLuint vao = -1;
glm_vec3_buffer *buf = 0;


void gl_uniform(const char *name, const glm::mat4x4 &p) {
	GLint loc = glGetUniformLocation(
		shader::active->program,
		name
	);
	engine_gl_check();

	glUniformMatrix4fv(
		loc,
		1,
		GL_FALSE,
		glm::value_ptr(p)
	);
	engine_gl_check();
}


object_base::object_base() {

	this->the_world = 0;
	this->material_shader = resource_load_shader("shaders/flat.vert", 0, "shaders/flat.frag");

	if (buf == 0) {
		buf = new glm_vec3_buffer();

		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		buf->gl_vao_bind(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		glm::vec3 v[12];

		v[0].x = 0.000;   v[0].y =  1.000;  v[0].z = 0.000;    // Top-most point.
		v[1].x = 0.894;   v[1].y =  0.447;  v[1].z = 0.000;
		v[2].x = 0.276;   v[2].y =  0.447;  v[2].z = 0.851;
		v[3].x = -0.724;  v[3].y =  0.447;  v[3].z = 0.526;
		v[4].x = -0.724;  v[4].y =  0.447;  v[4].z = -0.526;
		v[5].x = 0.276;   v[5].y =  0.447;  v[5].z = -0.851;
		v[6].x = 0.724;   v[6].y = -0.447;  v[6].z = 0.526;
		v[7].x = -0.276;  v[7].y = -0.447;  v[7].z = 0.851;
		v[8].x = -0.894;  v[8].y = -0.447;  v[8].z = 0.000;
		v[9].x = -0.276;  v[9].y = -0.447;  v[9].z = -0.851;
		v[10].x= 0.724;   v[10].y= -0.447;  v[10].z= -0.526;
		v[11].x= 0.000;   v[11].y= -1.000;  v[11].z= 0.000;    // Bottom-most point.

		//## PRINT VERTICES:
		// for(int i=0; i<12; i++)
		// {
		// fprintf(fout,"v %.5g %.5g %.5g\n", v[i].x*radius+pt.x, v[i].y*radius+pt.y, v[i].z*radius+pt.z);
		// }

		//## PRINT FACES:
		buf->data.push_back(v[12-1]); buf->data.push_back(v[10-1]); buf->data.push_back(v[11-1]);
		buf->data.push_back(v[12-1]); buf->data.push_back(v[9-1]); buf->data.push_back(v[10-1]);
		buf->data.push_back(v[12-1]); buf->data.push_back(v[8-1]); buf->data.push_back(v[9-1]);
		buf->data.push_back(v[12-1]); buf->data.push_back(v[7-1]); buf->data.push_back(v[8-1]);
		buf->data.push_back(v[12-1]); buf->data.push_back(v[11-1]); buf->data.push_back(v[7-1]);
		buf->data.push_back(v[1-1]); buf->data.push_back(v[6-1]); buf->data.push_back(v[5-1]);
		buf->data.push_back(v[1-1]); buf->data.push_back(v[5-1]); buf->data.push_back(v[4-1]);
		buf->data.push_back(v[1-1]); buf->data.push_back(v[4-1]); buf->data.push_back(v[3-1]);
		buf->data.push_back(v[1-1]); buf->data.push_back(v[3-1]); buf->data.push_back(v[2-1]);
		buf->data.push_back(v[1-1]); buf->data.push_back(v[2-1]); buf->data.push_back(v[6-1]);
		buf->data.push_back(v[11-1]); buf->data.push_back(v[10-1]); buf->data.push_back(v[6-1]);
		buf->data.push_back(v[10-1]); buf->data.push_back(v[9-1]); buf->data.push_back(v[5-1]);
		buf->data.push_back(v[9-1]); buf->data.push_back(v[8-1]); buf->data.push_back(v[4-1]);
		buf->data.push_back(v[8-1]); buf->data.push_back(v[7-1]); buf->data.push_back(v[3-1]);
		buf->data.push_back(v[7-1]); buf->data.push_back(v[11-1]); buf->data.push_back(v[2-1]);
		buf->data.push_back(v[6-1]); buf->data.push_back(v[10-1]); buf->data.push_back(v[5-1]);
		buf->data.push_back(v[5-1]); buf->data.push_back(v[9-1]); buf->data.push_back(v[4-1]);
		buf->data.push_back(v[4-1]); buf->data.push_back(v[8-1]); buf->data.push_back(v[3-1]);
		buf->data.push_back(v[3-1]); buf->data.push_back(v[7-1]); buf->data.push_back(v[2-1]);
		buf->data.push_back(v[2-1]); buf->data.push_back(v[11-1]); buf->data.push_back(v[6-1]);


		// buf->data.push_back(glm::vec3(2, 0, 2));
		// buf->data.push_back(glm::vec3(0, 0, 4));
		// buf->data.push_back(glm::vec3(-2, 0, 2));

		// buf->data.push_back(glm::vec3(2, 2, 0));
		// buf->data.push_back(glm::vec3(0, 4, 0));
		// buf->data.push_back(glm::vec3(-2, 2, 0));

		// buf->data.push_back(glm::vec3(2, 2, 0));
		// buf->data.push_back(glm::vec3(4, 0, 0));
		// buf->data.push_back(glm::vec3(2, -2, 0));

		buf->gl_update();

	}

	this->orientation = glm::mat4x4(1.0f);
	this->world_position = glm::vec3(0, 0, 0);

	this->motion_state = new btDefaultMotionState(btTransform(
		btQuaternion(0, 0, 0, 1),
		btVector3(0, 0, 0)
	));

	this->collision_shape = new btSphereShape(1);

	btRigidBody::btRigidBodyConstructionInfo info(
		100,
		this->motion_state,
		this->collision_shape,
		btVector3(0, 0, 0)
	);

	info.m_restitution = 0.5f;

	this->rigid_body = new btRigidBody(info);
	this->rigid_body->setUserPointer(this);

}


object_base::~object_base() {
	printf("object base destructor\n");
}


void object_base::step(float time_delta) {

}

void object_base::render(pipeline &pipe) const {
	glDisable(GL_CULL_FACE);

	this->material_shader->enable();

	glBindAttribLocation(shader::active->program, 0, "vertex");

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

	glm::mat4x4 model = translate * rotate * scale;

	gl_uniform("model", model);

	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, buf->data.size());
	glBindVertexArray(0);

	this->material_shader->disable();

	glEnable(GL_CULL_FACE);
	//glDisableTexture(GL_TEXTURE1);
//	glDisable(GL_CULL_FACE);

//	shd->enable();

/*	btTransform trans;
	this->motion_state->getWorldTransform(trans);

	glm::vec3 pos(
		trans.getOrigin().getX(),
		trans.getOrigin().getY(),
		trans.getOrigin().getZ()
	);
*/
	//glBindAttribLocation(shader::active->program, 0, "vertex");
/*
	if (pipe.camera)
		pipe.camera->gl_bind();

	glm::mat4x4 rotate = glm::mat4_cast(this->rotation);
	glm::mat4x4 scale(1.0f);
	glm::mat4x4 translate = glm::translate(glm::mat4x4(1.0f), pos);
	glm::mat4x4 model = translate * rotate * scale;

	gl_uniform("model", model);*/

/*
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
	glm::mat4x4 model = translate * rotate;

	gl_uniform("model", model);m

	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, buf->data.size());
	glBindVertexArray(0);

	shd->disable();

	glEnable(GL_CULL_FACE);
	*/

}


void object_base::update_collision_shape() {

}


void object_base::enter_world(ton::world *new_world) {
	assert(this->the_world == 0);
	this->the_world = new_world;
	this->the_world->dynamics_world->addRigidBody(this->rigid_body);
}

void object_base::leave_world() {
	assert(this->the_world != 0);
	this->the_world->dynamics_world->removeRigidBody(this->rigid_body);
	this->the_world = 0;
}



glm::vec3 object_base::get_world_position() const {
	btTransform trans;
	this->motion_state->getWorldTransform(trans);

	return glm::vec3(
		trans.getOrigin().getX(),
		trans.getOrigin().getY(),
		trans.getOrigin().getZ()
	);
}


glm::vec3 object_base::get_forward_direction() const {

	btVector3 dir(0, 0, 1);
	dir = this->rigid_body->getWorldTransform().getBasis() * dir;
	return glm::vec3(dir.getX(), dir.getY(), dir.getZ());

	// btVector3 dir(0, 0, 1);
	// btTransform trans;
	// this->motion_state->getWorldTransform(trans);
	// btQuaternion rot = trans.getRotation();
	// btVector3 fwd = trans * dir;
	// return glm::vec3(fwd.getX(), fwd.getY(), fwd.getZ());
}


glm::vec3 object_base::get_up_direction() const {
	btVector3 dir(0, 1, 0);
	dir = this->rigid_body->getWorldTransform().getBasis() * dir;
	return glm::vec3(dir.getX(), dir.getY(), dir.getZ());
}




glm::vec3 object_base::get_right_direction() const {
	btVector3 dir(1, 0, 0);
	dir = this->rigid_body->getWorldTransform().getBasis() * dir;
	return glm::vec3(dir.getX(), dir.getY(), dir.getZ());
}



void object_base::apply_local_force(const glm::vec3 &force, const glm::vec3 &pos) {
	btMatrix3x3& rotation = this->rigid_body->getWorldTransform().getBasis();
	btVector3 f = rotation * btVector3(force.x, force.y, force.z);
	this->rigid_body->activate();
	this->rigid_body->applyForce(
		f,
		rotation * btVector3(pos.x, pos.y, pos.z)
	);
	//std::cout << "OK: " << f.getX() << " " << f.getY() << " " << f.getZ() << std::endl;
}

void object_base::apply_local_impulse(const glm::vec3 &imp, const glm::vec3 &pos) {
	btMatrix3x3& rotation = this->rigid_body->getWorldTransform().getBasis();
	this->rigid_body->activate();
	this->rigid_body->applyImpulse(
		rotation * btVector3(imp.x, imp.y, imp.z),
		rotation * btVector3(pos.x, pos.y, pos.z)
	);
}
