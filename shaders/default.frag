#version 330 core
out vec4 outColor;
float AMBIENT = 0.1;

in vec3 normal;
in vec2 texCoord;
in vec3 worldPos;
in vec3 dirLightPos;
uniform vec3 camPos;

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

//texture uniforms
uniform vec4 color = vec4(1.f);
uniform sampler2D texAlbedoOrDiffuse;
uniform sampler2D texMetallic;
uniform sampler2D texRoughness;
uniform sampler2D texAO;
uniform sampler2D texNormal;
uniform sampler2D texParallax;
uniform sampler2D texBaseColor;
uniform sampler2D texAMR;
//for checking if they're active
uniform bool texAlbedoActive = false;
uniform bool texMetallicActive = false;
uniform bool texRoughnessActive = false;
uniform bool texAOActive = false;
uniform bool texNormalActive = false;
uniform bool texParallaxActive = false;
uniform bool texBaseColorActive = false;
uniform bool texAMRActive = false;

in vec3 viewDirTS;
in vec3 dirLightDirTS;
in vec3 spotLight1DirTS;

vec3 PBR(vec3 lightDir, vec3 lightColor, vec3 normal, vec3 viewDir, vec3 color){
	vec3 H =normalize(viewDir + lightDir);
	vec3 R = reflect(-lightDir, normal);  
	float specular = pow(max(dot(viewDir, R), 0.0), 32);

	//float k_s = calculateSpecularComponent(R/specular);
	float PI = 3.14;
	float a = 1.;

	float a2 = a*a;
	float NdotH = max(dot(normal, H), 0.0);
	float NdotH2 = NdotH*NdotH;
	float denom  = (NdotH2 * (a2 - 1.0) + 1.0);

	float D = a2/(PI * denom * denom);

	float k = ((a+1)*(a+1))/8;

	float NdotV   = max(dot(normal, viewDir), 0.0);
    float denom2 = NdotV * (1.0 - k) + k;

	float GeometrySchlickGGX1 = NdotV / denom2;

	float NdotL = max(dot(normal, lightDir), 0.0);

	float GeometrySchlickGGX2 = NdotL / denom2;

	float ggx1 = GeometrySchlickGGX1;
	float ggx2 = GeometrySchlickGGX2;

	float GeometrySmith = ggx1 * ggx2;

	float cosTheta = dot(lightDir, normal);  

	vec3 F0 = vec3(0.04);
	F0  = mix(F0, color, 0.5); //ost metalness

	vec3 fresnelSchlick = F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
	vec3 k_d = 1.0 - fresnelSchlick;

	return ((k_d* (color/PI))+(D * GeometrySmith * fresnelSchlick)/(4*dot(viewDir,normal)*dot(lightDir,normal)))*lightColor*dot(lightDir,normal);
}

vec3 phongLight(vec3 lightDir, vec3 lightColor, vec3 normal,vec3 viewDir, vec3 color){
	float diffuse=max(0,dot(normal,lightDir));
	vec3 R = reflect(-lightDir, normal);  

	float specular = pow(max(dot(viewDir, R), 0.0), 32);

	vec3 resultColor = color*diffuse*lightColor + lightColor*specular;
	return resultColor;
}

void main(){
	vec3 finalColor = color.rgb;
	if(texBaseColorActive){
		finalColor = (texture(texBaseColor, texCoord)*vec4(finalColor, 1.f)).rgb;
	}
	if(texAlbedoActive){
		finalColor = (texture(texAlbedoOrDiffuse, texCoord)*vec4(finalColor, 1.f)).rgb;
	}

	vec3 normal = vec3(0,0,1);
	vec3 viewDir = normalize(viewDirTS);
	vec3 ilumination = vec3(1.f, 1.f, 1.f);
	vec3 dirExposition = vec3(1.f, 1.f, 1.f); //directional light has always the same exposition

	//directional light
	vec3 lightDir = normalize(dirLightPos-worldPos);
	vec3 ambient = AMBIENT*finalColor;
	vec3 attenuatedlightColor = dirLight.color/pow(length(dirLightPos-worldPos),2);
	ilumination = ambient + PBR(normalize(dirLightDirTS),attenuatedlightColor,normal,viewDir, finalColor);

	//spot light (like flashlight)
	if(spotLight1.angle > 0.f){
		vec3 spotlightDir= normalize(spotLight1.position-worldPos);
		float angle_atenuation = clamp((dot(-spotLight1DirTS,spotLight1.direction)-0.8)*3,0,1);
		attenuatedlightColor = spotLight1.color/pow(length(spotLight1.position-worldPos),2)*angle_atenuation;
		ilumination=ilumination+PBR(normalize(spotLight1DirTS),attenuatedlightColor,normal,viewDir, finalColor);
	}
	
	//outColor = vec4(1.0 - exp(-ilumination*dirExposition),1);
	outColor = vec4(finalColor, 1.f);
}