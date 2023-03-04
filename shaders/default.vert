#version 330 core
layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inTexCoord;
layout(location = 3) in vec3 inTangent;
layout(location = 4) in vec3 inBitangent;

float DIR_LIGHT_DISTANCE = 500;

//light structs
struct DirectionalLight{
	vec3 direction;
	vec3 color;
};
struct PointLight{
	vec3 position;
	float exposition; //if 0 it's not active
	vec3 color;
}; 
struct SpotLight{
	vec3 position;
	vec3 direction;
	float angle; //if 0 it's not active
	vec3 color;
};

//light uniforms
uniform DirectionalLight dirLight = {{0.1f, -1.f, 0.1f},{1.f, 1.f, 1.f}};
uniform PointLight pointLight1 = {{0.f, 0.f, 0.f}, 0.f,{1.f, 1.f, 1.f}};
uniform PointLight pointLight2 = {{0.f, 0.f, 0.f}, 0.f,{1.f, 1.f, 1.f}};
uniform PointLight pointLight3 = {{0.f, 0.f, 0.f}, 0.f,{1.f, 1.f, 1.f}};
uniform SpotLight spotLight1 = {{0.f, 0.f, 0.f},{0.f, 0.f, -1.f}, 0.f,{1.f, 1.f, 1.f}};

out vec3 normal;
out vec2 texCoord;
out vec3 worldPos;
out vec3 dirLightPos;
uniform vec3 camPos;

uniform mat4 modelM;
uniform mat4 viewM;
uniform mat4 transformation;

out vec3 viewDirTS;
out vec3 dirLightDirTS;
out vec3 spotLight1DirTS;

void main(){
	gl_Position = transformation*vec4(inPos, 1.0);
	normal = normalize((modelM* vec4(inNormal,0)).xyz);
	texCoord = inTexCoord;
	worldPos = (modelM* vec4(inPos,1.0)).xyz;

	vec3 w_tangent = normalize(mat3(modelM)*inTangent);
	vec3 w_bitangent = normalize(mat3(modelM)*inBitangent);
	mat3 TBN = transpose(mat3(w_tangent, w_bitangent, normal));
	dirLightPos = normalize(-dirLight.direction)*DIR_LIGHT_DISTANCE;

	vec3 V = normalize(camPos-worldPos);
	viewDirTS = TBN*V;
	vec3 L = normalize(dirLightPos-worldPos);
	dirLightDirTS = TBN*L;
	vec3 SL = normalize(spotLight1.position-worldPos);
	spotLight1DirTS = TBN*SL;
}