#pragma once

#include "../system/Error.h"


namespace Sort {
	//Max Stack Size 2n + 2
	template <typename T, typename Length, typename Func, typename Func2>
	inline void QuickSortFunc(T *_datas, Length _length, Length* _stack, Func _lessCompareFunc, Func2 _moreCompareFunc) {
#ifdef _DEBUG
			if (_length < 2);
#endif
		_stack[0] = _length - 1;
		_stack[1] = 0;
		Length _length2 = 2, left, right;
		while (_length2 > 0) {
			const Length start = _stack[_length2 - 1];
			const Length end = _stack[_length2 - 2];
			_length2 -= 2;
			const T& key = _datas[end];
			left = start;
			right = end - 1;
			while (true) {
				while (_lessCompareFunc(_datas[left], key))left++;
				while ((right > 0) && _moreCompareFunc(_datas[right], key))right--;
				if (left >= right)break;
				swap(_datas[left], _datas[right]);
				left++;
				right--;
			}
			swap(_datas[left], _datas[end]);
			if (end - left > 1) {
				_stack[_length2] = end;
				_stack[_length2 + 1] = left + 1;
				_length2 += 2;
			}
			if (left - start > 1) {
				_stack[_length2] = left - 1;
				_stack[_length2 + 1] = start;
				_length2 += 2;
			}
		}
	}
	//Max Stack Size 2n + 2
	template <typename T, typename Length> inline void QuickSort(T *_datas, Length _length, Length* _stack) { QuickSortFunc(_datas, _length, _stack, less, greater); }
	//Max Stack Size 2n + 2
	template <typename T,typename T2,typename Length> inline void QuickSortProperty(T *_datas, Length _length, Length* _stack, T2 T::*_prop) {
		QuickSortFunc(_datas, _length, _stack, [](const T& data, const T& data2)->bool {return data.*_prop < data2.*_prop; }, [](const T& data, const T& data2)->bool {return data.*_prop > data2.*_prop; });
	}
	//Max Stack Size 2n + 2
	template <typename T, typename T2, typename Length> inline void QuickSortPropertyPointer(T **_datas, Length _length, Length _stack, T2 T::*_prop) {
		QuickSortFunc(_datas, _length, _stack, [](const T* data, const T* data2)->bool {return data->*_prop < data2->*_prop; }, [](const T* data, const T* data2)->bool {return data->*_prop > data2->*_prop; });
	}
};
