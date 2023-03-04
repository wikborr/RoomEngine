#version 330 core
out vec4 outColor;

in vec3 texCoord;

uniform samplerCube skybox;

void main(){
	outColor = texture(skybox,texCoord);
}