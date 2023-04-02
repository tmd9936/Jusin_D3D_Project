#pragma once
#include "Component.h"

BEGIN(Engine)

class CGameObject;

class ENGINE_DLL CHP : public CComponent
{
public:
	typedef struct hp_Desc
	{
		_uint m_MaxHp;
		_bool m_HpChangeTick;
		_bool bDeadAfterOwnerDead;
	} HP_DESC;

public:
	static const FamilyId familyId = FAMILY_ID_HP;
protected:
	explicit CHP(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CGameObject* pOwner);
	explicit CHP(const CHP& rhs, CGameObject* pOwner);
	virtual ~CHP() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;


public:
	const _uint		Get_MaxHpP() const {
		return m_MaxHP;
	}

	const _uint		Get_CurrentHp() const {
		return m_CurrentHP;
	}

public:
	const _float		Get_HP_Ratio() const {
		return m_CurrentHP / (_float)m_MaxHP;
	}

	const _bool	Is_Dead() const {
		if (m_CurrentHP < 0)
			return true;
		else
			return false;
	}

public:
	void	Get_Damage(_uint damage);

	void Heal(_uint healNum) {
		m_CurrentHP += healNum;
		if (m_CurrentHP > m_MaxHP)
			m_CurrentHP = m_MaxHP;
	}

private:	
	HP_DESC		m_Desc = {};

	_uint		m_MaxHP = { 0 };
	_uint		m_CurrentHP = { 0 };


public:
	static CHP* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(CGameObject* pOwner, void* pArg = nullptr) override;

private:
	virtual void Free();

};

END