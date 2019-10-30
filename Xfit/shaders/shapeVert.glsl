#version 400

layout(location = 0) in vec2 pos;

out gl_PerVertex
{
    vec4 gl_Position;
};
uniform mat4 matUniform;

void main() {
	gl_Position = matUniform*vec4(pos, 0.f , 1.f);
}


