
#ifdef AS_VS

attribute vec4  a_position/*:POSITION*/;
attribute vec2  a_texCoord0/*:TEXCOORD0*/;
attribute vec4	a_texColor/*:COLOR*/;

#else //AS_VS

#ifdef GL_ES
	precision mediump float;
#endif
uniform sampler2D u_texture0;
uniform sampler2D u_texture1;

#endif //AS_VS

varying vec2    v_texCoord0;
varying vec4	v_texColor;

#ifdef AS_VS

void main()
{
	gl_Position = a_position;
	v_texCoord0 = a_texCoord0;
	v_texColor = a_texColor.bgra;
}

#else

void main()
{
	vec4 diffuseMap = texture2D( u_texture0, v_texCoord0 );	
	vec4 secondMap = texture2D( u_texture1, v_texCoord0 );

	float cutoff = 1.0 - v_texColor.a;
	vec4 factor = vec4(1.0, 1.0, 1.0, 1.0);

	float origLum = clamp( dot(diffuseMap, vec4(0.299, 0.587, 0.114, 0.)), 0.0, 1.0);
	float lum = max( origLum, 0.1 ) * (1.0 + clamp( 2.0 - (cutoff - secondMap.a) * 40.0, 0.0, 1.0 ) );

	gl_FragColor =  secondMap.a >= cutoff ? diffuseMap * factor : vec4(clamp( factor.xyz * lum, 0.0, 1.0 ), diffuseMap.a * factor.a);
}

#endif
