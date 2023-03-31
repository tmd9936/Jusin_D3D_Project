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
		_uint m_HpChangeTick;
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
	_uint	Tick();


public:
	const _uint		Get_MaxHpP() const {
		return m_MaxHP;
	}

	const _uint		Get_CurrentHp() const {
		return m_CurrentHP;
	}

	_bool 	Set_TargetHP(_uint targetHP);

public:
	const _float		Get_HP_Ratio() const {
		return m_CurrentHP / m_MaxHP;
	}

private:
	HP_DESC		m_Desc = {};

	_uint		m_MaxHP = { 0 };
	_uint		m_CurrentHP = { 0 };

	_uint		m_TargetHP = { 0 };

public:
	static CHP* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(CGameObject* pOwner, void* pArg = nullptr) override;

private:
	virtual void Free();

};

END