#pragma once

#include <GL/glew.h>
#include "common.h"

using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::mat4;

class Shader
{
private:
	GLuint mId;

	enum TypeShader { Vertex, Fragment, Program };
	void checkErrors(GLuint id, TypeShader type) const;
public:
	Shader();
	~Shader();

	void generate(const char * vCode, const char * fCode);
	void use() const;
	GLuint getId() const;

	// uniform-setting functions
	void seti(const char * name, GLint v0) const;
	void setf(const char * name, GLfloat v0) const;
	void set2f(const char * name, GLfloat v0, GLfloat v1) const;
	void set2f(const char * name, const vec2 & v) const;
	void set3f(const char * name, GLfloat v0, GLfloat v1, GLfloat v2) const;
	void set3f(const char * name, const vec3 & v) const;
	void set4f(const char * name, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3) const;
	void set4f(const char * name, const vec4 & v) const;
	void set4x4f(const char * name, const mat4 & m) const;

	void clear() const;
};