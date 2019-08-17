#version 410

out vec4 color;

in vec4 vCol;
in vec2 TexCoord;
in vec3 normal;
in vec3 FragPos;
in vec4 DirLightSpacePos;

const int MAX_POINT_LIGHT = 8;
const int MAX_SPOT_LIGHT = 3;

struct Light 
{
	vec3 color;
	float ambientIntesity;
	float diffuseIntensity;
};

struct DirectionalLight 
{
	Light base;
	vec3 direction;
};

struct PointLight
{
	Light base;
	vec3 position;
	float constant;
	float linear;
	float exponent;
};

struct SpotLight
{
	PointLight base;
	vec3 direction;
	float edge;
};


struct Material
{
	float specIntensity;
	float shininess;
};

uniform int pointLightCount;
uniform int spotLightCount;

uniform DirectionalLight directionalLight;
uniform PointLight PointLights[MAX_POINT_LIGHT];
uniform SpotLight SpotLights[MAX_SPOT_LIGHT];

uniform sampler2D tex;
uniform sampler2D normalM;
uniform sampler2D shadow;
uniform samplerCube shadowCube[MAX_POINT_LIGHT+MAX_SPOT_LIGHT];

uniform Material material;

uniform vec3 eyePos;
uniform float far_plane;


float shadowCalc(vec3 pos, int i)
{
	vec3 fragToLight = FragPos - pos;

	float closestDepth = texture(shadowCube[i], fragToLight).r;

	closestDepth *= far_plane;

	float currentDepth = length(fragToLight);
	float bias = 0.05;
	float shadows = currentDepth - bias > closestDepth ? 1.0 : 0.0;

	return shadows;
}

float CalcDirectionalShadowFactor(DirectionalLight light)
{
	vec3 projCoords = DirLightSpacePos.xyz / DirLightSpacePos.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadow, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
	float bias = 0.005;
    float shadows = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
	if(projCoords.z > 1.0)
			shadows = 0.0;
    return shadows;
}

//Name explains it
vec4 CalcLightByDirection(Light light, vec3 direction, float shadowFactor)
{
	vec4 ambientColor = vec4(light.color, 1.0f) * light.ambientIntesity;
	//base off of the angle of the light towards a surface
	vec3 fix = vec3(-direction.x, -direction.y, -direction.z);
	float diffuseFactor = 0.2f *  max(dot(normalize(normal), normalize(fix)), 0.0f) ;
	//diffuse
	vec3 diffCol = light.color * light.diffuseIntensity * diffuseFactor;
	vec4 diffuseColor = vec4(diffCol, 1.0f);

	//specular
	vec4 specularColor = vec4(0, 0, 0, 0);
	if(diffuseFactor > 0.0f)
	{
		vec3 fragToEye = normalize(eyePos - FragPos);
		vec3 reflectedVert = normalize(reflect(direction, normalize(normal)));

		float specFactor = dot(fragToEye, reflectedVert);

		if(specFactor> 0.0f)
		{
			specFactor = pow(specFactor, material.shininess);
			specularColor = vec4(light.color * material.specIntensity * specFactor, 1.0f);
		}
	}

	return (ambientColor + (1.0 - shadowFactor) * (diffuseColor + specularColor));
}
//Name explains it
vec4 CalcPointLight(PointLight pLight, int i)
{
	vec3 direction = FragPos - pLight.position;
		float distance = length(direction);
		direction = normalize(direction);
		float shadowf = shadowCalc(pLight.position, i);

		vec4 color = CalcLightByDirection(pLight.base, direction , shadowf);
		
		float attenuation = pLight.exponent * distance * distance +
							pLight.linear * distance + 
							pLight.constant;
							//ax^ + bx+c
		
		return (color/attenuation);
}
//Name explains it
vec4 CalcSpotLight(SpotLight sLight, int i)
{
	vec3 rayDirection = normalize(FragPos - sLight.base.position);
	float slFactor = dot(rayDirection, sLight.direction);

	//check if its in the cone
	if(slFactor > sLight.edge)
	{
		vec4 color = CalcPointLight(sLight.base, i);

		return color * (1.0f - (1.0f - slFactor)*(1.0f/(1.0f - sLight.edge)));
	} 
	else
	{
		return vec4(0,0,0,0);
	}


}

//Use the functions and get the lights
vec4 CalcDirectionalLight()
{
	float shadowFactor = CalcDirectionalShadowFactor(directionalLight);
	return CalcLightByDirection(directionalLight.base, directionalLight.direction, shadowFactor);
}

vec4  CalcPointLights()
{
	vec4 totalCol = vec4(0,0,0,1);
	for(int i = 0; i< pointLightCount; i++)
	{	
		totalCol += CalcPointLight(PointLights[i], i);
	}

	return totalCol;
}

vec4 CalcSpotLights()
{
	vec4 totalCol = vec4(0,0,0,1);
	for(int i = 0; i< spotLightCount; i++)
	{		
		totalCol += CalcSpotLight(SpotLights[i], MAX_POINT_LIGHT+i);
	}

	return totalCol;
}
void main()
{
	vec4 final = CalcDirectionalLight();
	final += CalcPointLights();
	final += CalcSpotLights();

	//color = vec4(vec3(closestDepth/far_plane ), 1.0f);
	color = texture(tex, TexCoord) * final;
}
											
