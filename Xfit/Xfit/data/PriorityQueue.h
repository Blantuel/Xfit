#pragma once

#include "Memory.h"

template <typename T, class CompareFunc> 
class PriorityQueue {
protected:
	T * data;
	size_t len;
	T *data2;
	size_t maxLen;

	T & _EnqueueSwap() {
		size_t lenT = len;
		while (lenT > 1) {
			if (!CompareFunc(data2[lenT], data2[lenT / 2])) break;
			swap(data2[lenT], data2[lenT / 2]);
			lenT /= 2;
		}
		return data2[lenT];
	}
public:
	PriorityQueue<T, CompareFunc>& Clear() { len = 0; return *this; }

	const T* GetData()const { return data; }
	T* GetData() { return data; }
	T& operator [](size_t _index) { return data[_index]; }
	const T& operator [](size_t _index)const { return data[_index]; }

	const T* begin()const { return data; }
	T* begin() { return data; }
	const T* end()const { return data + len - 1; }
	T* end() { return data + len - 1; }

	size_t Size()const { return len; }

	bool operator==(const PriorityQueue<T, CompareFunc>& _other) {
		if (_other.len != len)return false;
		return memcmp(data, _other.data, len * sizeof(T)) == 0;
	}
	static void Set(const T& _value) { Memory::Set(data, _value, len); }
	static size_t Search(const T& _value) { return Memory::Search(data, _value, len); }

	bool operator!=(const PriorityQueue<T, CompareFunc>& _other) { return !(*this == _other); }
	void Alloc(size_t _maxLen) {
#ifdef _DEBUG
		if (data);
#endif
		maxLen = _maxLen;
		len = 0;
		data2 = new T[maxLen];
		data = data2 + 1;
	}
	PriorityQueue():data2(nullptr) {}
	PriorityQueue(size_t _maxLen) { Alloc(_maxLen); }
	PriorityQueue(const PriorityQueue<T, CompareFunc> &_other) :maxLen(_other.maxLen) {
		len = _other.len;
		data2 = new T[maxLen];
		data = data2 + 1;
		memcpy(data, _other.data, sizeof(T)*len);
	}
	PriorityQueue<T, CompareFunc>& operator =(const PriorityQueue<T, CompareFunc> &_other) {
		if (*this == _other)return;
		len = _other.len;
#ifdef _DEBUG
		if (maxLen < len + 1);
#endif
		memcpy(data, _other.data, sizeof(T)*len);
		return *this;
	}
	PriorityQueue(PriorityQueue<T, CompareFunc> &&_other) :maxLen(_other.maxLen),data2(_other.data2) {
		data = _other.data;
		len = _other.len;
		_other.data = nullptr;
		_other.data2 = nullptr; 
	}
	PriorityQueue<T, CompareFunc>& operator=(PriorityQueue<T, CompareFunc> &&_other) {
		if (*this == _other)return;
		if(data2)delete[]data2;
		data = _other.data;
		data2 = _other.data2;
		len = _other.len;
		maxLen = _other.maxLen;
		_other.data = nullptr;
		_other.data2 = nullptr;
		return *this;
	}

	void Free() {
#ifdef _DEBUG
		if (!data);
#endif
		delete[]data2; 
		data = nullptr;
		data2 = nullptr;
	}
	~PriorityQueue() { delete[]data2; }

	T & Enqueue(const T& _value) {
		len++;
#ifdef _DEBUG
		if (maxLen < len + 1);
#endif
		data2[len] = _value;
		return _EnqueueSwap();
	}
	T & Enqueue(T&& _value) {
		len++;
#ifdef _DEBUG
		if (maxLen < len + 1);
#endif
		data2[len] = move(_value);
		return _EnqueueSwap();
	}
	T & Dequeue() {
#ifdef _DEBUG
		if (len == 0);
#endif
		const T returnData = data2[1];
		data2[1] = data2[len];
		len--;
		size_t lenT = 1;
		while (true) {
			const size_t leftChild = lenT * 2;
			if (leftChild > len)break;
			const bool b = CompareFunc(data2[lenT], data2[leftChild]);
			const size_t rightChild = leftChild + 1;
			if (rightChild > len) {
				if (b) {
					swap(data2[lenT], data2[leftChild]);
					lenT = leftChild;
				} else break;
			} else {
				const bool b2 = CompareFunc(data2[lenT], data2[rightChild]);
				if (b) {
					if (b2) {
						if (CompareFunc(data2[leftChild], data2[rightChild])) {
							swap(data2[lenT], data2[leftChild]);
							lenT = leftChild;
						} else {
							swap(data2[lenT], data2[rightChild]);
							lenT = rightChild;
						}
					} else {
						swap(data2[lenT], data2[leftChild]);
						lenT = leftChild;
					}
				} else if (b2) {
					swap(data2[lenT], data2[rightChild]);
					lenT = rightChild;
				} else break;
			}
		}
		return returnData;
	}
};