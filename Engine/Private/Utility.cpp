#include "Utility.h"

HRESULT CUtility::Load_Args_Data(const _tchar* filePath, void* args, _uint size)
{
	if (nullptr == filePath || nullptr == args)
		return E_FAIL;

	HANDLE		hFile = CreateFile(filePath, GENERIC_READ,
		NULL, NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	DWORD dwByte = 0;
	ReadFile(hFile, args, size, &dwByte, NULL);

	CloseHandle(hFile);
	return S_OK;
}
