#pragma once

#include "Base.h"

BEGIN(Engine)

class CLayer final : public CBase
{
private:
	CLayer();
	virtual ~CLayer() = default;

public:
	HRESULT Add_GameObject(class CGameObject* pGameObject);
	void Tick(_double TimeDelta);
	void LateTick(_double TimeDelta);

private:
	list<class CGameObject*>							m_GameObjects;
	unordered_map<wstring, CGameObject*>				m_objectStore;
	unordered_multimap<FamilyId, CGameObject*>			m_componentStore;

public:
	static CLayer* Create();
	virtual void Free() override;
};

END