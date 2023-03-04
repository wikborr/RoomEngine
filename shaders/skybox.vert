#version 330 core
layout (location = 0) in vec3 inPos;

out vec3 texCoord;

uniform mat4 transformation;

void main(){
	texCoord = inPos;
	gl_Position = transformation * vec4(inPos, 1.0);
}