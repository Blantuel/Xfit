#version 300 es
precision highp float;
layout(location = 0) in vec2 pos;
layout(location = 1) in vec2 UV;
layout(location = 2) in mat4 insMat;
layout(location = 6) in mat4 colorMat;
layout(location = 10) in int imgIndex;

out vec2 outUV;
out mat4 outColorMat;
flat out int outImgIndex;

uniform mat4 matUniform;
uniform mat4 viewMatUniform;

void main() {
	gl_Position = viewMatUniform * matUniform * insMat * vec4(pos, 0.f , 1.f);

	outUV = UV;
	outColorMat = colorMat;
	outImgIndex = imgIndex;
}


