#include "Font.h"

FT_Library Font::library = nullptr;
Array<Font::CharImage> Font::charImages;
size_t Font::charImageBlockLen;
mutex Font::mutex;
thread_local unsigned* Font::widths;
thread_local unsigned* Font::heights;
thread_local int* Font::paddings;
thread_local Font::CharImage** Font::charImages_t;
thread_local unsigned *Font::image;
thread_local unsigned Font::imageSize;
thread_local unsigned Font::blockImageSize;

void Font::Init(size_t _charImageMaxLen, size_t _charImageBlockLen) {
	FT_Init_FreeType(&library);
	charImages.Alloc((size_t)_charImageMaxLen);
	charImageBlockLen = _charImageBlockLen;
}
void Font::ThreadInit(size_t _maxLine, size_t _maxLen, size_t _maxImageSize, size_t _blockImageSize) {
	widths = new unsigned[_maxLine];
	heights = new unsigned[_maxLine];
	paddings = new int[_maxLine];
	charImages_t = new CharImage*[_maxLen];
	image = new unsigned[_maxImageSize];
	imageSize = _maxImageSize;
	blockImageSize = _blockImageSize;
}
void Font::ThreadRelease() {
	delete[]widths;
	delete[]heights;
	delete[]paddings;
	delete[]charImages_t;
	delete[]image;
}
void Font::Release() {
	charImages.Free();
	FT_Done_FreeType(library);
}
Font::Font(void* _data, unsigned _size, unsigned _index/* = 0*/) {
	FT_New_Memory_Face(library, (FT_Byte*)_data, _size, _index, &face);
}

bool Font::IsBold()const {return face->style_flags& FT_STYLE_FLAG_BOLD;}
bool Font::IsItalic()const {return face->style_flags& FT_STYLE_FLAG_ITALIC;}
Font::~Font() {
	Font::mutex.lock();
	for(int i=charImages.Size()-1;i>=0;i--) {
		if(charImages[i].font==this)charImages.EraseIndex(i);
	}
	Font::mutex.unlock();
	FT_Done_Face(face);
}
const char* Font::GetFamilyName()const {return face->family_name;}
const char* Font::GetStyleName()const {return face->style_name;}
bool Font::IsKerning()const {return face->face_flags& FT_FACE_FLAG_KERNING;}
bool Font::IsHinting()const {return face->face_flags& FT_FACE_FLAG_HINTER;}
unsigned Font::GetIndex()const { return face->face_index; }