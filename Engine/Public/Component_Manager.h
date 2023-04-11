#pragma once

#pragma region COMPONENT_VIBUFFER
#include "VIBuffer_Rect.h"
#include "VIBuffer_Terrain.h"
#include "VIBuffer_FlatTerrain.h"

#include "VIBuffer_Rect_Instance.h"
#pragma endregion

#pragma region COMPONENT
#include "Transform.h"
#include "Renderer.h"
#include "Shader.h"
#include "Texture.h"
#include "Calculator.h"
#include "Model.h"
#include "PickingCube.h"
#include "MonFSM.h"
#include "Billboard.h"
#include "Navigation.h"
#include "HP.h"
#include "Attack.h"
#include "Formation.h"
#include "ManualCollisionState.h"
#pragma endregion

#pragma region COLLIDER
#include "Collider.h"
#include "AABB.h"
#include "OBB.h"
#include "Sphere.h"
#pragma endregion



/* 0.����������Ʈ�� �������� �����Ѵ�. */
/* 1.Ư�� ������ ã�Ƽ� �����Ͽ� �����Ѵ�. */

BEGIN(Engine)

class CGameObject;

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
	class CComponent* Clone_Component(_uint iLevelIndex, const _tchar* pPrototypeTag, CGameObject* pGameObject, void* pArg = nullptr);

	_bool	Check_Prototype(const wstring prototypeTag);

private:
	unordered_map<wstring, class CComponent*>* m_pPrototypes = { nullptr };
	typedef unordered_map<wstring, class CComponent*>			PROTOTYPES;

	_uint				m_iNumLevels = { 0 };

private:
	class CComponent* Find_Prototype(_uint iLevelIndex, const _tchar* pPrototypeTag);

public:
	virtual void Free() override;
};

END