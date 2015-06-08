
#ifdef AS_VS

attribute vec4 	a_position/*:POSITION*/;
attribute vec4 	a_color/*:COLOR*/;
attribute vec2	a_texCoord0/*:TEXCOORD0*/;

uniform mat4 u_matWorldViewProj;

#else //AS_PS

#ifdef GL_ES
	precision mediump float;
#endif

uniform sampler2D u_texture0;

#endif

varying vec4 v_color;
varying vec2 v_texCoord0;


#ifdef AS_VS

void main()
{
	gl_Position = a_position * u_matWorldViewProj;
	v_color = a_color.bgra;
	v_texCoord0 = a_texCoord0;
}

#else

void main()
{
	gl_FragColor = v_color * texture2D(u_texture0, v_texCoord0);
}

#endif
