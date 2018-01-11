
#include <sys/time.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#include "common.hpp"
#include "../lua/common.hpp"

#define OK (0)
#define ERROR_NO_OPENGL (1)
#define ERROR_NO_FONT (2)
#define ERROR_NO_EXTENSION (3)


enum {
	KS_RESTART = 400
};

PFNGLGENERATEMIPMAPPROC glGenerateMipmap;
PFNGLGENFRAMEBUFFERSPROC glGenFramebuffers;
PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
PFNGLBINDFRAMEBUFFERPROC glBindFramebuffer;
PFNGLFRAMEBUFFERTEXTUREPROC glFramebufferTexture;
PFNGLCHECKFRAMEBUFFERSTATUSPROC glCheckFramebufferStatus;
PFNGLFRAMEBUFFERTEXTURE2DEXTPROC glFramebufferTexture2D;
PFNGLGENRENDERBUFFERSPROC glGenRenderbuffers;
PFNGLBINDRENDERBUFFERPROC glBindRenderbuffer;
PFNGLRENDERBUFFERSTORAGEPROC glRenderbufferStorage;
PFNGLFRAMEBUFFERRENDERBUFFERPROC glFramebufferRenderbuffer;
PFNGLBLITFRAMEBUFFERPROC glBlitFramebuffer;
PFNGLBINDVERTEXARRAYPROC glBindVertexArray;
PFNGLGENVERTEXARRAYSPROC glGenVertexArrays;
PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv;
PFNGLBINDBUFFERPROC glBindBuffer;
PFNGLBINDATTRIBLOCATIONPROC glBindAttribLocation;
PFNGLUNIFORM1IPROC glUniform1i;
PFNGLCREATEPROGRAMPROC glCreateProgram;
PFNGLDELETEPROGRAMPROC glDeleteProgram;
PFNGLATTACHSHADERPROC glAttachShader;
PFNGLDETACHSHADERPROC glDetachShader;
PFNGLLINKPROGRAMPROC glLinkProgram;
PFNGLGETPROGRAMIVPROC glGetProgramiv;
PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog;
PFNGLCREATESHADERPROC glCreateShader;
PFNGLCOMPILESHADERPROC glCompileShader;
PFNGLSHADERSOURCEPROC glShaderSource;
PFNGLGETSHADERIVPROC glGetShaderiv;
PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;
PFNGLDELETESHADERPROC glDeleteShader;
PFNGLUSEPROGRAMPROC glUseProgram;
PFNGLGENBUFFERSPROC glGenBuffers;
PFNGLDELETEBUFFERSPROC glDeleteBuffers;
PFNGLBUFFERDATAPROC glBufferData;
PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;

SDL_Window *video_window = 0;
SDL_GLContext gl_context;
int fps_limit = 0;
unsigned char keystate[512] = {0};
TTF_Font *font = 0;


#define GL_GET(var,type,name)\
	var = reinterpret_cast<type>(SDL_GL_GetProcAddress(name));\
	if (var) {\
		printf(" + %s\n", name);\
	}\
	else {\
		printf(" - %s\n", name);\
		return ERROR_NO_EXTENSION;\
	}

int engine_init() {


	// Start by loading preferences to the Lua stack.
	lua.dofile("preferences.lua");
	lua_State *L = lua.get_state();

	lua_getglobal(L, "preferences");
	if (lua_isnil(L, -1)) {
		printf("No preferences - using nasty defaults.\n");
		// Push an empty table to the stack to simulate having preferences.
		lua_newtable(L);
	}

	// Continue with initialization by querying the preferences table
	// from the Lua stack.

	int video_bits = 32;

	int video_width = lua.get_table_int("video_width", 800);
	int video_height = lua.get_table_int("video_height", 600);
	int video_flags = lua.get_table_int("video_fullscreen", 0) ? SDL_WINDOW_FULLSCREEN : 0;

	int audio_format = AUDIO_S16SYS;
	int audio_rate = lua.get_table_int("audio_rate", 44100);
	int audio_channels = lua.get_table_int("audio_channels", 2);
	int audio_buffers = lua.get_table_int("audio_buffers", 4096);

	struct timeval t1;

	TTF_Init();

	Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers);

	SDL_Init(SDL_INIT_VIDEO);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	video_flags |= SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;

	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 0);

	video_window = SDL_CreateWindow(
		"ton",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		video_width, video_height, video_flags
	);

	gl_context = SDL_GL_CreateContext(video_window);

	printf("OPENGL %s / GLSL %s\n", glGetString(GL_VERSION), glGetString(GL_SHADING_LANGUAGE_VERSION));

	font = TTF_OpenFont("monaco.ttf", 10);
	if (!font) {
		return ERROR_NO_FONT;
	}

	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#elif _MSC_VER
