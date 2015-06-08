#include "com.glsl"
#include "light_com.glsl"

#ifdef AS_VS

uniform mat4 u_matView;
uniform mat4 u_matProj;

void main()
{
	vec3 normal = normalize( (vec4( a_normal, 0 ) * u_matWorld).xyz );
	v_color = computeAllLight(normal);
	v_texCoord0 = a_texCoord0;

	//mat4 matTemp = u_matProj * u_matView * u_matWorld;
	mat4 matTemp = u_matWorld * u_matView * u_matProj;
	gl_Position = a_position * matTemp;

	//SIMPLE_TRANSLATION;
}

#else

void main()
{
	gl_FragColor = v_color * texture2D(u_texture0, v_texCoord0);
}

#endif
