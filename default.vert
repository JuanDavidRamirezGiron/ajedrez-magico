#version 330 core




layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aColor;
layout (location = 3) in vec2 aTex;



// Outputs the current position for the Fragment Shader
out vec3 crntPos;
// Outputs the normal for the Fragment Shader
out vec3 Normal;
// Outputs the color for the Fragment Shader
out vec3 color;
// Outputs the texture coordinates to the Fragment Shader
out vec2 texCoord;

out vec4 FragPosLightSpace;


// Imports the camera matrix
uniform mat4 camMatrix;
// Imports the transformation matrices
uniform mat4 model;
uniform mat4 translation;
uniform mat4 rotation;
uniform mat4 scale;
uniform mat4 lightSpaceMatrix;

void main()
{
	Normal = mat3(transpose(inverse(model))) *aNormal; //calculamos la normal tras las transformaciones
	crntPos = vec3(model * vec4(aPos, 1.0f));
	color = aColor;
	texCoord = aTex;
	
	FragPosLightSpace = lightSpaceMatrix * vec4(crntPos, 1.0); //matriz luz de un punto
	gl_Position = camMatrix * vec4(crntPos, 1.0);
}