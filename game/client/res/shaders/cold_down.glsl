#ifdef AS_VS

attribute vec4  a_position/*:POSITION*/;
attribute vec2  a_texCoord0/*:TEXCOORD0*/;

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
	gl_Position = a_position;
	v_texCoord0 = a_texCoord0;
}

#else

uniform float u_coldDownAlpha;

void main()
{
	vec4 diffuseMap = texture2D( u_texture0, v_texCoord0 );	
	
	if ( u_coldDownAlpha < diffuseMap.a )
		discard;
	
	gl_FragColor = vec4(0.5, 0.5, 0.5, 0.5);
}

#endif
