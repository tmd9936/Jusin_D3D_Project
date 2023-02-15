#pragma once

#pragma region COMPONENT_VIBUFFER
#include "VIBuffer_Rect.h"
#pragma endregion

#pragma region COMPONENT
#include "Renderer.h"
#include "Shader.h"
#pragma endregion


/* 0.����������Ʈ�� �������� �����Ѵ�. */
/* 1.Ư�� ������ ã�Ƽ� �����Ͽ� �����Ѵ�. */

BEGIN(Engine)

class CComponent_Manager final : public CBase
{
	DECLARE_SINGLETON(CComponent_Manager)

private:
	CComponent_Manager();
	virtual ~CComponent_Manager() = default;

public:
	HRESULT Reserve_Manager(_uint iNumLevels);
	void Clear(_uint iLevelIndex);

public:
	HRESULT Add_Prototype(_uint iLevelIndex, const _tchar* pPrototypeTag, class CComponent* pPrototype);
	class CComponent* Clone_Component(_uint iLevelIndex, const _tchar* pPrototypeTag, void* pArg = nullptr);

private:
	unordered_map<const _tchar*, class CComponent*>* m_pPrototypes = { nullptr };
	typedef unordered_map<const _tchar*, class CComponent*>			PROTOTYPES;

	_uint				m_iNumLevels = { 0 };

private:
	class CComponent* Find_Prototype(_uint iLevelIndex, const _tchar* pPrototypeTag);

public:
	virtual void Free() override;
};

END