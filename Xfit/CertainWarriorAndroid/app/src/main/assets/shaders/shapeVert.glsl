#version 300 es
precision highp float;

layout(location = 0) in vec2 pos;

uniform mat4 matUniform;
uniform mat4 viewMatUniform;

void main() {
	gl_Position = viewMatUniform * matUniform * vec4(pos, 0.f , 1.f);
}


