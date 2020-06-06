#pragma once

#include "../system/System.h"


namespace _System::_Renderer {
	inline bool vSync;
	inline unsigned msaaCount, msaaQuality;

	inline unsigned refleshRateTop;
	inline unsigned refleshRateBottom;

	inline System::RendererVersion version = { System::RendererName::None,0,0 };

	inline unsigned versionNumber;

	inline float backColor[4] = { 0.f,0.f,0.f,1.f };
	constexpr float _backColorZero[4] = { 0.f,0.f,0.f,0.f };

	inline PointU windowSize;
	inline float windowRatio = 0.f;

#ifdef _WIN32
	inline Point windowPos;
#endif
}