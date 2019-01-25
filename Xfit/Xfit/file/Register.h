#pragma once

#ifdef _WIN32

#include "../text/Text.h"

class Registry {
private:
	HKEY hKey;
	Tchar* str;
public:
	Registry():hKey(0),str() {}
	~Registry() { Close(); }

	Registry(const Tchar* path,bool _deleteAtSystemExit=false) { Create(path,_deleteAtSystemExit); }

	bool Create(const Tchar* _path,bool _deleteAtSystemExit=false) {
		lstrcpy(str,_T("Software\\"));
		lstrcat(str,_path);

		return RegCreateKeyEx(HKEY_CURRENT_USER,str,0,nullptr,_deleteAtSystemExit?REG_OPTION_VOLATILE:REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,0,&hKey,nullptr)==ERROR_SUCCESS;
	}
	bool Open(Tchar* _path) {
		lstrcpy(str,_T("Software\\"));
		lstrcat(str,_path);

		return RegOpenKey(HKEY_CURRENT_USER,str,&hKey)==ERROR_SUCCESS;
	}
	bool WriteString(const Tchar* _name,const Tchar* _string)const {
		return RegSetValueEx(hKey,_name,0,REG_SZ,(unsigned char*)_string,Text::GetCharArLen(_string)+1)==ERROR_SUCCESS;
	}
	bool WriteUnsigned(const Tchar* _name,unsigned _unsigned)const {
		return RegSetValueEx(hKey,_name,0,REG_DWORD,(unsigned char*)&_unsigned,sizeof(unsigned))==ERROR_SUCCESS;
	}
	bool WriteBytes(const Tchar* _name,const void* _data, unsigned _dataLen)const {
		return RegSetValueEx(hKey,_name,0,REG_BINARY,(unsigned char*)_data,_dataLen)==ERROR_SUCCESS;
	}
	Tchar* ReadString(const Tchar* _name, unsigned _strLen)const {
		Tchar* str=new Tchar[_strLen+1];
		DWORD len=_strLen+1;
		if(RegQueryValueEx(hKey,_name,nullptr,nullptr,(LPBYTE)str,&len)!=ERROR_SUCCESS)return nullptr;
		return str;
	}
	unsigned ReadUnsigned(const Tchar* _name)const {
		if(!hKey)return MAXDWORD;
		DWORD dword,len=sizeof(unsigned);
		if(RegQueryValueEx(hKey,_name,nullptr,nullptr,(LPBYTE)&dword,&len)!=ERROR_SUCCESS)return MAXDWORD32;
		return dword;
	}
	void* ReadBytes(const Tchar* _name, unsigned _dataLen)const {
		if(!hKey)return nullptr;
		void* data =new char[_dataLen];
		DWORD len=_dataLen;
		if(RegQueryValueEx(hKey,_name,nullptr,nullptr,(LPBYTE)data,&len)!=ERROR_SUCCESS)return nullptr;
		return data;
	}
	bool Close() {
		if(!hKey)return false;
		bool result=RegCloseKey(hKey);
		if(result)hKey=nullptr;
		return result;
	}
	bool Delete() {
		if(!hKey)return false;
		bool result = RegDeleteKey(HKEY_LOCAL_MACHINE,str)==ERROR_SUCCESS;
		if(result)hKey=nullptr;
		return result;
	}
	bool DeleteValue(const Tchar* _name) {
		if(!hKey)return false;
		return RegDeleteValue(hKey,_name)==ERROR_SUCCESS;
	}
};
#endif