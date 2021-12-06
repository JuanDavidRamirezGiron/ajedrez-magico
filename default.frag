#version 330 core

// Outputs colors in RGBA
out vec4 FragColor;

in vec3 crntPos;
in vec3 Normal;
in vec2 texCoord;
in vec4 FragPosLightSpace;

uniform sampler2D shadowMap;
uniform sampler2D diffuse0;

uniform vec4 lightColor;
uniform vec3 lightPos;
uniform vec3 camPos;
uniform mat4 depthMVP;


void main()
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

	//calculate shadows
	vec3 projCoords = FragPosLightSpace.xyz / FragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    float currentDepth = projCoords.z;
    float shadow = currentDepth > closestDepth ? 1.0 : 0.0;
	
	//final texture
	FragColor = texture (diffuse0, texCoord) * ((diffuse + specular) + ambient) * lightColor;
	//FragColor = vec4(currentDepth);
}

