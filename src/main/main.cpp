/*
Game name: FlatWorld
Language: C++
Compiler: Visual Studio 2015

Additional header files:
<GLFW/glfw3.h>
<GL/glew.h>
<SOIL.h>
<freetype.h>
GLM header files

Additional LIBs:
opengl32.lib
glfw3dll.lib
glew32.lib
SOIL.lib
freetype.lib

Required DLLs:
glfw3.dll
glew32.dll
*/

#include "game.h"

int main()
{
	App.run();
	return 0;
}