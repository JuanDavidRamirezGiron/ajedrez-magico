#pragma once
#include<glad/glad.h>
#include "shaderClass.h"
#include <vector>
#include "texture.h"
#include "VAO.h"
#include<glm/gtc/matrix_transform.hpp>

class Skybox 
{
public:
	Skybox(Shader skyboxShader);
	void draw();
private:
	Shader skyboxShader;
	unsigned int cubemapTexture;
	GLint skyboxvaoID;
};