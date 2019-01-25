#include "Text.h"

namespace Text {
	size_t GetCharArLen(const Tchar* _c) {
#ifdef UNICODE
		return wcslen(_c);
#else
		return strlen(_c);
#endif
	}
}