#version 300 es
precision mediump float;
layout(location = 0) in vec2 pos;
layout(location = 1) in vec2 UV;
layout(location = 2) in mat4 insMat;


out vec2 outUV;

uniform mat4 matUniform;
uniform mat4 viewMatUniform;

void main() {
	gl_Position =  viewMatUniform * matUniform * insMat * vec4(pos, 0.f , 1.f);

	outUV = UV;
}


