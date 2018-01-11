#ifndef CORE_PIPELINE_HPP
#define CORE_PIPELINE_HPP

#include <vector>

class pipeline;
class camera_base;
class object_base;
class particle_array_base;

#include "../obj/object_base.hpp"
#include "../obj/camera.hpp"

#include "fbo.hpp"

template <class T>
class some {
public:
	T var;
	some(T c): var(c) {}
};

template <class T>
class pipeline_manip {

	public:

		T func_param;

		pipeline_manip(const T i):
			func_param(i)
		{}

};

class pipeline {

	public:

		fbo *buffer;
		camera_base *camera;

		int offset;


		/*
			Function: constructor
				Constructor
		*/
		pipeline(): buffer(NULL), camera(NULL), offset(0) {}

		inline pipeline & operator << (pipeline &pipe) {
			return *this;
		}


		inline pipeline & operator << (fbo *active_buffer) {
			this->buffer = active_buffer;
			this->buffer->render_start();
			return *this;
		}

		inline pipeline & operator << (fbo &active_buffer) {
			this->buffer = &active_buffer;
			this->buffer->render_start();
			return *this;
		}

		inline pipeline & operator << (camera_base *active_camera) {
			this->camera = active_camera;
			this->camera->activate();
			return *this;
		}

		inline pipeline & operator << (camera_base &active_camera) {
			this->camera = &active_camera;
			this->camera->activate();
			return *this;
		}

		inline pipeline & operator << (const std::vector <object_base*> &objects) {
			for (const object_base *obj : objects) {
				obj->render(*this);
			}
			return *this;
		}

		// inline pipeline & operator << (const particle_array_base &particles) {
		// 	particles.render(*this);
		// 	return *this;
		// }

		// inline pipeline & operator << (const std::vector <particle_array_base*> &particles) {
		// 	for (const particle_array_base *part : particles) {
		// 		part->render(*this);
		// 	}
		// 	return *this;
		// }

		// inline pipeline & operator << (const particle_array_base *particles) {
		// 	particles->render(*this);
		// 	return *this;
		// }


		void flush() {
			if (this->buffer) {
				this->buffer->blit_smooth(NULL, 0, this->offset, this->buffer->width, this->buffer->height);
				this->buffer->render_end();
				this->buffer = NULL;
			}
			this->camera = NULL;
		}


		inline pipeline &operator << (pipeline &(*func)(pipeline &)) {
			return func(*this);
		}

		template <class T>
		pipeline &operator << (pipeline_manip <T> &pm) {
			//(this->*pm.func_pointer)(pm.func_param);
			return *this;
		}


		inline pipeline &operator << (const some <char> &blah) {
			//(this->*pm.func_pointer)(pm.func_param);
			//this->ae(3);
			return *this;
		}


};



inline pipeline & flush(pipeline &pipe) {
	pipe.flush();
	return pipe;
}

inline pipeline & clear(pipeline &pipe) {
	glClearColor(0.2, 0, 0, 0);
	glClearDepth(2000);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	return pipe;
}


inline some <char> testaa(char t) {
	return some <char> (t);
}


#endif
