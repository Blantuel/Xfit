#version 400
out vec4 out_Color;

in vec2 outUV;
flat in uint outImgIndex;

uniform mat4 colorMatUniform;
uniform sampler2DArray samplerUniform;

void main(){
	out_Color = colorMatUniform*texture(samplerUniform,vec3(outUV,outImgIndex));
}