#pragma once

#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CGameObject abstract : public CBase
{
protected:
	CGameObject(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	CGameObject(const CGameObject& rhs);
	virtual ~CGameObject() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual _uint Tick(_double TimeDelta);
	virtual _uint LateTick(_double TimeDelta);
	virtual HRESULT Render();


public:
	//void					Get_Components_FamilyId(vector<FamilyId>& vecFamilyIds);
	const wstring			Get_NameTag() const { return m_strNameTag; }

	const RENDERGROUP		Get_RenderId() const { return m_eRenderId; }
	void					Set_RenderId(RENDERGROUP eRenderId) { m_eRenderId = eRenderId; }

	const wstring			Get_LayerId() const { return m_eLayerId; }
	void					Set_LayerId(const wstring& eLayerId) { m_eLayerId = eLayerId; }

	void					Set_NameTag(wstring& strName) { m_strNameTag = strName; }
	void					Set_NameTag(const _tchar* strName) { m_strNameTag = strName; }

public:
	_bool					Is_Dead() { return m_bDead; }
	void					Set_Alive() { m_bDead = false; }
	void					Set_Dead() { m_bDead = true; }

protected:
	HRESULT Add_Components(_uint iLevelIndex, const _tchar* pPrototypeTag,
		const _tchar* pComponentTag, class CComponent** ppOut, void* pArg = nullptr);

	class CComponent* Find_Component(const _tchar* pComponentTag);

protected:
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };

protected:
	/* 해시테이블 */
	map<const _tchar*, class CComponent*>			m_Components;
	wstring											m_strNameTag = { };
	wstring											m_eLayerId = { };
	RENDERGROUP										m_eRenderId = { RENDER_END };

	_bool											m_bDead = { false };

public:
	virtual CGameObject* Clone(void* pArg = nullptr) = 0;
	virtual void Free() override;
};

END