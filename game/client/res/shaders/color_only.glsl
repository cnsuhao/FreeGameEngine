#include "com.glsl"

#ifdef AS_VS

void main()
{
    SIMPLE_TRANSLATION;
    v_color = a_color;
}

#else

void main()
{
	gl_FragColor = v_color;
}

#endif
