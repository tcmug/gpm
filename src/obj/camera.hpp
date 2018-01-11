#ifndef CORE_CAMERA_HPP
#define CORE_CAMERA_HPP

#include "common.hpp"

class pipeline;
class object;

void gl_uniform(const char *name, const glm::vec3 &p);
void gl_uniform(const char *name, const glm::mat4x4 &p);

/*
	Class: camera_base
		Base camera class
*/
class camera_base {

	protected:

		/*
			Variable: world_position
				The position of the camera in the world.
		*/
		glm::vec3 world_position;

	public:

		/*
			Function: set_position
				Set the position of the camera

			Parameters:
				&new_position - The new position
		*/
		void set_position(const glm::vec3 &new_position);

		/*
			Function: get_position
				Get position of the camera

			Returns:
				glm::vec3 The world position of the camera
		*/
		const glm::vec3 &get_position() const;

		virtual void activate();

		virtual void gl_bind();

		virtual glm::vec3 ray(const glm::vec3 &localray) const;

};


/*
	Class: camera
		A generic camera implementation.
*/
class generic_camera: public camera_base {

	protected:

		glm::vec3 up_vector, camera_target;
		glm::mat4 camera_matrix;
		glm::mat4 projection_matrix;

		float field_of_view, width, height, near, far;

	public:

		generic_camera(
			const glm::vec3 &from,
			const glm::vec3 &to,
			const glm::vec3 &up,
			float w, float h
		);


		void set_target(const glm::vec3 &new_target);

		virtual void activate();
		virtual glm::vec3 ray(const glm::vec3 &localray) const;


		void gl_bind();

};


/*
	Class: camera
		A generic camera implementation.
*/
class orto_camera: public generic_camera {
	public:

		orto_camera(
			const glm::vec3 &from,
			const glm::vec3 &to,
			const glm::vec3 &up,
			float w, float h
		);

		virtual void activate();
};


class effect_base {

};



#endif
