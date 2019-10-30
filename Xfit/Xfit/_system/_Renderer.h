#pragma once

#include "../system/System.h"


namespace _System::_Renderer {
	inline bool vSync;
	inline unsigned msaaCount, msaaQuality;

	inline System::RendererVersion version = { System::RendererName::None,0,0 };

	inline float backColor[4] = { 0.f,0.f,0.f,1.f };
	constexpr float _backColorZero[4] = { 0.f,0.f,0.f,0.f };

	inline PointU windowSize;
}