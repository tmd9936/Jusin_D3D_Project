#pragma once

#include "AttackEffect.h"

BEGIN(Engine)

END

BEGIN(Client)

class CSkill_Manager;

class CBezierAttackEffect :
	public CAttackEffect
{
public:


public:
	typedef struct Homming_Attack_Effect_Desc
	{
		_float3					m_vPoint1 = {};
		_float3					m_vPoint2 = {};
		_float3					m_vPoint3 = {};

		ATTACK_EFFECT_DESC		attackEffectDesc;

	} BEZIER_ATTACK_EFFECT_DESC;

private:
	CBezierAttackEffect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBezierAttackEffect(const CBezierAttackEffect& rhs);
	virtual ~CBezierAttackEffect() = default;

public:
	virtual HRESULT Initialize_Prototype() override; /* 원형객체의 초기화작업 */
	virtual HRESULT Initialize_Prototype(BEZIER_ATTACK_EFFECT_DESC& desc);

	virtual HRESULT Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg) override; /* 사본객체의 초기화작업 */

	virtual _uint Tick(_double TimeDelta) override;
	virtual _uint LateTick(_double TimeDelta) override;

public:
	void Set_BezierPoints(_float3 vPoint1, _float3 vPoint2, _float3 vPoint3) {
		m_BezierAttackEffectDesc.m_vPoint1 = vPoint1;
		m_BezierAttackEffectDesc.m_vPoint2 = vPoint2;
		m_BezierAttackEffectDesc.m_vPoint3 = vPoint3;
	}

protected:
	void		Bezier(const _double& TimeDelta);

protected:
	BEZIER_ATTACK_EFFECT_DESC		m_BezierAttackEffectDesc = {};

	_float							m_BezierTime = { 0.f };

public:
	friend CLoader;
	static CBezierAttackEffect* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	static CBezierAttackEffect* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, BEZIER_ATTACK_EFFECT_DESC& desc);

	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg = nullptr) override;
	virtual void Free() override;

};

END