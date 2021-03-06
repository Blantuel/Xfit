#pragma once

#include "Memory.h"
#include "../system/Error.h"

class ArrayError : public Error {
public:
	enum class Code {
		ZeroSize,
		ZeroMaxSize,
		TooBigSize,
		NullData,
		AlreadyAlloc,
		NotAlloc,
		NullInput,
		OutIndex,
		TooSmallNewSize
	};
protected:
	Code code;
public:
	Code GetCode()const{return code;}
	ArrayError(ArrayError::Code _code) :code(_code) {}
};
template <typename T> class Array {
protected:
	T * data;
	size_t len;
	size_t maxLen;
	bool isAlloc;
public:
	Array<T>& Clear() { 
#ifdef _DEBUG
		if(!data)throw ArrayError(ArrayError::Code::NullData);
#endif
		len = 0; return *this; 
	}
	bool IsAlloc()const {return isAlloc;}
	const T* GetData()const { return data; }
	T* GetData() { return data; }
	T& operator [](size_t _index) {
#ifdef _DEBUG
		if(!data)throw ArrayError(ArrayError::Code::NullData);
#endif
		return data[_index];
	}
	const T& operator [](size_t _index)const {
#ifdef _DEBUG
		if(!data)throw ArrayError(ArrayError::Code::NullData);
#endif
		return data[_index];
	}

	const T* begin()const {
#ifdef _DEBUG
		if(!data)throw ArrayError(ArrayError::Code::NullData);
#endif
		return data;
	}
	T* begin() {
#ifdef _DEBUG
		if(!data)throw ArrayError(ArrayError::Code::NullData);
#endif
		return data;
	}
	const T* end()const {
#ifdef _DEBUG
		if(!data)throw ArrayError(ArrayError::Code::NullData);
#endif
		return data + len;
	}
	T* end() {
#ifdef _DEBUG
		if(!data)throw ArrayError(ArrayError::Code::NullData);
#endif
		return data + len;
	}

	size_t Size()const {
#ifdef _DEBUG
		if(!data)throw ArrayError(ArrayError::Code::NullData);
#endif
		return len; 
	}
	size_t MaxSize()const {
#ifdef _DEBUG
		if(!data)throw ArrayError(ArrayError::Code::NullData);
#endif
		return maxLen; 
	}
	bool operator==(const Array<T>& _other) {
		if (_other.Size() != Size())return false;
		if(Size()==0) return _other.Size()==0;
		else if(_other.Size()==0) return false;
		return memcmp(data, _other.data, Size() * sizeof(T)) == 0;
	}
	Array():data(nullptr),isAlloc(false) {}
	~Array() {	if(isAlloc)delete[]data;}
	bool operator!=(const Array<T>& _other) { return !(*this == _other); }
	void Alloc(size_t _maxLen) {
#ifdef _DEBUG
		if(isAlloc)throw ArrayError(ArrayError::Code::AlreadyAlloc);
		if (_maxLen == 0)throw ArrayError(ArrayError::Code::ZeroMaxSize);
#endif
		maxLen = _maxLen;
		data = new T[maxLen];
		len = 0;
		isAlloc=true;
	}
	Array(size_t _maxLen):isAlloc(false){Alloc(_maxLen);}
	void SetData(T* _data,size_t _len,size_t _maxLen) {
#ifdef _DEBUG
		if (_maxLen == 0)throw ArrayError(ArrayError::Code::ZeroMaxSize);
		if (_len>_maxLen)throw ArrayError(ArrayError::Code::TooBigSize);
#endif
		if(isAlloc) {
			delete[]data;
			isAlloc=false;
		}
		data = _data;
		len = _len;
		maxLen = _maxLen;
	}
	Array(T* _data,size_t _len,size_t _maxLen){
		isAlloc=false;
		data = _data;
		len = _len;
		maxLen = _maxLen;
	}
	void ReAlloc(size_t _newSize) {
#ifdef _DEBUG
		if(_newSize < maxLen)throw ArrayError(ArrayError::Code::TooSmallNewSize);
#endif
		maxLen = _newSize;
		T* tempData = new T[maxLen];
		Memory::Copy(tempData, maxLen, data, len);
		if (isAlloc) delete[]data;
		else isAlloc = true;
		data = tempData;
	}
	void Free() { 
#ifdef _DEBUG
		if(!isAlloc)throw ArrayError(ArrayError::Code::NotAlloc);
		if(!data)throw ArrayError(ArrayError::Code::NullData);
#endif
		delete[]data;
		data=nullptr;
		isAlloc=false;
	}
	
	void Resize(size_t _len) {
#ifdef _DEBUG
		if(!data)throw ArrayError(ArrayError::Code::NullData);
		if (maxLen < _len)throw ArrayError(ArrayError::Code::TooBigSize);
#endif
		len = _len;
	}
	Array(const Array<T> &_other):len(_other.len),maxLen(_other.maxLen) {
		if(_other.data) {
			data = new T[maxLen];
#ifdef _WIN32
			memcpy_s(data, sizeof(T)*len,_other.data, sizeof(T)*len);
#else
			memcpy(data, _other.data, sizeof(T)*len);
#endif
			isAlloc=true;
		} else {
			data=nullptr;
			isAlloc=false;
		}
	}
	Array(Array<T> &&_other) : maxLen(_other.maxLen),data(_other.data),len(_other.len),isAlloc(_other.isAlloc) {_other.data = nullptr;}
	Array<T>& operator =(const Array<T> &_other) {
		if (&_other == this)return *this;
		if(_other.data) {
#ifdef _DEBUG
			if(data) {
#endif
				Resize(_other.len);
#ifdef _WIN32
				memcpy_s(data, sizeof(T)*len,_other.data, sizeof(T)*len);
#else
				memcpy(data, _other.data, sizeof(T)*len);
#endif
#ifdef _DEBUG
			} else throw ArrayError(ArrayError::Code::NullData);
#endif
		}else{
			if(isAlloc) {
				delete[]data;
				isAlloc=false;
			}
			data=nullptr;
		}
		return *this;
	}
	Array<T>& operator=(Array<T> &&_other) {
		if(&_other == this)return *this;
		if (isAlloc)delete[]data;
		data = _other.data;
		len = _other.len;
		maxLen = _other.maxLen;
		isAlloc = _other.isAlloc;
		_other.data = nullptr;
		return *this;
	}
	T& InsertLast(const T& _value) {
#ifdef _DEBUG
		if(!data)throw ArrayError(ArrayError::Code::NullData);
#endif
		Resize(len + 1);
		return data[len - 1] = _value;
	}
	T& InsertLast(T&& _value){
#ifdef _DEBUG
		if(!data)throw ArrayError(ArrayError::Code::NullData);
#endif
		Resize(len+1);
		return data[len - 1] = std::move(_value);
	}
	template<class... _Valty>
	T& EmplaceLast(_Valty&&... _Val) {
#ifdef _DEBUG
		if (!data)throw ArrayError(ArrayError::Code::NullData);
#endif
		Resize(len + 1);
		return data[len - 1] = T(std::forward<_Valty>(_Val)...);
	}
	template<class... _Valty>
	T& EmplaceFirst(_Valty&&... _Val) {
#ifdef _DEBUG
		if (!data)throw ArrayError(ArrayError::Code::NullData);
#endif
		Resize(len + 1);
#ifdef _WIN32
		memmove_s(data + 1, (len - 1) * sizeof(T), data, (len - 1) * sizeof(T));
#else
		memmove(data + 1, data, (len - 1) * sizeof(T));
#endif

		return data[0] = T(std::forward<_Valty>(_Val)...);
	}
	Array<T>& InsertArLast(const T* _ar, size_t _len) {
#ifdef _DEBUG
		if(!data)throw ArrayError(ArrayError::Code::NullData);
		if(!_ar)throw ArrayError(ArrayError::Code::NullInput);
		if(_len==0)throw ArrayError(ArrayError::Code::ZeroSize);
#endif
		Resize(len + _len);
#ifdef _WIN32
		memmove_s(data + len - _len, _len * sizeof(T), _ar, _len * sizeof(T));
#else
		memmove(data + len - _len, _ar, _len * sizeof(T));
#endif

		return *this;
	}
	Array<T>& InsertArLast(const Array<T>& _ar) {return InsertArLast(_ar.GetData(), _ar.Size());}
	Array<T>& InsertAr(const T* _ar, size_t _len, size_t _index) {
#ifdef _DEBUG
		if(!data)throw ArrayError(ArrayError::Code::NullData);
		if(!_ar)throw ArrayError(ArrayError::Code::NullInput);
		if(_len==0)throw ArrayError(ArrayError::Code::ZeroSize);
		if(_index>len)throw ArrayError(ArrayError::Code::OutIndex);
#endif
		Resize(len+ _len);
		const size_t lenT = len - _index - _len;

#ifdef _WIN32
		if(lenT > 0)memmove_s(data + _index + _len, lenT * sizeof(T), data + _index,lenT*sizeof(T));
		memmove_s(data + _index, (len - _index) * sizeof(T), _ar, _len*sizeof(T));
#else
		if(lenT > 0)memmove(data + _index + _len, data + _index,lenT*sizeof(T));
		memmove(data + _index, _ar, _len*sizeof(T));
#endif

		return *this;
	}
	Array<T>& InsertAr(const Array<T>& _ar, size_t _index) { return InsertAr(_ar.GetData(), _ar.Size(), _index); }
	Array<T>& InsertArFirst(const T* _ar, size_t _len) {
#ifdef _DEBUG
		if(!data)throw ArrayError(ArrayError::Code::NullData);
		if(!_ar)throw ArrayError(ArrayError::Code::NullInput);
		if(_len==0)throw ArrayError(ArrayError::Code::ZeroSize);
#endif
		Resize(len + _len);

#ifdef _WIN32
		memmove_s(data + _len, (len - _len) * sizeof(T), data, (len - _len)*sizeof(T));
		memmove_s(data, len * sizeof(T), _ar, _len*sizeof(T));
#else
		memmove(data + _len, data, (len - _len)*sizeof(T));
		memmove(data, _ar, _len*sizeof(T));
#endif

		return *this;
	}
	Array<T>& InsertArFirst(const Array<T>& _ar) {return InsertArFirst(_ar.GetData(),_ar.Size());}
	Array<T>& SetAr(const T* _ar, size_t _len, size_t _index) {
#ifdef _DEBUG
		if(!data)throw ArrayError(ArrayError::Code::NullData);
		if(!_ar)throw ArrayError(ArrayError::Code::NullInput);
		if(_len==0)throw ArrayError(ArrayError::Code::ZeroSize);
		if(_index>len)throw ArrayError(ArrayError::Code::OutIndex);
#endif
		if (_index + _len>len) Resize(_index + _len);

#ifdef _WIN32
		memmove_s(data + _index, (len - _index) * sizeof(T), _ar, _len * sizeof(T));
#else
		memmove(data + _index, _ar, _len * sizeof(T));
#endif
		return *this;
	}
	T& InsertFirst(const T& _value) {
#ifdef _DEBUG
		if(!data)throw ArrayError(ArrayError::Code::NullData);
#endif
		Resize(len + 1);

#ifdef _WIN32
		memmove_s(data + 1, (len - 1) * sizeof(T), data, (len - 1) * sizeof(T));
#else
		memmove(data + 1, data, (len - 1) * sizeof(T));
#endif

		return data[0] = _value;
	}
	T& InsertFirst(T&& _value) {
#ifdef _DEBUG
		if(!data)throw ArrayError(ArrayError::Code::NullData);
#endif
		Resize(len + 1);

#ifdef _WIN32
		memmove_s(data + 1, (len-1) * sizeof(T),data, (len-1)*sizeof(T));
#else
		memmove(data + 1, data, (len-1)*sizeof(T));
#endif

		return data[0] = std::move(_value);
	}
	Array<T>& Insert(T&& _value, size_t _index) {
#ifdef _DEBUG
		if(!data)throw ArrayError(ArrayError::Code::NullData);
		if(_index>len)throw ArrayError(ArrayError::Code::OutIndex);
#endif
		if (_index >= len)Resize(_index + 1);
		else Resize(len + 1);

#ifdef _WIN32
		memmove_s(data + _index + 1, (len - _index-1) * sizeof(T), data + _index, (len - _index-1)*sizeof(T));
#else
		memmove(data + _index + 1, data + _index, (len - _index-1)*sizeof(T));
#endif

		data[_index] = std::move(_value);
		return *this;
	}
	Array<T>& Insert(const T& _value, size_t _index) {
#ifdef _DEBUG
		if(!data)throw ArrayError(ArrayError::Code::NullData);
		if(_index>len)throw ArrayError(ArrayError::Code::OutIndex);
#endif
		if (_index >= len)Resize(_index + 1);
		else Resize(len + 1);
#ifdef _WIN32
		memmove_s(data + _index + 1, (len - _index-1) * sizeof(T),data + _index, (len - _index-1)*sizeof(T));
#else
		memmove(data + _index + 1, data + _index, (len - _index-1)*sizeof(T));
#endif
		data[_index] = _value;
		return *this;
	}
	Array<T>& InsertInitializerLast(const std::initializer_list<T> _init) {
		Resize(len + _init.size());
#ifdef _WIN32
		memmove_s(data + len - _init.size(), _init.size() * sizeof(T), _init.begin(), _init.size() * sizeof(T));
#else
		memmove(data + len - _init.size(), _init.begin(), _init.size() * sizeof(T));
#endif
		return *this;
	}

	Array<T>& InsertInitializer(const std::initializer_list<T> _init, size_t _index) {
		Resize(len + _init.size());
#ifdef _WIN32
		memmove_s(data + _index + _init.size(), (len - _index - _init.size()) * sizeof(T), data + _index, (len - _index - _init.size()) * sizeof(T));
		memmove_s(data + _index, (len - _index) * sizeof(T), _init.begin(), _init.size() * sizeof(T));
#else
		memmove(data + _index + _init.size(), data + _index, (len - _index - _init.size()) * sizeof(T));
		memmove(data + _index, _init.begin(), _init.size() * sizeof(T));
#endif
		return *this;
	}
	Array<T>& InsertInitializerFirst(const std::initializer_list<T> _init) {
		Resize(len + _init.size());
#ifdef _WIN32
		memmove_s(data + _init.size(), (len - _init.size()) * sizeof(T), data, (len - _init.size()) * sizeof(T));
		memmove_s(data, len * sizeof(T), _init.begin(), _init.size() * sizeof(T));
#else
		memmove(data + _init.size(), data, (len - _init.size()) * sizeof(T));
		memmove(data, _init.begin(), _init.size() * sizeof(T));
#endif
		return *this;
	}
	Array<T>& operator+=(T&& _data) { return InsertLast(_data); }
	Array<T>& operator+(T&& _data) const { return Array<T>(*this).InsertLast(_data); }
	Array<T>& operator+=(const T& _data) { return InsertLast(_data); }
	Array<T>& operator+(const T& _data) const { return Array<T>(*this).InsertLast(_data); }
	Array<T>& EraseFirst() {
#ifdef _DEBUG
		if(!data)throw ArrayError(ArrayError::Code::NullData);
		if(len==0)throw ArrayError(ArrayError::Code::ZeroSize);
#endif
#ifdef _WIN32
		memmove_s(data, len * sizeof(T), data + 1, (len - 1) * sizeof(T));
#else
		memmove(data, data + 1, (len - 1) * sizeof(T));
#endif
		len--;
		return *this;
	}
	T& EraseLast() { 
#ifdef _DEBUG
		if(!data)throw ArrayError(ArrayError::Code::NullData);
		if(len==0)throw ArrayError(ArrayError::Code::ZeroSize);
#endif
		len -= 1; 
		return data[len];
	}
	Array<T>& EraseIndex(size_t _index) {
#ifdef _DEBUG
		if(!data)throw ArrayError(ArrayError::Code::NullData);
		if(len==0)throw ArrayError(ArrayError::Code::ZeroSize);
		if(_index>(len-1))throw ArrayError(ArrayError::Code::OutIndex);
#endif
#ifdef _WIN32
		memmove_s(data + _index, (len - _index) * sizeof(T),data + _index + 1, (len - _index - 1) * sizeof(T));
#else
		memmove(data + _index, data + _index + 1, (len - _index - 1) * sizeof(T));
#endif

		len--;
		return *this;
	}
	Array<T>& EraseThisAr(size_t _index, size_t _len) {
#ifdef _DEBUG
		if(!data)throw ArrayError(ArrayError::Code::NullData);
		if(len==0 || _len==0)throw ArrayError(ArrayError::Code::ZeroSize);
		if((_index+_len)>len)throw ArrayError(ArrayError::Code::OutIndex);
#endif
#ifdef _WIN32
		memmove_s(data + _index, (len - _index) * sizeof(T),data + _index + _len, (len - _index - _len) * sizeof(T));
#else
		memmove(data + _index, data + _index + _len, (len - _index - _len) * sizeof(T));
#endif
		len -= _len;
		return *this;
	}
	void Set(const T& _value) {
#ifdef _DEBUG
		if(!data)throw ArrayError(ArrayError::Code::NullData);
		if(len==0)throw ArrayError(ArrayError::Code::ZeroSize);
#endif
		Memory::Set(data, _value, len);
	}
	size_t Search(const T& _value) {
#ifdef _DEBUG
		if(!data)throw ArrayError(ArrayError::Code::NullData);
		if(len==0)throw ArrayError(ArrayError::Code::ZeroSize);
#endif
		return Memory::Search(data, _value, len);
	}
};