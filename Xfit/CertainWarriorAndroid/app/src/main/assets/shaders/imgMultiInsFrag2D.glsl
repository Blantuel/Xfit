#version 300 es
precision highp float;
out vec4 out_Color;

in vec2 outUV;
in mat4 outColorMat;
flat in int outImgIndex;

uniform sampler2DArray samplerUniform;

void main(){
	out_Color = outColorMat * texture(samplerUniform, vec3(outUV, float(outImgIndex)));
}