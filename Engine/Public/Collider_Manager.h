#pragma once

#include "Base.h"
#include "Engine_Defines.h"

BEGIN(Engine)

class CCollider;
class CGameObject;

union COLLIDER_ID
{
	struct
	{
		UINT Left_id;
		UINT Right_id;
	};
	ULONGLONG ID;
};

class CCollider_Manager final : public CBase
{
	DECLARE_SINGLETON(CCollider_Manager)


private:
	explicit CCollider_Manager();
	virtual ~CCollider_Manager() = default;

public:
	void Update_CollisionMgr(_uint iLevelIndex);
	HRESULT Add_Check_CollisionGroup(wstring LeftLayerTag, wstring RightLayerTag);

public:
	void Collision_GroupUpdate(wstring LeftTag, wstring RightTag, _uint iLevelIndex);
	bool Is_Collision(CCollider* pLeftCol, CCollider* pRightCol, _float* fX, _float* fY, _float* fZ);
	void Get_Min_Max(CCollider* Col, _vector& vMin, _vector& vMax);
	XMVECTOR Get_Min_Vector(CCollider* Col);
	XMVECTOR Get_Max_Vector(CCollider* Col);

	_float Get_Min(_float fSour, _float fDest);
	_float Get_Max(_float fSour, _float fDest);

	void Reset_CollisionGroup() {
		m_ColLayers.clear();
	}

	vector<pair<wstring, wstring>> m_ColLayers;

	map<ULONGLONG, _bool> m_mapColInfo;

public:
	virtual void Free(void) override;

};

END