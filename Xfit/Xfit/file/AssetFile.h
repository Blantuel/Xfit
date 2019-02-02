#pragma once

#ifdef __ANDROID__

#include "../_system/_Android.h"
#include "../system/Error.h"


class AssetFileError :public Error {
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
	AssetFileError(AssetFileError::Code _code) :Error((int)_code) {}
};
class AssetFile {
	AAsset* hFile;

	AssetFile(const AssetFile& _other) {}
	AssetFile& operator=(const AssetFile& _other) {}
public:
	enum class OpenMode {
		Read
	};
	enum class MoveOption {
		Begin = SEEK_SET,
		Current = SEEK_CUR,
		End = SEEK_END
	};
	AssetFile() :hFile(nullptr) {}
	AssetFile(const char* _path, OpenMode _openMode = OpenMode::Read) { Open(_path, _openMode); }

	bool IsOpen()const { return hFile != nullptr; }
	void Open(const char* _path, OpenMode _openMode = OpenMode::Read) {
#ifdef _DEBUG
		if (IsOpen());
#endif
		switch (_openMode) {
		case OpenMode::Read:hFile = AAssetManager_open(_System::_Android::app.activity->assetManager,_path,AASSET_MODE_STREAMING); break;
#ifdef _DEBUG
		default:break;
#endif
		}
		if (!hFile) throw AssetFileError((AssetFileError::Code)errno);
	}
	AssetFile(AssetFile && _other) {
		hFile = _other.hFile;
		_other.hFile = nullptr;
	}
	AssetFile& operator=(AssetFile && _other) {
		hFile = _other.hFile;
		_other.hFile = nullptr;
		return *this;
	}
	template<typename T> unsigned ReadBytes(unsigned _size, T * _buffer) {
#ifdef _DEBUG
		if (!IsOpen()) throw AssetFileError(AssetFileError::Code::NotOpened);
#endif
		const int read = AAsset_read(hFile,_buffer,(unsigned)_size);
		if (!read) throw AssetFileError((AssetFileError::Code)errno);
		return  read;
	}
	void SetPos(int _index = 0, MoveOption _moveType = MoveOption::Begin) {
#ifdef _DEBUG
		if (!IsOpen()) throw AssetFileError(AssetFileError::Code::NotOpened);
		int index = _index;
		if (_moveType == MoveOption::Current) index += GetPos();
		else if (_moveType == MoveOption::End) index = GetSize() - 1 - index;
		if (index > (GetSize() - 1))throw AssetFileError(AssetFileError::Code::OverflowIndex);
		else if (index < 0) throw AssetFileError(AssetFileError::Code::MinusIndex);
#endif
		AAsset_seek(hFile,(int)index,(int)_moveType);
	}
	unsigned GetSize()const {
#ifdef _DEBUG
		if (!IsOpen()) throw AssetFileError(AssetFileError::Code::NotOpened);
#endif
		return (unsigned)AAsset_getLength(hFile);
	}
	static void Delete(const Tchar * path) {
		int error = remove(path);
		if (error != 0) throw AssetFileError((AssetFileError::Code)error);
	}
	void Close() {
#ifdef _DEBUG
		if (!IsOpen()) throw AssetFileError(AssetFileError::Code::NotOpened);
#endif
		AAsset_close(hFile);
		hFile = nullptr;
	}
	unsigned GetPos()const {
#ifdef _DEBUG
		if (!IsOpen()) throw AssetFileError(AssetFileError::Code::NotOpened);
#endif
		return (unsigned)(AAsset_getLength(hFile)-AAsset_getRemainingLength(hFile));
	}
	template<typename T> unsigned Read(T * _data)const { return ReadBytes(sizeof(T), _data); }
	template <typename T> unsigned Write(const T & _data) { return WriteBytes(sizeof(T), &_data); }
	~AssetFile() { if (IsOpen())Close(); }
};
#endif