#elif __BORLANDC__
#elif __MINGW32__
#endif

	GL_GET(glGenerateMipmap, PFNGLGENERATEMIPMAPPROC, "glGenerateMipmap");
	GL_GET(glGenFramebuffers, PFNGLGENFRAMEBUFFERSPROC, "glGenFramebuffers");
	GL_GET(glGetUniformLocation, PFNGLGETUNIFORMLOCATIONPROC, "glGetUniformLocation");
	GL_GET(glBindFramebuffer, PFNGLBINDFRAMEBUFFERPROC, "glBindFramebuffer");
	GL_GET(glFramebufferTexture, PFNGLFRAMEBUFFERTEXTUREPROC, "glFramebufferTexture");
	GL_GET(glCheckFramebufferStatus, PFNGLCHECKFRAMEBUFFERSTATUSPROC, "glCheckFramebufferStatus");
	GL_GET(glFramebufferTexture2D, PFNGLFRAMEBUFFERTEXTURE2DEXTPROC, "glFramebufferTexture2D");
	GL_GET(glGenRenderbuffers, PFNGLGENRENDERBUFFERSPROC, "glGenRenderbuffers");
	GL_GET(glBindRenderbuffer, PFNGLBINDRENDERBUFFERPROC, "glBindRenderbuffer");
	GL_GET(glRenderbufferStorage, PFNGLRENDERBUFFERSTORAGEPROC, "glRenderbufferStorage");
	GL_GET(glFramebufferRenderbuffer, PFNGLFRAMEBUFFERRENDERBUFFERPROC, "glFramebufferRenderbuffer");
	GL_GET(glBlitFramebuffer, PFNGLBLITFRAMEBUFFERPROC, "glBlitFramebuffer");
	GL_GET(glBindVertexArray, PFNGLBINDVERTEXARRAYPROC, "glBindVertexArray");
	GL_GET(glGenVertexArrays, PFNGLGENVERTEXARRAYSPROC, "glGenVertexArrays");
	GL_GET(glUniformMatrix4fv, PFNGLUNIFORMMATRIX4FVPROC, "glUniformMatrix4fv");
	GL_GET(glBindBuffer, PFNGLBINDBUFFERPROC, "glBindBuffer");
	GL_GET(glBindAttribLocation, PFNGLBINDATTRIBLOCATIONPROC, "glBindAttribLocation");
	GL_GET(glUniform1i, PFNGLUNIFORM1IPROC, "glUniform1i");
	GL_GET(glCreateProgram, PFNGLCREATEPROGRAMPROC, "glCreateProgram");
	GL_GET(glDeleteProgram, PFNGLDELETEPROGRAMPROC, "glDeleteProgram");
	GL_GET(glAttachShader, PFNGLATTACHSHADERPROC, "glAttachShader");
	GL_GET(glDetachShader, PFNGLATTACHSHADERPROC, "glDetachShader");
	GL_GET(glLinkProgram, PFNGLLINKPROGRAMPROC, "glLinkProgram");
	GL_GET(glGetProgramiv, PFNGLGETPROGRAMIVPROC, "glGetProgramiv");
	GL_GET(glGetProgramInfoLog, PFNGLGETPROGRAMINFOLOGPROC, "glGetProgramInfoLog");
	GL_GET(glCreateShader, PFNGLCREATESHADERPROC, "glCreateShader");
	GL_GET(glCompileShader, PFNGLCOMPILESHADERPROC, "glCompileShader");
	GL_GET(glShaderSource, PFNGLSHADERSOURCEPROC, "glShaderSource");
	GL_GET(glGetShaderiv, PFNGLGETSHADERIVPROC, "glGetShaderiv");
	GL_GET(glGetShaderInfoLog, PFNGLGETSHADERINFOLOGPROC, "glGetShaderInfoLog");
	GL_GET(glDeleteShader, PFNGLDELETESHADERPROC, "glDeleteShader");
	GL_GET(glUseProgram, PFNGLUSEPROGRAMPROC, "glUseProgram");
	GL_GET(glGenBuffers, PFNGLGENBUFFERSPROC, "glGenBuffers");
	GL_GET(glDeleteBuffers, PFNGLDELETEBUFFERSPROC, "glDeleteBuffers");
	GL_GET(glBufferData, PFNGLBUFFERDATAPROC, "glBufferData");
	GL_GET(glVertexAttribPointer, PFNGLVERTEXATTRIBPOINTERPROC, "glVertexAttribPointer");
	GL_GET(glEnableVertexAttribArray, PFNGLENABLEVERTEXATTRIBARRAYPROC, "glEnableVertexAttribArray");
	GL_GET(glVertexAttribPointer, PFNGLVERTEXATTRIBPOINTERPROC, "glVertexAttribPointer");
	GL_GET(glEnableVertexAttribArray, PFNGLENABLEVERTEXATTRIBARRAYPROC, "glEnableVertexAttribArray");
	GL_GET(glVertexAttribPointer, PFNGLVERTEXATTRIBPOINTERPROC, "glVertexAttribPointer");

