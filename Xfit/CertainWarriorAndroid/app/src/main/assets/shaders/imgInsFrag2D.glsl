#version 300 es
precision highp float;
out vec4 out_Color;

in vec2 outUV;
in mat4 outColorMat;

uniform sampler2D samplerUniform;

void main(){
	out_Color = outColorMat * texture(samplerUniform,outUV);
}