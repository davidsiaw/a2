#ifndef OPENGL_HPP
#define OPENGL_HPP

#if defined(__IPHONEOS__) || defined(__ANDROID__)
#define HAVE_OPENGLES
#endif

// For Ubuntu
#define GL_GLEXT_PROTOTYPES

// Switch for OpenGL ES
#ifdef HAVE_OPENGLES
#include <SDL_opengles2.h>
#else
#include <GL/glew.h>
#include <SDL_opengl.h>
#endif

// Open GL Math tools
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define OFFSET_OF(type, field) ((void*)offsetof(type, field))
#define STRINGIFY(str) #str

#define PRINT_GLERROR { auto glerr = glGetError(); if (glerr) { printf("%d 0x%x %s:%d\n", glerr, glerr, __FILE__, __LINE__); } }

#endif // OPENGL_HPP
