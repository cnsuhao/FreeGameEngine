#include "com.glsl"

#ifdef AS_VS

void main()
{
	SIMPLE_TRANSLATION;
    v_texCoord0 = a_texCoord0;
}

#else

const vec3 MASK_COLOR = vec3(1.5, 0.3, 0.0);
const vec3 LUMINANCE_COLOR = vec3(0.299, 0.587, 0.114);

void main()
{
    vec4 color = texture2D(u_texture0, v_texCoord0);
    float factor = dot(color.xyz, LUMINANCE_COLOR);
    gl_FragColor = vec4(MASK_COLOR * factor, color.w * 0.6);
}

#endif
