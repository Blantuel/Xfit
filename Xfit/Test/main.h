#pragma once

#include <system/System.h>
#include <sound/Sound.h>

extern mt19937_64 random;


constexpr unsigned uiButtonPx = 120;
constexpr unsigned windowWidth = 1280;
constexpr unsigned windowHeight = 720;

class Font;

inline Font* font;
inline Font* boldFont;

enum class Scene {
	Title
};

inline Scene scene;

class Title;
class Option;

inline Title* title;
inline Option* option;

struct FontRender;

inline FontRender* fontRender;

inline Sound* bgSound;
inline void* bgSoundOutData;
inline SoundSource source;

inline bool sizeRequest = true;

inline float textPx = 1.f;

void Main_Size(void* _data);
