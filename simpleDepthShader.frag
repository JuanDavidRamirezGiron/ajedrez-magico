#version 330 core
out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D depthMap;

void main()
{             
	gl_FragDepth = gl_FragCoord.z;
	//FragColor = vec4(gl_FragCoord.z);

}  