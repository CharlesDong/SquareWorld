#pragma once

#include <map>
#include <string>
#include "shader.h"
#include "texture.h"

class ResourceManager
{
private:
	static std::map<std::string, Shader> shaders;
	static std::map <std::string, Texture> textures;
	//static Texture textures[512];
	static Shader loadShaderFromFile(const char * vPath, const char * fPath);
	static Texture loadTextureFromFile(const char * path);
public:
	static void loadShader(const std::string & name, const char * vPath, const char * fPath);
	static void loadTexture(const std::string & name, const char * path);
	static Shader & getShader(const std::string & name);
	static Texture & getTexture(const std::string & name);
	static void clear();
};