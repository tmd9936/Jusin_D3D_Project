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
		_double					m_RushSpeed = { 1.0 };

		ATTACK_EFFECT_DESC		attackEffectDesc;

	} RUSH_ATTACK_EFFECT_DESC;

private:
	CRushAttackEffect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CRushAttackEffect(const CRushAttackEffect& rhs);
	virtual ~CRushAttackEffect() = default;

public:
	virtual HRESULT Initialize_Prototype() override; /* ������ü�� �ʱ�ȭ�۾� */
	virtual HRESULT Initialize_Prototype(RUSH_ATTACK_EFFECT_DESC& desc);

	virtual HRESULT Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg) override; /* �纻��ü�� �ʱ�ȭ�۾� */

	virtual _uint Tick(_double TimeDelta) override;
	virtual _uint LateTick(_double TimeDelta) override;

public:
	void		Set_RushSpeed(_double	rushSpeed) {
		m_RushAttackEffectDesc.m_RushSpeed = rushSpeed;
	}

protected:
	void		Rush(const _double& TimeDelta);

protected:
	RUSH_ATTACK_EFFECT_DESC		m_RushAttackEffectDesc = {};

public:
	friend CLoader;
	static CRushAttackEffect* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	static CRushAttackEffect* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, RUSH_ATTACK_EFFECT_DESC& desc);

	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg = nullptr) override;
	virtual void Free() override;

};

END