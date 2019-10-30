#pragma once

#include "../system/Error.h"

class MemoryError : public Error {
public:
	enum class Code {
		NullData,
		NullData2,
		NullDest,
		NullSrc,
		ZeroSize,
		ZeroDestSize
	};
protected:
	Code code;
public:
	Code GetCode()const{return code;}
	MemoryError(MemoryError::Code _code) :code(_code) {}
};
class Memory{
	template<typename T> static size_t _Search(const T* _ar, const T& _value, size_t _count) {
		if (_count == 0)return UINTPTR_MAX;
		for (size_t s = 0; s < _count; s++) {
			if (_ar[s] == _value)return s;
		}
		return UINTPTR_MAX;
	}
	template<typename T> static T* _Set(T* _ar, const T& _value, size_t _count) {
		if (_count == 0)return _ar;
		for (size_t s = 0; s < _count; s++) _ar[s] = _value;

		return _ar;
	}
public:
	template<typename T> static bool Equal(const T* _ar, const T* _ar2, size_t _count) {
#ifdef _DEBUG
		if(!_ar)throw MemoryError(MemoryError::Code::NullData);
		if(!_ar2)throw MemoryError(MemoryError::Code::NullData2);
		if(_count==0)throw MemoryError(MemoryError::Code::ZeroSize);
#endif
		return memcmp(_ar, _ar2, _count * sizeof(T))==0;
	}
	template <typename T> static size_t Search(const T* _ar, const T&_value, size_t _count) {
#ifdef _DEBUG
		if(!_ar)throw MemoryError(MemoryError::Code::NullData);
		if(_count==0)throw MemoryError(MemoryError::Code::ZeroSize);
#endif
		for (size_t i = 0; i < _count; i++) {
			if (_ar[i] == _value)return i;
		}
		return UINTPTR_MAX;
	}
	static size_t Search(const int* _ar, const int& _value, size_t _count) {
#ifdef _DEBUG
		if(!_ar)throw MemoryError(MemoryError::Code::NullData);
		if(_count==0)throw MemoryError(MemoryError::Code::ZeroSize);
#endif
#ifdef AVX2
		size_t s = 0, ss = _count % 8;
		_count -= ss;
		for (; s < _count; s += 8) {
			__m256i a=_mm256_loadu_si256((__m256i*)(_ar + s));			
			const __m256i b = _mm256_set1_epi32(_value);
			const __m256i result = _mm256_cmpeq_epi32(a, b);
			const unsigned index = _tzcnt_u32(_mm256_movemask_epi8(result)) >> 2;
			if (index < 8)return s+index;
		}
		return _Search(_ar+s, _value, ss);
#elif SSE4
		size_t s = 0, ss = _count % 4;
		_count -= ss;
		for (; s < _count; s += 4) {
			__m128i a = _mm_loadu_si128((__m128i*)(_ar + s));
			const __m128i b = _mm_set1_epi32(_value);
			union {
				__m128i result;
				int ar[4];
			};
			result = _mm_cmpeq_epi32(a, b);

			for (int i = 0; i < 4; i++) {
				if (ar[i] == -1) return s + i;
			}
		}
		return _Search(_ar + s, _value, ss);
#else 
		return _Search(_ar, _value, _count);
#endif
	}
	static size_t Search(const short* _ar, const short& _value, size_t _count) {
#ifdef _DEBUG
		if(!_ar)throw MemoryError(MemoryError::Code::NullData);
		if(_count==0)throw MemoryError(MemoryError::Code::ZeroSize);
#endif
#ifdef AVX2
		size_t s = 0, ss = _count % 16;
		_count -= ss;
		for (; s < _count; s += 16) {
			__m256i a = _mm256_loadu_si256((__m256i*)(_ar + s));
			const __m256i b = _mm256_set1_epi16(_value);
			const __m256i result = _mm256_cmpeq_epi16(a, b);
			const unsigned index = _tzcnt_u32(_mm256_movemask_epi8(result)) >> 1;
			if (index < 16)return s+index;
		}
		return _Search(_ar + s, _value, ss);
#elif SSE4
		size_t s = 0, ss = _count % 8;
		_count -= ss;
		for (; s < _count; s += 8) {
			__m128i a = _mm_loadu_si128((__m128i*)(_ar + s));
			const __m128i b = _mm_set1_epi16(_value);
			union {
				__m128i result;
				short ar[8];
			};
			result = _mm_cmpeq_epi16(a, b);

			for (int i = 0; i < 8; i++) {
				if (ar[i] == -1) return s + i;
			}
		}
		return _Search(_ar + s, _value, ss);
#else 
		return _Search(_ar, _value, _count);
#endif
	}
	static size_t Search(const char* _ar, const char& _value, size_t _count) {
#ifdef _DEBUG
		if(!_ar)throw MemoryError(MemoryError::Code::NullData);
		if(_count==0)throw MemoryError(MemoryError::Code::ZeroSize);
#endif
		const char* result = (const char*)memchr(_ar, _value, _count);
		if (!result)return UINTPTR_MAX;
		return size_t(result - _ar);
	}
	static size_t Search(const long long* _ar, const long long& _value, size_t _count) {
#ifdef _DEBUG
		if(!_ar)throw MemoryError(MemoryError::Code::NullData);
		if(_count==0)throw MemoryError(MemoryError::Code::ZeroSize);
#endif
#ifdef AVX2
		size_t s = 0, ss = _count % 4;
		_count -= ss;
		for (; s < _count; s += 4) {
			__m256i a = _mm256_loadu_si256((__m256i*)(_ar + s));
			const __m256i b = _mm256_set1_epi64x(_value);
			const __m256i result = _mm256_cmpeq_epi64(a, b);
			const unsigned index = _tzcnt_u32(_mm256_movemask_epi8(result)) >> 3;
			if (index < 4)return s+ index;
		}
		return _Search(_ar + s, _value, ss);
#elif SSE4
		size_t s = 0, ss = _count % 2;
		_count -= ss;
		for (; s < _count; s += 2) {
			__m128i a = _mm_loadu_si128((__m128i*)(_ar + s));
			const __m128i b = _mm_set1_epi64x(_value);
			union {
				__m128i result;
				long long ar[2];
			};
			result = _mm_cmpeq_epi64(a, b);

			for (int i = 0; i < 2; i++) {
				if (ar[i] == -1) return s + i;
			}
		}
		return _Search(_ar + s, _value, ss);
#else 
		return _Search(_ar, _value, _count);
#endif
	}
	static size_t Search(const unsigned long long* _ar, const unsigned long long& _value, size_t _count) {
		return Search((long long*)_ar, (long long)_value, _count);
	}
	static size_t Search(const unsigned int* _ar, const unsigned int& _value, size_t _count) {
		return Search((int*)_ar, (int)_value, _count);
	}
	static size_t Search(const unsigned short* _ar, const unsigned short& _value, size_t _count) {
		return Search((short*)_ar, (short)_value, _count);
	}
	static size_t Search(const unsigned char* _ar, const unsigned char& _value, size_t _count) {
		return Search((char*)_ar, (char)_value, _count);
	}
	template<typename T> static T* Set(T* _ar, const T& _value, size_t _count) {
#ifdef _DEBUG
		if(!_ar)throw MemoryError(MemoryError::Code::NullData);
		if(_count==0)throw MemoryError(MemoryError::Code::ZeroSize);
#endif
		for (size_t i = 0; i < _count; i++) _ar[i] = _value;
		return _ar;
	}
	static int* Set(int* _ar, const int &_value, size_t _count) {
#ifdef _DEBUG
		if(!_ar)throw MemoryError(MemoryError::Code::NullData);
		if(_count==0)throw MemoryError(MemoryError::Code::ZeroSize);
#endif
#ifdef AVX2
		size_t ss = _count % 8;
		_count -= ss;
		for (size_t s = 0; s < _count; s += 8) {
			const __m256i a = _mm256_set1_epi32(_value);
			_mm256_storeu_si256((__m256i*)(_ar + s), a);
		}
		return _Set(_ar + _count, _value, ss);
#elif SSE4
		size_t ss = _count % 4;
		_count -= ss;
		for (size_t s = 0; s < _count; s += 4) {
			const __m128i a = _mm_set1_epi32(_value);
			_mm_storeu_si128((__m128i*)(_ar + s), a);
		}
		return _Set(_ar + _count, _value, ss);
#else 
		return _Set(_ar, _value, _count);
#endif
	}
	static short* Set(short* _ar, const short &_value, size_t _count) {
#ifdef _DEBUG
		if(!_ar)throw MemoryError(MemoryError::Code::NullData);
		if(_count==0)throw MemoryError(MemoryError::Code::ZeroSize);
#endif
#ifdef AVX2
		size_t ss = _count % 16;
		_count -= ss;
		for (size_t s = 0; s < _count; s += 16) {
			const __m256i a = _mm256_set1_epi16(_value);
			_mm256_storeu_si256((__m256i*)(_ar + s), a);
		}
		return _Set(_ar + _count, _value, ss);
#elif SSE4
		size_t ss = _count % 8;
		_count -= ss;
		for (size_t s = 0; s < _count; s += 8) {
			const __m128i a = _mm_set1_epi16(_value);
			_mm_storeu_si128((__m128i*)(_ar + s), a);
		}
		return _Set(_ar + _count, _value, ss);
#else 
		return _Set(_ar, _value, _count);
#endif
	}
	static char* Set(char* _ar, char _value, size_t _count) {
#ifdef _DEBUG
		if(!_ar)throw MemoryError(MemoryError::Code::NullData);
		if(_count==0)throw MemoryError(MemoryError::Code::ZeroSize);
#endif
		return (char*)memset((void*)_ar, (int)_value, _count);
	}
	static long long* Set(long long* _ar,const long long& _value, size_t _count) {
#ifdef _DEBUG
		if(!_ar)throw MemoryError(MemoryError::Code::NullData);
		if(_count==0)throw MemoryError(MemoryError::Code::ZeroSize);
#endif
#ifdef AVX2
		size_t ss = _count % 4;
		_count -= ss;
		for (size_t s = 0; s < _count; s += 4) {
			const __m256i a = _mm256_set1_epi64x(_value);
			_mm256_storeu_si256((__m256i*)(_ar + s), a);
		}
		return _Set(_ar+_count, _value, ss);
#elif SSE4
		size_t ss = _count % 2;
		_count -= ss;
		for (size_t s = 0; s < _count; s += 2) {
			const __m128i a = _mm_set1_epi64x(_value);
			_mm_storeu_si128((__m128i*)(_ar + s), a);
		}
		return _Set(_ar + _count, _value, ss);
#else 
		return _Set(_ar, _value, _count);
#endif
	}
	static unsigned long long* Set(unsigned long long* _ar, const unsigned long long& _value, size_t _count) {
		return (unsigned long long*)Set((long long*)_ar, (long long)_value, _count);
	}
	static unsigned int* Set(unsigned int* _ar, const unsigned int& _value, size_t _count) {
		return (unsigned int*)Set((int*)_ar, (int)_value, _count);
	}
	static unsigned short* Set(unsigned short* _ar, const unsigned short&  _value, size_t _count) {
		return (unsigned short*)Set((short*)_ar, (short)_value, _count);
	}
	static unsigned char* Set(unsigned char* _ar, const unsigned char& _value, size_t _count) {
		return (unsigned char*)Set((char*)_ar, (char)_value, _count);
	}

	template <typename T> static T* Copy(T* _dest, size_t _destCount, const T* _src, size_t _count) {
#ifdef _DEBUG
		if(!_dest)throw MemoryError(MemoryError::Code::NullDest);
		if(!_src)throw MemoryError(MemoryError::Code::NullSrc);
		if(_count==0)throw MemoryError(MemoryError::Code::ZeroSize);
		if(_destCount==0)throw MemoryError(MemoryError::Code::ZeroDestSize);
#endif
		memcpy_s(_dest, _destCount * sizeof(T),_src, _count*sizeof(T));
		return _dest;
	}
	template <typename T> static T* CopyInThis(T* _dest, size_t _destCount, const T* _src, size_t _count) {
#ifdef _DEBUG
		if(!_dest)throw MemoryError(MemoryError::Code::NullDest);
		if(!_src)throw MemoryError(MemoryError::Code::NullSrc);
		if(_count==0)throw MemoryError(MemoryError::Code::ZeroSize);
		if(_destCount==0)throw MemoryError(MemoryError::Code::ZeroDestSize);
#endif
		memmove_s(_dest, _destCount * sizeof(T), _src, _count * sizeof(T));
		return _dest;
	}
};