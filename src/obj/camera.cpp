
#include "camera.hpp"

#include <iostream>

void camera_base::set_position(const glm::vec3 &new_position) {
	this->world_position = new_position;
}


const glm::vec3 &camera_base::get_position() const {
	return this->world_position;
}


void camera_base::activate() {
}


void camera_base::gl_bind() {
}

glm::vec3 camera_base::ray(const glm::vec3 &localray) const {
	return glm::vec3(0, 0, -1);
}


generic_camera::generic_camera(
	const glm::vec3 &from,
	const glm::vec3 &to,
	const glm::vec3 &up,
	float w, float h) {

	this->world_position = from;
	this->camera_target = to;
	this->up_vector = up;
	this->field_of_view = 1.21;
	this->width = w;
	this->height = h;
	this->near = 0.01;
	this->far = 500;

}


void generic_camera::set_target(const glm::vec3 &new_target) {
	this->camera_target = new_target;
}


glm::vec3 generic_camera::ray(const glm::vec3 &_localray) const {

	glm::vec4 lRayStart_NDC(_localray.x, _localray.y, -1, 1);
	glm::vec4 lRayEnd_NDC(_localray.x, _localray.y, 0, 1);

	glm::mat4 M = glm::inverse(this->projection_matrix * this->camera_matrix);

	glm::vec4 lRayStart_world = M * lRayStart_NDC;
	lRayStart_world /= lRayStart_world.w;
	glm::vec4 lRayEnd_world   = M * lRayEnd_NDC;
	lRayEnd_world /= lRayEnd_world.w;

	glm::vec3 lRayDir_world(lRayEnd_world - lRayStart_world);

	//glm::vec3 no(temp.x, temp.y, temp.z);

	return glm::normalize(lRayDir_world);

}


void generic_camera::activate() {

	this->projection_matrix = glm::perspective(
		this->field_of_view,
		this->width / this->height,
		this->near,
		this->far
	);

	this->camera_matrix = glm::lookAt(
		this->world_position,
		this->camera_target,
		this->up_vector
	);

}



orto_camera::orto_camera(
	const glm::vec3 &from,
	const glm::vec3 &to,
	const glm::vec3 &up,
	float w, float h): generic_camera(from, to, up, w, h) {

	this->near = -1000;
	this->far = 1000;

}




void orto_camera::activate() {

	// this->projection_matrix = glm::perspective(
	// 	this->field_of_view,
	// 	this->width / this->height,
	// 	this->near,
	// 	this->far
	// );

	// (T const &left, T const &right, T const &bottom, T const &top)
	//  (T const &left, T const &right, T const &bottom, T const &top, T const &zNear, T const &zFar)

	this->projection_matrix = glm::ortho(
		0.0f,
		this->width,
		0.0f,
		this->height,
		this->near,
		this->far
	);

	//this->projection_matrix = this->projection_matrix * t;

	this->camera_matrix = glm::translate(
		glm::mat4x4(1.0f),
		glm::vec3(
			this->width / 2,
			this->height / 2,
			0
		)
	) * glm::lookAt(
		this->world_position,
		this->camera_target,
		this->up_vector
	) * glm::scale(
		glm::mat4(1.0f),
		glm::vec3(10.0f)
	);

}


void generic_camera::gl_bind() {

	gl_uniform("projection", this->projection_matrix);
	gl_uniform("view", this->camera_matrix);

}

