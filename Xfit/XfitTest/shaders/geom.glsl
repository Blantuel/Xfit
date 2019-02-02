#version 430
#extension GL_ARB_geometry_shader4 : enable

layout(points) in;
layout(triangles, max_vertices = 3) out;

void main() {
	gl_Position = vec4(position,1 , 1);
}