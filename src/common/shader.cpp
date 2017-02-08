#include "shader.h"
#include "common.h"

#include <glm/gtc/type_ptr.hpp>
using glm::value_ptr;

Shader::Shader()
{
}

Shader::~Shader()
{
}

void Shader::generate(const char * vCode, const char * fCode)
{
	GLuint vertex, fragment;

	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vCode, nullptr);
	glCompileShader(vertex);
	checkErrors(vertex, Vertex);

	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fCode, nullptr);
	glCompileShader(fragment);
	checkErrors(fragment, Fragment);

	mId = glCreateProgram();
	glAttachShader(mId, vertex);
	glAttachShader(mId, fragment);
	glLinkProgram(mId);
	checkErrors(mId, Program);

	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

void Shader::checkErrors(GLuint id, TypeShader type) const
{
	static GLint success;
	static GLchar infoLog[512];
	if (type == Program)
	{
		glGetProgramiv(id, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(id, 512, nullptr, infoLog);
			fatals << "Failed to link the shaders to the program!\n"
				<< "Info log:\n" << infoLog << '\n';
			//exit(1);
		}
	}
	else
	{
		glGetShaderiv(id, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(id, 512, nullptr, infoLog);
			fatals << "Failed to compile shader!\n"
				<< "Shader type id: " << type << ", info log:\n" << infoLog << '\n';
			//exit(1);
		}
	}
}

void Shader::clear() const
{
	glDeleteProgram(mId);
}

GLuint Shader::getId() const
{
	return mId;
}

void Shader::use() const
{
	glUseProgram(mId);
}

#define Location glGetUniformLocation(mId, name)

void Shader::seti(const char * name, GLint v0) const
{
	glUniform1i(Location, v0);
}

void Shader::setf(const char * name, GLfloat v0) const
{
	glUniform1f(Location, v0);
}

void Shader::set2f(const char * name, GLfloat v0, GLfloat v1) const
{
	glUniform2f(Location, v0, v1);
}

void Shader::set2f(const char * name, const vec2 & v) const
{
	glUniform2fv(Location, 1, value_ptr(v));
}

void Shader::set3f(const char * name, GLfloat v0, GLfloat v1, GLfloat v2) const
{
	glUniform3f(Location, v0, v1, v2);
}

void Shader::set3f(const char * name, const vec3 & v) const
{
	glUniform3fv(Location, 1, value_ptr(v));
}

void Shader::set4f(const char * name, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3) const
{
	glUniform4f(Location, v0, v1, v2, v3);
}

void Shader::set4f(const char * name, const vec4 & v) const
{
	glUniform4fv(Location, 1, value_ptr(v));
}

void Shader::set4x4f(const char * name, const mat4 & m) const
{
	glUniformMatrix4fv(Location, 1, GL_FALSE, value_ptr(m));
}