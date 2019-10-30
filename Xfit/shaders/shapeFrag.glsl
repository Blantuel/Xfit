#version 400

out vec4 out_Color;

uniform vec4 colorUniform;

void main(){
	out_Color = colorUniform;
}