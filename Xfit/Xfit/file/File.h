#pragma once

#include "../system/Error.h"

#ifdef _WIN32

class FileError :public Error {
public:
	enum class Code {
		AccessDenined = ERROR_ACCESS_DENIED,
		NotFound = ERROR_FILE_NOT_FOUND,
		SharingViolation = ERROR_SHARING_VIOLATION,
		AlreadyExists = ERROR_ALREADY_EXISTS,
		NotEnoughMemory = ERROR_NOT_ENOUGH_MEMORY,
		InvalidBuffer = ERROR_INVALID_USER_BUFFER,
		CopyFailed,
		CopyCreateFailed,
		NotOpened,
		OverflowIndex,
		MinusIndex,
		InvalidOpenMode,
		AlreadyOpened
	};
protected:
	Code code;
public:
	Code GetCode()const { return code; }
	FileError(FileError::Code _code) :code(_code) {}
};
class File {
	HANDLE hFile;

	File(const File &_other) {}
	File& operator=(const File &_other) {}
public:
	enum class OpenMode {
		Read,
		ReadWrite,
		ReadWriteCreate
	};
	enum class MoveOption {
		Begin = FILE_BEGIN,
		Current = FILE_CURRENT,
		End = FILE_END
	};
	bool IsOpen()const { return hFile != INVALID_HANDLE_VALUE; }
	File() :hFile(INVALID_HANDLE_VALUE){}
	File(const char* _path, OpenMode _openMode = OpenMode::Read) {
#ifdef _DEBUG
		hFile = INVALID_HANDLE_VALUE;
#endif
		Open(_path, _openMode);
	}
	void Open(const char* _path, OpenMode _openMode = OpenMode::Read) {
#ifdef _DEBUG
		if (IsOpen()) throw FileError(FileError::Code::AlreadyOpened);
#endif

		switch (_openMode) {
		case OpenMode::Read:
			hFile = CreateFileA(_path, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
			break;
		case OpenMode::ReadWrite:
			hFile = CreateFileA(_path, GENERIC_ALL, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
			break;
		case OpenMode::ReadWriteCreate:
			hFile = CreateFileA(_path, GENERIC_ALL, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
			break;
		default:
			throw FileError(FileError::Code::InvalidOpenMode);
			break;
		}
		if (!IsOpen()) throw FileError((FileError::Code)GetLastError());
	}
	File(File &&_other) {
		hFile = _other.hFile;
		_other.hFile = INVALID_HANDLE_VALUE;
	}
	File& operator=(File &&_other) {
		hFile = _other.hFile;
		_other.hFile = INVALID_HANDLE_VALUE;
		return *this;
	}
	template<typename T> unsigned WriteBytes(unsigned size, const T* buffer) {
#ifdef _DEBUG
		if (!IsOpen()) throw FileError(FileError::Code::NotOpened);
#endif
		DWORD red;
		if (!WriteFile(hFile, buffer, size, &red, nullptr)) {
			const DWORD error = GetLastError();
			if (error == ERROR_HANDLE_EOF)return red;
			throw FileError((FileError::Code)error);
		}
		return red;
	}
	template<typename T> unsigned ReadBytes(unsigned size, T* buffer)const {
#ifdef _DEBUG
		if (!IsOpen()) throw FileError(FileError::Code::NotOpened);
#endif
		DWORD red;
		if (!ReadFile(hFile, buffer, size, &red, nullptr)) {
			const DWORD error = GetLastError();
			if (error == ERROR_HANDLE_EOF)return red;
			throw FileError((FileError::Code)error);
		}
		return red;
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
	unsigned GetSize()const {
#ifdef _DEBUG
		if (!IsOpen()) throw FileError(FileError::Code::NotOpened);
#endif
		return (unsigned)GetFileSize(hFile,nullptr);
	}
	static void Delete(const Tchar* path) {
		if (!DeleteFile(path)) throw FileError((FileError::Code)GetLastError());
	}
	void Close() {
#ifdef _DEBUG
		if (!IsOpen()) throw FileError(FileError::Code::NotOpened);
#endif
		CloseHandle(hFile);
		hFile = INVALID_HANDLE_VALUE;
	}
	size_t GetPos()const {
#ifdef _DEBUG
		if (!IsOpen()) throw FileError(FileError::Code::NotOpened);
#endif
		return SetFilePointer(hFile, 0, nullptr, DWORD(MoveOption::Current));
	}
	~File() { if (IsOpen())Close(); }
};

#elif __ANDROID__

class FileError :public Error {
public:
	enum class Code {
		AccessDenined = EACCES,
		NotFound = EDQUOT,
		//SharingViolation = ERROR_SHARING_VIOLATION,
		AlreadyExists = EEXIST,
		NotEnoughMemory = ENOMEM,
		//InvalidBuffer = ERROR_INVALID_USER_BUFFER,
		CopyFailed,
		CopyCreateFailed,
		NotOpened,
		OverflowIndex,
		MinusIndex,
		InvalidOpenMode,
		AlreadyOpened
	};
protected:
	Code code;
public:
	Code GetCode()const { return code; }
	FileError(FileError::Code _code) :code(_code) {}
};
class File {
	FILE* hFile;

	File(const File &_other) {}
	File& operator=(const File &_other) {}
public:
	enum class OpenMode {
		Read,
		ReadWrite,
		ReadWriteCreate
	};
	enum class MoveOption {
		Begin = SEEK_SET,
		Current = SEEK_CUR,
		End = SEEK_END
	};
	File() :hFile(nullptr){}
	File(const char* _path, OpenMode _openMode = OpenMode::Read) { Open(_path, _openMode); }

	bool IsOpen()const { return hFile != nullptr; }
	void Open(const char* _path, OpenMode _openMode = OpenMode::Read) {
#ifdef _DEBUG
		if (IsOpen());
#endif
		switch (_openMode) {
		case OpenMode::Read:hFile = fopen(_path, "rb"); break;
		case OpenMode::ReadWrite:hFile = fopen(_path, "r+b"); break;
		case OpenMode::ReadWriteCreate:hFile = fopen(_path, "w+b");break;
#ifdef _DEBUG
		default:break;
#endif
		}
		if (!hFile) throw FileError((FileError::Code)errno);
	}
	File(File &&_other) {
		hFile = _other.hFile;
		_other.hFile = nullptr;
	}
	
	File& operator=(File &&_other) {
		hFile = _other.hFile;
		_other.hFile = nullptr;
		return *this;
	}
	template<typename T> unsigned WriteBytes(unsigned _size, const T* _buffer) {
#ifdef _DEBUG
		if (!IsOpen()) throw FileError(FileError::Code::NotOpened);
#endif
		const unsigned write = fwrite(_buffer, _size, 1, hFile);
		if (!write) throw FileError((FileError::Code)errno);
		return  write;
	}
	template<typename T> unsigned ReadBytes(unsigned _size, T* _buffer)const {
#ifdef _DEBUG
		if (!IsOpen()) throw FileError(FileError::Code::NotOpened);
#endif
		const unsigned read = fread(_buffer, _size, 1, hFile);
		if (!read) throw FileError((FileError::Code)errno);
		return  read;
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
		fseek(hFile, _index, (int)_moveType);
	}
	unsigned GetSize()const {
#ifdef _DEBUG
		if (!IsOpen()) throw FileError(FileError::Code::NotOpened);
#endif
		const long pos = ftell(hFile);
		fseek(hFile, 0, SEEK_END);
		const unsigned size = (unsigned)ftell(hFile);
		fseek(hFile, pos, SEEK_SET);
		return size;
	}
	static void Delete(const Tchar* path) {
		int error = remove(path);
		if (error!=0) throw FileError((FileError::Code)error);
	}
	void Close() {
#ifdef _DEBUG
		if (!IsOpen()) throw FileError(FileError::Code::NotOpened);
#endif
		fclose(hFile);
		hFile = nullptr;
	}
	unsigned GetPos()const {
#ifdef _DEBUG
		if (!IsOpen()) throw FileError(FileError::Code::NotOpened);
#endif
		return (unsigned)ftell(hFile);
	}
	template<typename T> unsigned Read(T* _data)const {return fread(&_data, sizeof(T), 1, hFile);}
	template <typename T> unsigned Write(const T& _data) { return fwrite(&_data, sizeof(T), 1, hFile); }
	~File() { if (IsOpen())Close(); }
};
#endif