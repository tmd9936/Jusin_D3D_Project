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

void CUtility::Save_Matrix_in_json(Value& arrayValue, _float4x4& matrix, Document::AllocatorType& allocator)
{
	size_t index = 0;
	for (size_t i = 0; i < 4; i++)
	{
		for (size_t j = 0; j < 4; j++)
		{
			arrayValue.PushBack(matrix.m[i][j], allocator);
		}
	}
}
