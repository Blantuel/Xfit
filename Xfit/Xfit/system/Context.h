#pragma once

#include "../stdafx.h"



class Context {
	//HGLRC context;
	

#ifdef _WIN32
	ID3D11DeviceContext* context;
	ID3D11DeviceContext1* context1;
	ID3D11CommandList* commandList;
#elif __ANDROID__
	EGLContext context;
#endif
public:
	Context();
	~Context();
	void Link();
	void Finish();
	void Unlink();
	void Execute();

	static void GlobalLock();
	static void GlobalUnlock();
};