#version 400

out vec4 out_Color;

in vec2 outUV;

uniform mat4 colorMatUniform;
uniform sampler2D samplerUniform;

void main(){
	out_Color = colorMatUniform*texture(samplerUniform,outUV);
}