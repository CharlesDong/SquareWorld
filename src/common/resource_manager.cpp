#include "resource_manager.h"
#include "common.h"
#include <fstream>
#include <sstream>

#include <SOIL.h>

std::map<std::string, Shader> ResourceManager::shaders;
std::map<std::string, Texture> ResourceManager::textures;

// Add a shader to the resource manager.
void ResourceManager::loadShader(const std::string & name, const char * vPath, const char * fPath)
{
	shaders[name] = loadShaderFromFile(vPath, fPath);
}

// Add a texture to the resource manager.
void ResourceManager::loadTexture(const std::string & name, const char * path)
{
	textures[name] = loadTextureFromFile(path);
}

// Load a shader from file.
Shader ResourceManager::loadShaderFromFile(const char * vPath, const char * fPath)
{
	static std::ifstream fin;
	static std::ostringstream ostr;
	static std::string vCode, fCode;
	static const char *vCodeFinal, *fCodeFinal;

	fin.open(vPath);
	if (!fin.is_open())
	{
		fatals << "Cannot open shader file, filename: " << vPath << " (maybe not exist)!\n";
		//exit(1);
	}
	ostr << fin.rdbuf();
	vCode = ostr.str();
	fin.close();
	ostr.str("");

	fin.open(fPath);
	if (!fin.is_open())
	{
		fatals << "Cannot open shader file, filename: " << fPath << " (maybe not exist)!\n";
		//exit(1);
	}
	ostr << fin.rdbuf();
	fCode = ostr.str();
	fin.close();
	ostr.str("");

	vCodeFinal = vCode.c_str();
	fCodeFinal = fCode.c_str();
	Shader shader;
	shader.generate(vCodeFinal, fCodeFinal);
	return shader;
}

// Load a texture from file.
Texture ResourceManager::loadTextureFromFile(const char * path)
{
	using std::string;

	int width, height;
	unsigned char * image;
	string p = path;
	bool alpha = false;

	if (p.find('.') != string::npos)		// if the file name has extension
		alpha = p.substr(p.find('.')) == ".png";

	image = SOIL_load_image(path, &width, &height, nullptr, alpha ? SOIL_LOAD_RGBA : SOIL_LOAD_RGB);
	if (image == nullptr)
		errors << "Failed to read texture file, filename: " << path << " (maybe not exist)!\n";

	Texture texture;
	texture.generate(image, width, height, alpha);
	SOIL_free_image_data(image);
	return texture;
}

// Gets a shader.
Shader & ResourceManager::getShader(const std::string & name)
{
	return shaders[name];
}

// Gets a texture.
Texture & ResourceManager::getTexture(const std::string & name)
{
	return textures[name];
}

// Clears all the resources.
void ResourceManager::clear()
{
	for (auto iter : shaders)
		iter.second.clear();
	for (auto iter : textures)
		iter.second.clear();
}