#version 400
layout(location = 0) in vec2 pos;
layout(location = 1) in vec2 UV;
layout(location = 2) in mat4 insMat;
layout(location = 6) in uvec4 imgIndex;

out gl_PerVertex
{
    vec4 gl_Position;
};

out vec2 outUV;
flat out uint outImgIndex;


void main() {
	gl_Position =  insMat*vec4(pos, 0.f , 1.f);

	outUV = UV;
	outImgIndex = imgIndex.x;
}

