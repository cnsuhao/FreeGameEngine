#ifdef AS_VS

attribute vec4  a_position/*:POSITION*/;
attribute vec2  a_texCoord0/*:TEXCOORD0*/;

uniform mat4	u_matWorld;//ViewProj;

#else //AS_VS

#ifdef GL_ES
	precision mediump float;
#endif
uniform sampler2D u_texture0;

#endif //AS_VS

varying vec2    v_texCoord0;


#ifdef AS_VS

void main()
{
	gl_Position = a_position * u_matWorld;//ViewProj;
	v_texCoord0 = a_texCoord0;
}

#else

uniform vec4 u_actionAlpha;

void main()
{
    vec4 tmp = texture2D(u_texture0, v_texCoord0);
    tmp *= u_actionAlpha;
	gl_FragColor = tmp;
}

#endif
