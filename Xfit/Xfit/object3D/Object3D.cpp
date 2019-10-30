#include "object3D.h"
#include "../view/Camera3D.h"
#include "../effect/Blend.h"

#include "../_system/_OpenGL.h"
#include "../_system/_Vulkan.h"


Object3D::Object3D(float _x, float _y, float _z, float _rotationX, float _rotationY, float _rotationZ,float _scaleX, float _scaleY, float _scaleZ) {
	
}


bool Object3D::Draw(unsigned index, unsigned viewIndex) {
	return true;
}
