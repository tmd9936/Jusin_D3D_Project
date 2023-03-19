#pragma once

#include "Engine_Defines.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CUtility : public CBase
{
private:
	CUtility() = default;
	virtual ~CUtility() = default;

public:
	static HRESULT	Load_Args_Data(const _tchar* filePath, void* args, _uint size);
	static void		Save_Matrix_in_json(Value& arrayValue, _float4x4& matrix, Document::AllocatorType& allocator);


};

END
