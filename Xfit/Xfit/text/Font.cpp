#include "Font.h"

void Font::Init(size_t _charImageMaxLen) {
	charImages.Alloc(_charImageMaxLen);
	FT_Init_FreeType(&library);
}
void Font::Release() {
	charImages.Free();
	FT_Done_FreeType(library);
}
Font::Font(void* _data, unsigned _size, unsigned _index/* = 0*/) {FT_New_Memory_Face(library, (FT_Byte*)_data, _size, _index, &face);}

bool Font::IsBold()const {return face->style_flags& FT_STYLE_FLAG_BOLD;}
bool Font::IsItalic()const {return face->style_flags& FT_STYLE_FLAG_ITALIC;}
Font::~Font() {FT_Done_Face(face);}
const char* Font::GetFamilyName()const {return face->family_name;}
const char* Font::GetStyleName()const {return face->style_name;}
bool Font::IsKerning()const {return face->face_flags& FT_FACE_FLAG_KERNING;}
bool Font::IsHinting()const {return face->face_flags& FT_FACE_FLAG_HINTER;}
unsigned Font::GetIndex()const { return face->face_index; }