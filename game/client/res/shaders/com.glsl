#ifdef AS_VS

attribute vec4  a_position/*:POSITION*/;
attribute vec3  a_normal/*:NORMAL*/;
attribute vec4	a_color/*:COLOR*/;
attribute vec2  a_texCoord0/*:TEXCOORD0*/;


uniform mat4    u_matWorld;
uniform mat4	u_matWorldViewProj;

#define SIMPLE_TRANSLATION gl_Position = a_position * u_matWorldViewProj;

#else //AS_VS

#ifdef GL_ES
	precision mediump float;
#endif
uniform sampler2D u_texture0;

#endif //AS_VS

varying vec2    v_texCoord0;
varying vec4    v_color;
