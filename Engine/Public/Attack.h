#pragma once
#include "Component.h"

BEGIN(Engine)

class CGameObject;

class ENGINE_DLL CAttack : public CComponent
{
public:
	typedef struct Attack_Desc
	{
		_uint m_AttackPower;
	} ATTACK_DESC;

public:
	static const FamilyId familyId = FAMILY_ID_ATTACK;
protected:
	explicit CAttack(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CGameObject* pOwner);
	explicit CAttack(const CAttack& rhs, CGameObject* pOwner);
	virtual ~CAttack() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;


public:
	const _uint		Get_AttackPower() const {
		return m_Desc.m_AttackPower;
	}

	void	Set_AttackPower(_uint attackPower) {
		m_Desc.m_AttackPower = attackPower;
	}

private:
	ATTACK_DESC		m_Desc = {};


public:
	static CAttack* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(CGameObject* pOwner, void* pArg = nullptr) override;

private:
	virtual void Free();

};

END