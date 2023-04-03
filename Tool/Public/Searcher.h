#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTransform;
class CRenderer;
class CShader;
class CModel;
class CPickingCube;
class CSphere;
END

BEGIN(Client)

class CSearcher final : public CGameObject
{
public:
	typedef struct Searcher_Desc
	{
		CTransform* pParentTransformCom = { nullptr };
	} SEARCHER_DESC;

	enum COLLISION_STATE {
		COLLISION_STATE_NO,
		COLLISION_STATE_ON,
		COLLISION_STATE_ENTER,
		COLLISION_STATE_EXIT,
		COLLISION_STATE_END
	};

private:
	CSearcher(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CSearcher(const CSearcher& rhs);
	virtual ~CSearcher() = default;

public:
	virtual HRESULT Initialize_Prototype() override; /* 원형객체의 초기화작업 */
	virtual HRESULT Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg) override; /* 사본객체의 초기화작업 */

	virtual _uint Tick(_double TimeDelta) override;
	virtual _uint LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

public:
	void	Set_Active(_bool bActive) {
		m_bActive = bActive;
	}

	const CGameObject* Get_Target() const {
		if (m_pTarget)
		{
			Safe_AddRef(m_pTarget);
		}
		return m_pTarget;
	}

public:
	virtual void On_Collision(CCollider* pOther, const _float& fX, const _float& fY, const _float& fZ) override;
	virtual void On_CollisionEnter(CCollider* pOther, const _float& fX, const _float& fY, const _float& fZ) override;
	virtual void On_CollisionExit(CCollider* pOther, const _float& fX, const _float& fY, const _float& fZ) override;

public:
	const COLLISION_STATE Get_Collision_State() const {
		return m_eCollisionState;
	}

private:
	HRESULT Add_Components();

	HRESULT SetUp_ShaderResources(); /* 셰이더 전역변수에 값을 던진다. */

private:
	CRenderer* m_pRendererCom = { nullptr };
	CSphere* m_pSphereCom = { nullptr };

private:
	SEARCHER_DESC		m_Desc = {};
	COLLISION_STATE		m_eCollisionState = { COLLISION_STATE_NO };

	CGameObject*		m_pTarget = { nullptr };

	_bool				m_bActive = { true };

public:
	static CSearcher* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg = nullptr) override;
	virtual void Free() override;
};

END