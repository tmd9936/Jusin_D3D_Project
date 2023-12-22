#pragma once
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CManualCollisionState : public CComponent
{
public:
	enum COLLISION_STATE
	{
		COLLISION_STATE_NONE,
		COLLISION_STATE_ENTER,
		COLLISION_STATE_ON,
		COLLISION_STATE_EXIT,
		COLLISION_STATE_END,
	};

public:
	static const FamilyId familyId = FAMILY_ID_MANUAL_COLLISION_STATE;
protected:
	explicit CManualCollisionState(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CGameObject* pOwner);
	explicit CManualCollisionState(const CManualCollisionState& rhs, CGameObject* pOwner);
	virtual ~CManualCollisionState() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;

public:
	_uint		Tick(const _double& Timedelta);

public:
	void		Set_State(COLLISION_STATE eState) {
		m_eState = eState;
	}

	const COLLISION_STATE	Get_State() const {
		return m_eState;
	}

private:
	COLLISION_STATE		m_eState = { COLLISION_STATE_END };

public:
	static CManualCollisionState* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(CGameObject* pOwner, void* pArg = nullptr) override;

private:
	virtual void Free();

};

END