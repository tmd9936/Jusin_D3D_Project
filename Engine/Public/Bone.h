#pragma once

#include "Base.h"

BEGIN(Engine)

class CBone final : public CBase
{
private:
	CBone();
	virtual ~CBone() = default;
public:
	const char* Get_Name() const {
		return m_szName;
	}

public:
	HRESULT Initialize(aiNode* pAINode);

private:
	char			m_szName[MAX_PATH] = "";

public:
	static CBone* Create(aiNode* pAINode);
	virtual void Free() override;
};

END