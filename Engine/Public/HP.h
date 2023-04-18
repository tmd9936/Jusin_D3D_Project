#pragma once
#include "Component.h"

BEGIN(Engine)

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
	_uint		Tick(const _double& Timedelta);

public:
	const _int	Get_MaxHp() const {
		return m_MaxHP;
	}

	const _int	Get_CurrentHp() const {
		return m_CurrentHP;
	}

public:
	const _float Get_HP_Ratio() const {
		return m_CurrentHP / (_float)m_MaxHP;
	}

	const _bool Is_DamageEvent() const {
		return m_GetDamageEvent;
	}

	const _int	Get_DamageRecieved() const {
		return m_DamageReceived;
	}

	const _bool	Is_Dead() const {
		if (m_CurrentHP < 0)
			return true;
		else
			return false;
	}

public:
	void Get_Damage(_int damage);

	void Get_PercentDamage(_float damagePercent);

	void Heal(_int healNum) {
		m_CurrentHP += healNum;
		if (m_CurrentHP > m_MaxHP)
			m_CurrentHP = m_MaxHP;
	}

	void Set_DamageGetPercent(_float percent);
	_float	Add_DamageGetPercent(_float percent);

	void Set_CanGetDamage(const _bool& bCanGetDamage) {
		m_bCanGetDamage = bCanGetDamage;
	}

	const _bool Get_CanGetDamage() const {
		return m_bCanGetDamage;
	}
private:	
	HP_DESC		m_Desc = {};

	_int		m_MaxHP = { 0 };
	_int		m_CurrentHP = { 0 };

	_int		m_DamageReceived = { 0 };
	_bool		m_GetDamageEvent = { false };

	_float		m_DamageGetPercent = { 1.f };

	_bool		m_bCanGetDamage = { true };
public:
	static CHP* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(CGameObject* pOwner, void* pArg = nullptr) override;

private:
	virtual void Free();

};

END