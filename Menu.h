#pragma once
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "Camera.h"
#include "FENFileReader.h"


class Menu {
public:

	Menu(GLFWwindow* window);
	~Menu() {};
	void init(FENFileReader* &fenFileReader, Camera& camera, int globalPlay, int numPlays, bool autoPlay);
	void shutDown();
	bool getBoardChange(){ return boardChange; }
	void setBoardChange(bool change) { boardChange = change;}

private:
	bool boardChange;
};