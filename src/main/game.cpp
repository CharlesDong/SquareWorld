#include <GL/glew.h>

#include "game.h"

#include "common/resource_manager.h"
#include "renderer/renderer.h"
#include "renderer/text_renderer.h"

#include <string>
#include <sstream>

void keyCallback(GLFWwindow *, int, int, int, int);
void cursorPosCallback(GLFWwindow *, double, double);
void mouseButtonCallback(GLFWwindow *, int, int, int);

Game Game::app;

const char * const Version = "0.4.1";

Game::Game()
{
}

Game::~Game()
{
	ResourceManager::clear();
	Renderer::clear();

	glfwTerminate();
}

void Game::run()
{
	init();
	while (!glfwWindowShouldClose(mWindow))
		runGameLoop();
}

void Game::close() const
{
	glfwSetWindowShouldClose(mWindow, GL_TRUE);
}

void Game::init()
{
	// init GLFW
	if (!glfwInit())
		fatals << "Failed to init GLFW!\n";

	// window settings
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	mWidth = 800;
	mHeight = 600;
	std::string title = "Minecraft 2D ";
	title += Version;

	// create window
	mWindow = glfwCreateWindow(mWidth, mHeight, title.c_str(), nullptr, nullptr);
	if (!mWindow)
		fatals << "Failed to create window using GLFW!\n";
	glfwMakeContextCurrent(mWindow);

	// set callbacks
	glfwSetKeyCallback(mWindow, keyCallback);
	glfwSetCursorPosCallback(mWindow, cursorPosCallback);
	glfwSetMouseButtonCallback(mWindow, mouseButtonCallback);

	// init GLEW
	glewExperimental = GL_TRUE;
	if (glewInit())
		fatals << "Failed to init GLEW!\n";

	// OpenGL settings
	glViewport(0, 0, mWidth, mHeight);

	glClearColor(0.0f, 0.75f, 1.0f, 1.0f);		// set the clear color to a light blue color
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// load resources
	loadShaders();
	loadTextures();

	// init game objects
	Renderer::init();
	TextRenderer::init();
	mSelectFrame.init();
	mMenu.init(&mWorld);
}

void Game::loadShader(const std::string & name, const char * vpath, const char * fpath) const
{
	ResourceManager::loadShader(name, vpath, fpath);
}

void Game::loadShaders() const
{
	loadShader("shader", "res/shaders/shader.vert", "res/shaders/shader.frag");
	loadShader("cursor", "res/shaders/cursor.vert", "res/shaders/cursor.frag");
	loadShader("text", "res/shaders/text.vert", "res/shaders/text.frag");
}

void Game::loadTexture(const std::string & name, const char * path) const
{
	ResourceManager::loadTexture(name, path);
}

void Game::loadTextures() const
{
	// block texture
	loadTexture("dirt", "res/textures/blocks/dirt.bmp");
	loadTexture("grass", "res/textures/blocks/grass.bmp");
	loadTexture("stone", "res/textures/blocks/stone.bmp");
	loadTexture("bedrock", "res/textures/blocks/bedrock.bmp");
	
	// player texture
	loadTexture("headL", "res/textures/skins/steve/head_l.bmp");
	loadTexture("headR", "res/textures/skins/steve/head_r.bmp");
	loadTexture("handL", "res/textures/skins/steve/hand_l.bmp");
	loadTexture("handR", "res/textures/skins/steve/hand_r.bmp");
	loadTexture("bodyL", "res/textures/skins/steve/body_l.bmp");
	loadTexture("bodyR", "res/textures/skins/steve/body_r.bmp");
	loadTexture("legL", "res/textures/skins/steve/leg_l.bmp");
	loadTexture("legR", "res/textures/skins/steve/leg_r.bmp");

	// GUI
	loadTexture("button", "res/textures/gui/button.bmp");
	loadTexture("button1", "res/textures/gui/button_selected.bmp");
	loadTexture("button2", "res/textures/gui/button_unavaliable.bmp");

	// cracks on select frame
	std::ostringstream os1, os2;
	std::string path, name;
	for (int i = 0; i <= 9; i++)
	{
		os1 << "res/textures/blocks/destroy" << i << ".png";
		os2 << "destroy" << i;
		path = os1.str();
		name = os2.str();
		loadTexture(name, path.c_str());
		os1.str("");
		os2.str("");
	}
}

void Game::runGameLoop()
{
	glfwPollEvents();

	update();

	render();

	glfwSwapBuffers(mWindow);
}

