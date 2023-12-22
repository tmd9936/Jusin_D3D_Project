#pragma once

#include "Client_Defines.h"
#include "Base.h"

class CFileDialogUtill final : public CBase
{
private:
	CFileDialogUtill() = default;
	~CFileDialogUtill() = default;
public:
	//static HRESULT File_Open_Dialog(string output);
	static string WstringToString(wstring value);

};

