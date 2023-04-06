#pragma once

#include "AttackEffect.h"

BEGIN(Engine)

END

BEGIN(Client)

class CSkill_Manager;

class CRushAttackEffect :
	public CAttackEffect
{
public:


public:
	typedef struct Homming_Attack_Effect_Desc
	{
		_vector					m_vRushDirection = {};
		_double					m_RushSpeed = { 1.0 };

		ATTACK_EFFECT_DESC		attackEffectDesc;

	} RUSH_ATTACK_EFFECT_DESC;

private:
	CRushAttackEffect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CRushAttackEffect(const CRushAttackEffect& rhs);
	virtual ~CRushAttackEffect() = default;

public:
	virtual HRESULT Initialize_Prototype() override; /* 원형객체의 초기화작업 */
	virtual HRESULT Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg) override; /* 사본객체의 초기화작업 */

	virtual _uint Tick(_double TimeDelta) override;
	virtual _uint LateTick(_double TimeDelta) override;

public:


protected:
	RUSH_ATTACK_EFFECT_DESC	m_RushAttackEffectDesc = {};

public:
	friend CLoader;
	static CRushAttackEffect* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg = nullptr) override;
	virtual void Free() override;

};

END