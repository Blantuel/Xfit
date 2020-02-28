#pragma once

#include "../data/Array.h"

class Index {
	friend class ImageBase;
	friend class ImageInstance;
	friend class ImageMultiInstance;
	friend class RTAnimateObjectBase;

#ifdef _WIN32
	union {
		ID3D11Buffer* index;
	};
#elif __ANDROID__
	GLuint index;
#endif

	unsigned num;
public:
	Array<unsigned> indices;

	Index();
	~Index();
	void Build(bool _editable = false);
	void Edit();
	bool IsBuild()const;
	void Delete();

	unsigned GetNum()const;
};