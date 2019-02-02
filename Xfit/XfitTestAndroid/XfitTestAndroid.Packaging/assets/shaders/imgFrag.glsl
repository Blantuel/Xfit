#version 300 es
precision mediump float;
out vec4 out_Color;

in vec2 outUV;

uniform sampler2D samplerUniform;

void main(){
	out_Color = texture(samplerUniform,outUV);
}