#pragma once

#include "../stdafx.h"
#include "../system/Error.h"

class FileError :public Error{
public:
	enum class Code {
		CopyFailed,
		CopyCreateFailed,
		NotOpened,
		OverflowIndex,
		MinusIndex,
		AccessDenined,
		NotFound,
		SharingViolation,
		AlreadyExists,
		NotEnoughMemory,
		InvalidBuffer,
		Other
	};
	FileError(FileError::Code _code) :Error((int)_code) {}
};
class File {
	HANDLE hFile;
	bool isCreateNew, isReadOnly;
	Tstring path;

	static FileError::Code _ConvertErrorCode(unsigned _code) {
		switch (_code) {
		case ERROR_ACCESS_DENIED:return FileError::Code::AccessDenined;
		case ERROR_FILE_NOT_FOUND:return FileError::Code::NotFound;
		case ERROR_SHARING_VIOLATION:return FileError::Code::SharingViolation;
		case ERROR_ALREADY_EXISTS:return FileError::Code::AlreadyExists;
		case ERROR_NOT_ENOUGH_MEMORY:return FileError::Code::NotEnoughMemory;
		case ERROR_INVALID_USER_BUFFER:return FileError::Code::InvalidBuffer;
		}
		return FileError::Code::Other;
	}

	File(const File &_other) {}
	File& operator=(const File &_other) {}
public:
	enum class MoveOption {
		Begin = FILE_BEGIN,
		Current = FILE_CURRENT,
		End = FILE_END
	};
	bool IsOpen()const { return hFile != INVALID_HANDLE_VALUE; }
	File() :hFile(INVALID_HANDLE_VALUE){}
	void Load(const Tchar* _path, bool _createNew=false, bool _isReadOnly=false) {
		isCreateNew = _createNew;  isReadOnly = _isReadOnly;
		if (hFile != INVALID_HANDLE_VALUE) Close();

		
		hFile = CreateFile(_path, isReadOnly ?GENERIC_ALL:GENERIC_READ, isReadOnly ? FILE_SHARE_READ:0, nullptr, 
			isCreateNew ? CREATE_ALWAYS : OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
		if (!IsOpen()) throw FileError(_ConvertErrorCode(GetLastError()));
		path = _path;
	}
	File(const Tchar* _path, bool _createNew = false,  bool _isReadOnly = false) :isCreateNew(_createNew), isReadOnly(_isReadOnly) {
		hFile = CreateFile(_path, isReadOnly ? GENERIC_ALL : GENERIC_READ, isReadOnly ? FILE_SHARE_READ : 0, nullptr,
			isCreateNew ? CREATE_ALWAYS : OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL, nullptr);
		if (IsOpen())path = _path;
		else throw FileError(_ConvertErrorCode(GetLastError()));
	}
	File(File &&_other) {
		if (!_other.IsOpen()) { hFile = INVALID_HANDLE_VALUE; return; }
		isCreateNew = _other.isCreateNew;
		isReadOnly = _other.isReadOnly;
		path = _other.path;
		_other.hFile = INVALID_HANDLE_VALUE;
	}
	
	File& operator=(File &&_other) {
		if (!_other.IsOpen()) { hFile = INVALID_HANDLE_VALUE; return*this; }
		isCreateNew = _other.isCreateNew;
		isReadOnly = _other.isReadOnly;
		path = _other.path;
		_other.hFile = INVALID_HANDLE_VALUE;
		return *this;
	}
	template<typename T> pair<unsigned,bool> WriteBytes(unsigned size, const T* buffer) {
#ifdef _DEBUG
		if (!IsOpen()) throw FileError(FileError::Code::NotOpened);
#endif
		DWORD red;
		if (!WriteFile(hFile, buffer, size, &red, nullptr)) {
			const DWORD error = GetLastError();
			if (error == ERROR_HANDLE_EOF)return {red,true};
			throw FileError(_ConvertErrorCode(error));
		}
		return { red,false };
	}
	template<typename T> pair<unsigned, bool> ReadBytes(unsigned size, T* buffer) {
#ifdef _DEBUG
		if (!IsOpen()) throw FileError(FileError::Code::NotOpened);
#endif
		DWORD red;
		if (!ReadFile(hFile, buffer, size, &red, nullptr)) {
			const DWORD error = GetLastError();
			if (error == ERROR_HANDLE_EOF)return { red,true };
			throw FileError(_ConvertErrorCode(error));
		}
		return { red,false };
	}
	void SetPos(int _index = 0, MoveOption _moveType = MoveOption::Begin) {
#ifdef _DEBUG
		if (!IsOpen()) throw FileError(FileError::Code::NotOpened);
		int index = _index;
		if (_moveType == MoveOption::Current) index+=GetPos();
		else if (_moveType == MoveOption::End) index = GetSize()-1 - index;
		if (index > (GetSize() - 1))throw FileError( FileError::Code::OverflowIndex);
		 else if (index < 0) throw FileError(FileError::Code::MinusIndex);
#endif
		SetFilePointer(hFile, _index, nullptr, DWORD(_moveType));
	}
	const Tstring& GetPath()const { return path; }
	size_t GetSize()const {
#ifdef _DEBUG
		if (!IsOpen()) throw FileError(FileError::Code::NotOpened);
#endif
#ifdef XFIT_BIT64
		DWORD size2;
		const DWORD size = GetFileSize(hFile,&size2);
		return size_t(size2) << 32Ui64 | size_t(size);
#else
		return GetFileSize(hFile,nullptr);
#endif
	}
	static void Delete(const Tchar* path) {
		if (!DeleteFile(path)) throw FileError(_ConvertErrorCode(GetLastError()));
	}
	void Close() {
#ifdef _DEBUG
		if (!IsOpen()) throw FileError(FileError::Code::NotOpened);
#endif
		CloseHandle(hFile);
		hFile = INVALID_HANDLE_VALUE;
	}
	bool IsCreateNew()const { return isCreateNew; }
	size_t GetPos()const {
#ifdef _DEBUG
		if (!IsOpen()) throw FileError(FileError::Code::NotOpened);
#endif
#ifdef XFIT_BIT64
		LARGE_INTEGER l,l2;
		l.QuadPart = 0;
		SetFilePointerEx(hFile, l, &l2, DWORD(MoveOption::Current));
		return l2.QuadPart;
#else
		return SetFilePointer(hFile, 0, nullptr, DWORD(MoveOption::Current));
#endif
	}
	template<typename T> pair<unsigned, bool> Read(T* _data)const {return ReadBytes(sizeof(T) , _data);}
	template <typename T> pair<unsigned, bool> Write(const T& _data) { return WriteBytes(sizeof(T), &_data); }
	~File() { if (IsOpen())Close(); }
};