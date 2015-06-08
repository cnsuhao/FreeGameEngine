#include "com.glsl"

#ifdef AS_VS

void main()
{
	v_color = a_color;
	v_texCoord0 = a_texCoord0;

	SIMPLE_TRANSLATION;
}

#else

const float PI = 3.141592654;

void main()
{
    if(cos(v_texCoord0.x * PI * 20.0) > 0.5)
        gl_FragColor = vec4(0.0, 0.0, 0.0, 0.0);
    else
        gl_FragColor = v_color;
}

#endif
