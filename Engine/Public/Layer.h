#pragma once

#include "Base.h"

BEGIN(Engine)

class CLayer final : public CBase
{
private:
	CLayer();
	virtual ~CLayer() = default;

public:
	HRESULT Add_GameObject(class CGameObject* pGameObject, const _tchar* pObjectNameTag = nullptr);
	_uint Tick(_double TimeDelta);
	void LateTick(_double TimeDelta);

public:
	HRESULT Store_Component(CGameObject* pGameObject, const FamilyId& id);
	CGameObject* Get_Object(const _tchar* pObjectNameTag);
	unordered_multimap<FamilyId, CGameObject*>* Get_ComponentStore() {
		return &m_componentStore;
	}

	HRESULT Remove_Component(const FamilyId& familyId, CGameObject* pObj);

	void Get_All_GameObject_In_Layer(vector<CGameObject*>& result);

private:
	HRESULT Erase_GameObject(CGameObject* pObj);
	HRESULT Remove_GameObject(CGameObject* pObj);

private:
	list<class CGameObject*>							m_GameObjects;
	unordered_map<wstring, CGameObject*>				m_objectStore;
	unordered_multimap<FamilyId, CGameObject*>			m_componentStore;

public:
	static CLayer* Create();
	virtual void Free() override;
};

END