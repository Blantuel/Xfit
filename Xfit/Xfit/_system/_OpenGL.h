#pragma once


#ifdef __ANDROID__

#include "../system/System.h"


namespace _System::_OpenGL {
	void Release();
	void Init(System::CreateInfo* _info);
	void Resize();


	inline GLuint imgProg;
	inline GLuint shapeProg;
	inline GLuint imgInsProg;
	inline GLuint imgMultiInsProg;

	inline GLuint imgVertProg;
	inline GLuint imgInsVertProg;
	inline GLuint imgMultiInsVertProg;
	inline GLuint shapeVertProg;
	inline GLuint imgFragProg;
	inline GLuint imgMultiInsFragProg;
	inline GLuint shapeFragProg;
	inline GLuint progPipeline;

	inline GLuint vao;

	namespace img {
		inline GLint matUniform;
        inline GLint viewMatUniform;
		inline GLint colorMatUniform;
		inline GLint samplerUniform;
	}
	namespace shape {
		inline GLint matUniform;
		inline GLint colorUniform;
	}
	namespace imgIns {
        inline GLint matUniform;
        inline GLint viewMatUniform;
		inline GLint colorMatUniform;
		inline GLint samplerUniform;
	}
	namespace imgMultiIns {
		inline GLint colorMatUniform;
		inline GLint samplerUniform;
	}

	namespace imgVert {
		inline GLint matUniform;
        inline GLint viewMatUniform;
	}
	namespace imgFrag {
		inline GLint colorMatUniform;
		inline GLint samplerUniform;
	}
	namespace imgMultiInsFrag {
		inline GLint colorMatUniform;
		inline GLint samplerUniform;
	}
	namespace shapeVert {
		inline GLint matUniform;
	}
	namespace shapeFrag {
		inline GLint colorUniform;
	}
}

#endif