#pragma once
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "Camera.h"

class Menu {
public:

	Menu(GLFWwindow* window);
	~Menu() {};
	void init(Camera& camera);
	void shutDown();
};