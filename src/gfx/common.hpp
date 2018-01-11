#ifndef GFX_COMMON_HPP
#define GFX_COMMON_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

void _engine_gl_check(const char *file, const char *function, int line);

#define engine_gl_check() (_engine_gl_check(__FILE__, __FUNCTION__, __LINE__))

extern PFNGLGENERATEMIPMAPPROC glGenerateMipmap;
extern PFNGLGENFRAMEBUFFERSPROC glGenFramebuffers;
extern PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
extern PFNGLBINDFRAMEBUFFERPROC glBindFramebuffer;
extern PFNGLFRAMEBUFFERTEXTUREPROC glFramebufferTexture;
extern PFNGLCHECKFRAMEBUFFERSTATUSPROC glCheckFramebufferStatus;
extern PFNGLFRAMEBUFFERTEXTURE2DEXTPROC glFramebufferTexture2D;
extern PFNGLGENRENDERBUFFERSPROC glGenRenderbuffers;
extern PFNGLBINDRENDERBUFFERPROC glBindRenderbuffer;
extern PFNGLRENDERBUFFERSTORAGEPROC glRenderbufferStorage;
extern PFNGLFRAMEBUFFERRENDERBUFFERPROC glFramebufferRenderbuffer;
extern PFNGLBLITFRAMEBUFFERPROC glBlitFramebuffer;
extern PFNGLBINDVERTEXARRAYPROC glBindVertexArray;
extern PFNGLGENVERTEXARRAYSPROC glGenVertexArrays;
extern PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv;
extern PFNGLBINDBUFFERPROC glBindBuffer;
extern PFNGLBINDATTRIBLOCATIONPROC glBindAttribLocation;
extern PFNGLUNIFORM1IPROC glUniform1i;
extern PFNGLCREATEPROGRAMPROC glCreateProgram;
extern PFNGLDELETEPROGRAMPROC glDeleteProgram;
extern PFNGLATTACHSHADERPROC glAttachShader;
extern PFNGLDETACHSHADERPROC glDetachShader;
extern PFNGLLINKPROGRAMPROC glLinkProgram;
extern PFNGLGETPROGRAMIVPROC glGetProgramiv;
extern PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog;
extern PFNGLCREATESHADERPROC glCreateShader;
extern PFNGLCOMPILESHADERPROC glCompileShader;
extern PFNGLSHADERSOURCEPROC glShaderSource;
extern PFNGLGETSHADERIVPROC glGetShaderiv;
extern PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;
extern PFNGLDELETESHADERPROC glDeleteShader;
extern PFNGLUSEPROGRAMPROC glUseProgram;
extern PFNGLGENBUFFERSPROC glGenBuffers;
extern PFNGLDELETEBUFFERSPROC glDeleteBuffers;
extern PFNGLBUFFERDATAPROC glBufferData;
extern PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
extern PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;

#endif
