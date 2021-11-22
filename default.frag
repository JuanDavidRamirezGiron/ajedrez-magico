#version 330 core

// Outputs colors in RGBA
out vec4 FragColor;

in vec3 crntPos;
in vec3 Normal;
in vec2 texCoord;



uniform sampler2D diffuse0;
uniform vec4 lightColor;
uniform vec3 lightPos;
uniform vec3 camPos;


vec4 direcLight()
{
	// ambient lighting
	float ambient = 0.4f;

	// diffuse lighting
	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(vec3(0.9f, 1.0f, 0.8f));
	float diffuse = max(dot(normal, lightDirection), 0.0f);
	
	// specular lighting 
	float specularLight = 0.9f;
	vec3 viewDirection = normalize(camPos - crntPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
	float specular = specAmount * specularLight;

	return texture (diffuse0, texCoord) * (diffuse + ambient + specular) * lightColor;
}



void main()
{
	FragColor = direcLight();
}