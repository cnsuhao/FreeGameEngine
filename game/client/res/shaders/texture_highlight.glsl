#include "com.glsl"

#ifdef AS_VS

void main()
{
	SIMPLE_TRANSLATION;
    v_texCoord0 = a_texCoord0;
}

#else

const float HIGHT_LIGHT_FACTOR = 1.5;
const vec3 LUMINANCE_COLOR = vec3(0.299, 0.587, 0.114);

void main()
{
	vec4 color = texture2D(u_texture0, v_texCoord0);
	float factor = 1.0 + HIGHT_LIGHT_FACTOR * dot(color.xyz, LUMINANCE_COLOR);

    gl_FragColor = vec4(color.xyz * factor, color.w);
}

#endif
