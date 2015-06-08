#ifdef AS_VS

struct DirLight
{
    vec4  direction;
    vec4  color;
};

struct Material
{
    vec4 ambient;
    vec4 diffuse;
    vec4 emissive;
};

uniform vec4        u_ambient;
uniform DirLight    u_dirLight;
uniform Material    u_material;

vec4 computeDirLight(vec3 normal)
{
	vec4 color;
	float factor;
    factor = max(0.0, dot(u_dirLight.direction.xyz, normal));
	color = u_dirLight.color * u_material.diffuse * factor;
	return color;
}

vec4 computePointLight()
{
    return vec4(0.0, 0.0, 0.0, 1.0);
}

vec4 computeSpotLight()
{
    return vec4(0.0, 0.0, 0.0, 1.0);
}

vec4 computeAllLight(vec3 normal)
{
	vec4 color = u_material.emissive;
    
    color += u_ambient * u_material.ambient;
	color += computeDirLight(normal);
    
    color.w = 1.0;
	return color;
}

#else


const vec3 MASK_COLOR = vec3(1.5, 0.3, 0.0);
const vec3 LUMINANCE_COLOR = vec3(0.299, 0.587, 0.114);

vec4 makeTextureRed()
{
    vec4 color = texture2D(u_texture0, v_texCoord0);
    float factor = dot(color.xyz, LUMINANCE_COLOR);
    return vec4(MASK_COLOR * factor, color.w * 0.6);
}

const float HIGHT_LIGHT_FACTOR = 1.5;
vec4 makeTextureHighlight()
{
    vec4 color = texture2D(u_texture0, v_texCoord0);
    return vec4(color.xyz * HIGHT_LIGHT_FACTOR, color.w);
}

#endif
