#pragma once

#include "AttackEffect.h"

BEGIN(Engine)

END

BEGIN(Client)

class CSkill_Manager;

class CBumerangAttackEffect :
	public CAttackEffect
{
public:
	enum DIRECTION
	{
		DIRECTION_FRONT,
		DIRECTION_STAY,
		DIRECTION_BACK,
		DIRECTION_END
	};

public:
	typedef struct Bumerang_Attack_Effect_Desc
	{
		_double					m_RushSpeed = { 1.0 };
		_double					m_ChangeToBackTime = { 0.0 };

		ATTACK_EFFECT_DESC		attackEffectDesc;

	} BUMERANG_ATTACK_EFFECT_DESC;

private:
	CBumerangAttackEffect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBumerangAttackEffect(const CBumerangAttackEffect& rhs);
	virtual ~CBumerangAttackEffect() = default;

public:
	virtual HRESULT Initialize_Prototype() override; /* 원형객체의 초기화작업 */
	virtual HRESULT Initialize_Prototype(BUMERANG_ATTACK_EFFECT_DESC& desc);

	virtual HRESULT Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg) override; /* 사본객체의 초기화작업 */

	virtual _uint Tick(_double TimeDelta) override;
	virtual _uint LateTick(_double TimeDelta) override;

public:
	void		Set_RushSpeed(_double	rushSpeed) {
		m_BumerangAttackEffectDesc.m_RushSpeed = rushSpeed;
	}

	void		Set_ChangeToBackTime(_double changeToBackTime) {
		m_BumerangAttackEffectDesc.m_ChangeToBackTime = changeToBackTime;
	}

protected:
	void		Rush(const _double& TimeDelta);

protected:
	BUMERANG_ATTACK_EFFECT_DESC		m_BumerangAttackEffectDesc = {};
	
	DIRECTION						m_eDirection = { DIRECTION_FRONT };

	_double							m_ChangeToBackTimeAcc = { 0.0 };

	_double							m_StayTime = { 0.3 };
	_double							m_StayTimeAcc = { 0.0 };

public:
	friend CLoader;
	static CBumerangAttackEffect* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	static CBumerangAttackEffect* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, BUMERANG_ATTACK_EFFECT_DESC& desc);

	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg = nullptr) override;
	virtual void Free() override;

};

END