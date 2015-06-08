#include "com.glsl"

#ifdef AS_VS

void main()
{
	SIMPLE_TRANSLATION;
	v_texCoord0 = a_texCoord0;
}

#else

void main()
{
	gl_FragColor = texture2D(u_texture0, v_texCoord0);
}

#endif
