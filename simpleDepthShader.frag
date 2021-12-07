#version 330 core
out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D depthMap;

void main()
{             
	//profundidad = componente z desde el pov de la camara
	gl_FragDepth = gl_FragCoord.z;
}  