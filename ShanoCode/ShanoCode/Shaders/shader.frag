#version 410

out vec4 color;

in vec4 vCol;
in vec2 TexCoord;
in vec3 normal;
in vec3 FragPos;

const int MAX_POINT_LIGHT = 10;
const int MAX_SPOT_LIGHT = 10;

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

uniform sampler2D tex ;
uniform Material material;
uniform vec3 eyePos;

//Name explains it
vec4 CalcLightByDirection(Light light, vec3 direction)
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

	return (ambientColor + diffuseColor + specularColor);
}
//Name explains it
vec4 CalcPointLight(PointLight pLight)
{
	vec3 direction = FragPos - pLight.position;
		float distance = length(direction);
		direction = normalize(direction);

		vec4 color = CalcLightByDirection(pLight.base, direction);
		float attenuation = pLight.exponent * distance * distance +
							pLight.linear * distance + 
							pLight.constant;
							//ax^ + bx+c
		
		return (color/attenuation);
}
//Name explains it
vec4 CalcSpotLight(SpotLight sLight)
{
	vec3 rayDirection = normalize(FragPos - sLight.base.position);
	float slFactor = dot(rayDirection, sLight.direction);

	//check if its in the cone
	if(slFactor > sLight.edge)
	{
		vec4 color = CalcPointLight(sLight.base);

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
	return CalcLightByDirection(directionalLight.base, directionalLight.direction);
}

vec4  CalcPointLights()
{
	vec4 totalCol = vec4(0,0,0,1);
	for(int i = 0; i< pointLightCount; i++)
	{	
		totalCol += CalcPointLight(PointLights[i]);
	}

	return totalCol;
}

vec4 CalcSpotLights()
{
	vec4 totalCol = vec4(0,0,0,1);
	for(int i = 0; i< spotLightCount; i++)
	{		
		totalCol += CalcSpotLight(SpotLights[i]);
	}

	return totalCol;
}
void main()
{
	vec4 final = CalcDirectionalLight();
	final += CalcPointLights();
	final += CalcSpotLights();

	color = texture(tex, TexCoord) * final;

}
											