#ifdef __GNUC__
#pragma GCC diagnostic pop
#elif _MSC_VER
#elif __BORLANDC__
#elif __MINGW32__
#endif

	gettimeofday(&t1, NULL);
	srand(t1.tv_usec * t1.tv_sec);

	video_bits = 32;

	fps_limit = lua.get_table_int("fps_limit", 0);

	int vsync = lua.get_table_int("video_vsync", 1);

	memset(keystate, 0, sizeof(keystate));

	SDL_GL_SetSwapInterval(vsync);

    engine_gl_check();

    // Pop the configuration from the Lua stack.
    lua_pop(L, -1);

	return OK;
}


int engine_handle_input() {

	int running = 1;
	SDL_Event sdl_event;

	while (SDL_PollEvent(&sdl_event)) {

		if (sdl_event.key.repeat) {
			continue;
		}

		switch (sdl_event.type) {

			case SDL_QUIT:
				running = 0;
			break;

			case SDL_MOUSEBUTTONUP:
				keystate[6] = 0;
			break;

			case SDL_MOUSEBUTTONDOWN:
				keystate[6] = 1;
			break;

			case SDL_MOUSEMOTION: {
				// mouse_position.x = sdl_event.button.x;
				// mouse_position.y = sdl_event.button.y;
				// mouse_position.z = 1;

				// int window_width, window_height;
				// SDL_GetWindowSize(video_window, &window_width, &window_height);

				// float hw = window_width / 2;
				// float hh = window_height / 2;

				// mouse_position.x -= hw;
				// mouse_position.y -= hh;

				// mouse_position.x /= hw;
				// mouse_position.y /= -hh;

				// //mouse_position.y = -mouse_position.y;
				// mouse_position.z = -1;

				}
			break;

			case SDL_KEYDOWN:
				switch (sdl_event.key.keysym.sym) {
					case SDLK_ESCAPE:
						running = 0;
					break;
					case SDLK_F1:
						keystate[KS_RESTART] = 1;
					break;
					case SDLK_d:
						keystate[0] = 1;
					break;
					case SDLK_a:
						keystate[1] = 1;
					break;
					case SDLK_w:
						keystate[2] = 1;
					break;
					case SDLK_s:
						keystate[3] = 1;
					break;
					case SDLK_r:
						keystate[4] = 1;
					break;
					case SDLK_f:
						keystate[5] = 1;
					break;
				}
			break;

			case SDL_KEYUP:
				switch (sdl_event.key.keysym.sym) {
					case SDLK_F1:
						keystate[KS_RESTART] = 0;
					break;
					case SDLK_d:
						keystate[0] = 0;
					break;
					case SDLK_a:
						keystate[1] = 0;
					break;
					case SDLK_w:
						keystate[2] = 0;
					break;
					case SDLK_s:
						keystate[3] = 0;
					break;
					case SDLK_r:
						keystate[4] = 0;
					break;
					case SDLK_f:
						keystate[5] = 0;
					break;
				}
			break;

		}
	}

	return running;

}

void _engine_gl_check(const char *file, const char *function, int line) {
	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR) {
		printf("%s: %u => %s\n", file, line, function);
		exit(1);
	}
}





