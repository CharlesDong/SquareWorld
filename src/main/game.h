#pragma once

#define GLFW_INCLUDE_NONE		// prevent GLFW from including <GL/gl.h>
#include <GLFW/glfw3.h>

#include "common/common.h"
#include "world/world.h"
#include "player/player.h"
#include "selectframe/select_frame.h"
#include "gui/widget_manager.h"
#include "gui/menu.h"

#include <string>

class Game
{
private:
	GLFWwindow * mWindow = nullptr;

	int mFps = 0;
	double mDeltaTime = 0.0;
	bool mDisplayDebugInfo = false;
	bool mGameStarted = false;
	bool mGamePaused = false;
	bool keysProc[GLFW_KEY_LAST + 1];

	World mWorld;
	Player mPlayer;
	SelectFrame mSelectFrame;
	WidgetManager mWidgets;
	Menu mMenu;

	static Game app;

	void init();
	void loadShaders() const;
	void loadTextures() const;
	void runGameLoop();
	void update();
	void updateFps();
	void render() const;
	void renderDebugInfo() const;
	void changePause();

	void loadShader(const std::string & name, const char * vpath, const char * fpath) const;
	void loadTexture(const std::string & name, const char * path) const;
public:
	Game();
	~Game();

	void run();
	void close() const;

	// input

	int getWidth() const;
	int getHeight() const;
	ivec2 getCursorPos() const;
	bool isKeyDown(int key) const;
	bool isKeyProc(int key) const;
	bool isKeyDownNotProc(int key) const;
	bool isMouseDown(int button) const;

	void setKeyProc(int key, bool state = true);

	// game

	WidgetManager & getWidgets();
	Menu & getMenu();
	int menuType() const;
	bool isGameStarted() const;
	bool isGamePaused() const;

	void setGameStarted(bool state);
	void setGamePaused(bool state);

	static Game & getApp();
};

#define App Game::getApp()