void Game::update()
{
	if (isKeyDownNotProc(GLFW_KEY_ESCAPE))
	{
		setKeyProc(GLFW_KEY_ESCAPE);
		if (mGameStarted)
			changePause();
		else
			close();
	}
	if (mGameStarted)
	{
		if (isKeyDownNotProc(GLFW_KEY_F3))
		{
			setKeyProc(GLFW_KEY_F3);
			mDisplayDebugInfo = !mDisplayDebugInfo;
		}
		updateFps();
		if (!mGamePaused)
		{
			mPlayer.update(mDeltaTime, mWorld);
			mSelectFrame.update(mDeltaTime, mPlayer.getSceneOffset(), mWorld);
		}
	}
	if (!mGameStarted || mGamePaused)
		mWidgets.update();
}

// Updates the game FPS.
void Game::updateFps()
{
	static int timeInt = 0;
	static double curr = 0.0, prev = 0.0;
	static int frameCount = 0;

	// update frame counter, timer and FPS
	frameCount++;

	prev = curr;
	curr = glfwGetTime();
	mDeltaTime = curr - prev;

	if (int(curr) - timeInt >= 1)
	{
		timeInt = int(curr);
		mFps = frameCount;
		frameCount = 0;
	}
}

void Game::render() const
{
	glClear(GL_COLOR_BUFFER_BIT);
	
	Renderer::setScreenCoord(false);
	if (mGameStarted)
	{
		mWorld.render(mPlayer.getSceneOffset());
		mPlayer.render();
		mSelectFrame.render();
	}
	Renderer::setScreenCoord(true);
	if (!mGameStarted || mGamePaused)
		mWidgets.render();

	if (mDisplayDebugInfo && mGameStarted)
		renderDebugInfo();
}

void Game::renderDebugInfo() const
{
	const int Height = TextRenderer::maxHeight() + 5;
	const int Top = 600;

	TextRenderer::text(0, Top - Height, "FlatWorld %s", Version);
	TextRenderer::text(0, Top - Height * 2, "FPS: %d", mFps);
	TextRenderer::text(0, Top - Height * 3, "x: %lf", mPlayer.getPosition().x);
	TextRenderer::text(0, Top - Height * 4, "y: %lf", mPlayer.getPosition().y);
	TextRenderer::text(0, Top - Height * 5, "Jumping: %b", mPlayer.isJumping());
}

void Game::changePause()
{
	if (mGamePaused)
		mMenu.change(MENU_NULL);
	else
		mMenu.change(MENU_PAUSE);
	mGamePaused = !mGamePaused;
}

int Game::getWinWidth() const
{
	return mWidth;
}

int Game::getWinHeight() const
{
	return mHeight;
}

void Game::setKey(int key, bool state)
{
	mInput.keys[key] = state;
}

// Returns true if the key is down, false otherwise.
bool Game::isKeyDown(int key) const
{
	return mInput.keys[key];
}

// Returns true if the key is down AND not processed, false otherwise.
bool Game::isKeyDownNotProc(int key) const
{
	return mInput.keys[key] && !mInput.keysProc[key];
}

void Game::setKeyProc(int key, bool state)
{
	mInput.keysProc[key] = state;
}

// Returns true if the key is processed, false otherwise.
bool Game::isKeyProc(int key) const
{
	return mInput.keysProc[key];
}

const ivec2 & Game::getCursorPos() const
{
	return mInput.cursor;
}

void Game::setCursorPos(int x, int y)
{
	mInput.cursor.x = x;
	mInput.cursor.y = y;
}

// Returns true if the mouse button is down, false otherwise.
bool Game::isMouseDown(int button) const
{
	return mInput.buttons[button];
}

void Game::setMouse(int button, bool state)
{
	mInput.buttons[button] = state;
}

WidgetManager & Game::getWidgets()
{
	return mWidgets;
}

Menu & Game::getMenu()
{
	return mMenu;
}

int Game::menuType() const
{
	return mMenu.getType();
}

bool Game::isGameStarted() const
{
	return mGameStarted;
}

bool Game::isGamePaused() const
{
	return mGamePaused;
}

void Game::setGameStarted(bool state)
{
	mGameStarted = state;
}

void Game::setGamePaused(bool state)
{
	mGamePaused = state;
}

Game & Game::getApp()
{
	return app;
}

// callbacks

void keyCallback(GLFWwindow * window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_UNKNOWN)		// unknown key
		return;
	if (action == GLFW_PRESS)
		App.setKey(key, true);
	else if (action == GLFW_RELEASE)
	{
		App.setKey(key, false);
		App.setKeyProc(key, false);
	}
}

void cursorPosCallback(GLFWwindow * window, double xpos, double ypos)
{
	App.setCursorPos((int)xpos, App.getWinHeight() - (int)ypos);
}

void mouseButtonCallback(GLFWwindow * window, int button, int action, int mods)
{
	if (button > GLFW_MOUSE_BUTTON_RIGHT)	// other buttons
		return;
	if (action == GLFW_PRESS)
		App.setMouse(button, true);
	else if (action == GLFW_RELEASE)
		App.setMouse(button, false);
}