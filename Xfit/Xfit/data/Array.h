#pragma once

#include "Memory.h"

template <typename T> class Array  {
protected:
	T * data;
	size_t len;
	size_t maxLen;
public:
	Array<T>& Clear() { len = 0; return *this; }

	const T* GetData()const { return data; }
	T* GetData() { return data; }
	T& operator [](size_t _index) { return data[_index]; }
	const T& operator [](size_t _index)const { return data[_index]; }

	const T* begin()const { return data; }
	T* begin() { return data; }
	const T* end()const { return data + len - 1; }
	T* end() { return data + len - 1; }

	size_t Size()const { return len; }
	size_t MaxSize()const { return maxLen; }

	bool operator==(const Array<T>& _other) {
		if (_other.len != len)return false;
		return memcmp(data, _other.data, len * sizeof(T)) == 0;
	}
	Array() {}
	bool operator!=(const Array<T>& _other) { return !(*this == _other); }
	void Alloc(size_t _maxLen) {
		maxLen = _maxLen;
		data = new T[maxLen];
		len = 0;
	}
	void SetData(T* _data,size_t _len,size_t _maxLen) {
#ifdef _DEBUG
		if (!data);
		if (_len == 0);
		if (_maxLen == 0);
		if (_len>_maxLen);
#endif
		data = _data;
		len = _len;
		maxLen = _maxLen;
	}
	void Free() { 
#ifdef _DEBUG
		if (!data);
#endif
		delete[]data;
	}
	
	void Resize(size_t _len) {
#ifdef _DEBUG
		if (maxLen < _len);
#endif
		len = _len;
	}
	Array(const Array<T> &_other):maxLen(_other.maxLen) {
		len = _other.len;
		data = new T[maxLen];
		memcpy_s(data, sizeof(T)*len,_other.data, sizeof(T)*len);
	}
	Array(Array<T> &&_other) : maxLen(_other.maxLen) {
		data = _other.data;
		len = _other.len;
		_other.data = nullptr;
	}
	Array<T>& operator =(const Array<T> &_other) {
		if (&_other == this)return *this;
		Resize(_other.len);
		memcpy_s(data, sizeof(T)*len,_other.data, sizeof(T)*len);
		return *this;
	}
	Array<T>& operator=(Array<T> &&_other) {
		if(&_other == this)return *this;
		if (data)delete[]data;
		data = _other.data;
		len = _other.len;
		maxLen = _other.maxLen;
		_other.data = nullptr;
		return *this;
	}
	T& InsertLast(const T& _value) {
		Resize(len + 1);
		return data[len - 1] = _value;
	}
	T& InsertLast(T&& _value){
		Resize(len+1);
		return data[len - 1] = move(_value);
	}
	Array<T>& InsertArLast(const T* _ar, size_t _len) {
		Resize(len + _len);
		memmove_s(data + len - _len, _len * sizeof(T), _ar, _len * sizeof(T));
		return *this;
	}
	Array<T>& InsertArLast(const Array<T>& _ar) {return InsertArLast(_ar.GetData(), _ar.Size());}
	Array<T>& InsertInitializerLast(const initializer_list<T> _init) {
		Resize(len + _init.size());
		memmove_s(data + len - _init.size(), _init.size() * sizeof(T), _init.begin(), _init.size()*sizeof(T));
		return *this;
	}
	Array<T>& InsertInitializer(const initializer_list<T> _init, size_t _index) {
		Resize(len+ _init.size());
		memmove_s(data + _index + _init.size(), (len - _index - _init.size()) * sizeof(T), data + _index, (len - _index - _init.size())*sizeof(T));
		memmove_s(data + _index, (len - _index) * sizeof(T), _init.begin(), _init.size()*sizeof(T));
		return *this;
	}
	Array<T>& InsertAr(const T* _ar, size_t _len, size_t _index) {
		Resize(len+ _len);
		memmove_s(data + _index + _len, (len - _index - _len) * sizeof(T), data + _index,(len - _index - _len)*sizeof(T));
		memmove_s(data + _index, (len - _index) * sizeof(T), _ar, _len*sizeof(T));
		return *this;
	}
	Array<T>& InsertAr(const Array<T>& _ar, size_t _index) { return InsertAr(_ar.GetData(), _ar.Size(), _index); }
	Array<T>& InsertInitializerFirst(const initializer_list<T> _init) {
		Resize(len + _init.size());
		memmove_s(data + _init.size(), (len - _init.size()) * sizeof(T), data, (len - _init.size())*sizeof(T));
		memmove_s(data, len * sizeof(T),_init.begin(), _init.size()*sizeof(T));
		return *this;
	}
	Array<T>& InsertArFirst(const T* _ar, size_t _len) {
		Resize(len + _len);
		memmove_s(data + _len, (len - _len) * sizeof(T), data, (len - _len)*sizeof(T));
		memmove_s(data, len * sizeof(T), _ar, _len*sizeof(T));
		return *this;
	}
	Array<T>& InsertArFirst(const Array<T>& _ar) {return InsertArFirst(_ar.GetData(),_ar.Size());}
	Array<T>& SetAr(const T* _value, size_t _len, size_t _index) {
		if (_index + _len>len) Resize(_index + _len);
		memmove_s(data + _index, (len-_index) * sizeof(T),_value, _len*sizeof(T));
		return *this;
	}
	T& InsertFirst(const T& _value) {
		Resize(len + 1);
		memmove_s(data + 1, (len - 1) * sizeof(T), data, (len - 1) * sizeof(T));
		return data[0] = _value;
	}
	T& InsertFirst(T&& _value) {
		Resize(len + 1);
		memmove_s(data + 1, (len-1) * sizeof(T),data, (len-1)*sizeof(T));
		return data[0] = move(_value);
	}
	Array<T>& Insert(T&& _value, size_t _index) {
		if (_index >= len)Resize(_index + 1);
		else Resize(len + 1);
		memmove_s(data + _index + 1, (len - _index-1) * sizeof(T), data + _index, (len - _index-1)*sizeof(T));
		data[_index] = move(_value);
		return *this;
	}
	Array<T>& Insert(const T& _value, size_t _index) {
		if (_index >= len)Resize(_index + 1);
		else Resize(len + 1);
		memmove_s(data + _index + 1, (len - _index-1) * sizeof(T),data + _index, (len - _index-1)*sizeof(T));
		data[_index] = _value;
		return *this;
	}
	Array<T>& operator+=(T&& _data) { return InsertLast(_data); }
	Array<T>& operator+(T&& _data) const { return Array<T>(*this).InsertLast(_data); }
	Array<T>& operator+=(const T& _data) { return InsertLast(_data); }
	Array<T>& operator+(const T& _data) const { return Array<T>(*this).InsertLast(_data); }
	Array<T>& EraseFirst() {
		memmove_s(data, len * sizeof(T), data + 1, (len - 1) * sizeof(T));
		len--;
		return *this;
	}
	T& EraseLast() { len -= 1; return data[len]; }
	Array<T>& EraseIndex(size_t _index) {
		memmove_s(data + _index, (len - _index) * sizeof(T),data + _index + 1, (len - _index - 1) * sizeof(T));
		len--;
		return *this;
	}
	Array<T>& EraseThisAr(size_t _index, size_t _len) {
		memmove_s(data + _index, (len - _index) * sizeof(T),data + _index + _len, (len - _index - _len) * sizeof(T));
		len -= _len;
		return *this;
	}
	void Set(const T& _value) {Memory::Set(data, _value, len);}
	size_t Search(const T& _value) {return Memory::Search(data, _value, len);}
};