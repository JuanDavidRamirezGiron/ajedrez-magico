#version 330 core

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
	// luz ambiente
	float ambient = 0.4f;

	// luz difusa
	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(lightPos);
	float diffuse = max(dot(normal, lightDirection), 0.0f);
	
	// luz especular 
	float specularLight = 0.9f;
	vec3 viewDirection = normalize(camPos - crntPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
	float specular = specAmount * specularLight;

	//sombras
	float shadow = 0.0f;
	// Sets lightCoords to cull space
	vec3 lightCoords = FragPosLightSpace.xyz / FragPosLightSpace.w;
	if(lightCoords.z <= 1.0f)
	{
		// Get from [-1, 1] range to [0, 1] range just like the shadow map
		lightCoords = (lightCoords + 1.0f) / 2.0f;
		float currentDepth = lightCoords.z;
		// Prevents shadow acne
		float bias = max(0.025f * (1.0f - dot(normal, lightDirection)), 0.0005f);

		// Smoothens out the shadows
		int sampleRadius = 2;
		vec2 pixelSize = 1.0 / textureSize(shadowMap, 0);
		for(int y = -sampleRadius; y <= sampleRadius; y++)
		{
		    for(int x = -sampleRadius; x <= sampleRadius; x++)
		    {
		        float closestDepth = texture(shadowMap, lightCoords.xy + vec2(x, y) * pixelSize).r;
				if (currentDepth > closestDepth + bias)
					shadow += 1.0f;     
		    }    
		}
		// Get average shadow
		shadow /= pow((sampleRadius * 2 + 1), 2);
	}
	
	//calculo del color
	FragColor = texture (diffuse0, texCoord) * ((diffuse + specular) * (1-shadow) + ambient) * lightColor; 
}