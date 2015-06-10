#ifdef AS_VS

attribute vec4  a_position/*:POSITION*/;
attribute vec3  a_normal/*:NORMAL*/;
attribute vec2  a_texCoord0/*:TEXCOORD0*/;

uniform mat4	u_matWorld;
uniform mat4	u_matWorldViewProj;

#else //AS_VS

#ifdef GL_ES
	precision mediump float;
#endif

uniform sampler2D u_texture0;

#endif //AS_VS

varying vec3 	v_normal;
varying vec2    v_texCoord0;

#ifdef AS_VS

uniform mat4 u_matView;
uniform mat4 u_matProj;

void main()
{
	v_normal = normalize( (vec4( a_normal, 0 ) * u_matWorld).xyz );
	v_texCoord0 = a_texCoord0;

	gl_Position = a_position * u_matWorldViewProj;
}

#else

const vec3 lightDir = vec3(1.0, 1.0, 1.0);
const vec4 lightColor = vec4(1.0, 1.0, 1.0, 1.0);
const vec4 ambientColor = vec4(0.3, 0.3, 0.3, 1.0);

vec4 simpleLight(vec3 normal)
{
	float factor = max(0.0, dot(normalize(normal), normalize(lightDir)));
	return ambientColor + lightColor * factor;
}

void main()
{
	vec4 color = texture2D(u_texture0, v_texCoord0);
	vec4 diffuse = simpleLight(v_normal);
	gl_FragColor = vec4(color.xyz * diffuse.xyz, color.a);
}

#endif
