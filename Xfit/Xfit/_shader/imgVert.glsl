#version 430
layout(location = 0) in vec2 pos;
layout(location = 1) in vec2 UV;

out gl_PerVertex
{
    vec4 gl_Position;
};

out vec2 outUV;

uniform mat4 matUniform;

void main() {
	gl_Position = matUniform*vec4(pos, 0.f , 1.f);

	outUV = UV;
}


