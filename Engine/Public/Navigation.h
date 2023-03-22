#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CNavigation final : public CComponent
{
public:
	static const FamilyId familyId = FAMILY_ID_NAVIGATION;

private:
	CNavigation(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, CGameObject* pOwner);
	CNavigation(const CNavigation& rhs, CGameObject* pOwner);
	virtual ~CNavigation() = default;

public:
	HRESULT Initialize_Prototype(const _tchar* pNavigationData);
	HRESULT Initialize(void* pArg) override;
	HRESULT Render();

private:
	_int					m_iCurrentIndex = { -1 };
	vector<class CCell*>	m_Cells;

#ifdef _DEBUG
private:
	class CShader* m_pShader = { nullptr };
#endif // _DEBUG

private:
	HRESULT SetUp_Neighbors();

public:
	static CNavigation* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pNavigationData);
	virtual CComponent* Clone(CGameObject* pOwner, void* pArg = nullptr) override;
	virtual void Free();

};

END