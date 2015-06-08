#include "com.glsl"
#include "light_com.glsl"

#ifdef AS_VS

void main()
{
	SIMPLE_TRANSLATION;
    v_texCoord0 = a_texCoord0;
}

#else

void main()
{
    gl_FragColor = makeTextureHighlight();
}

#endif
