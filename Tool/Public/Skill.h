#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTransform;
class CRenderer;
class CShader;
class CModel;

END

BEGIN(Client)

class CEffect;
class CLoader;

class CSkill final : public CGameObject
{
public:
	typedef struct Skill_Desc
	{
		wstring	m_skillPath;
		wstring	m_iconPath;
		_float	m_damagePercent;
		_float	m_chargeSecond;
		_float	m_rapidDelay;
		_bool	m_isEnablePotential_Nway;
		_bool	m_isEnablePotential_Distance;
		_bool	m_isEnablePotential_Extend;
		_bool	m_isEnablePotential_Homing;
		_bool	m_isEnablePotential_Continue;
		_bool	m_isEnablePotential_Rapid;
		_bool	m_isEnablePotential_Charge;
		_bool	m_isEnablePotential_ConditionDuration;
		_bool	m_isEnablePotential_BuffProbUp;
		_bool	m_isEnablePotential_DebuffProbUp;
		_bool	m_isEnablePotential_BuffTimeUp;
		_bool	m_isEnablePotential_BuffShare;
		_bool	m_isEnablePotential_Knockback;
		_bool	m_isEnablePotential_Damage;
	} SKILL_DESC;

private:
	explicit CSkill(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CSkill(const CSkill& rhs);
	virtual ~CSkill() = default;

public:
	virtual HRESULT Initialize_Prototype() override; /* ������ü�� �ʱ�ȭ�۾� */
	virtual HRESULT Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg) override; /* �纻��ü�� �ʱ�ȭ�۾� */

	virtual _uint Tick(_double TimeDelta) override;
	virtual _uint LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

public:
	/**
	@param effects: this param is transferred to m_effects,
	which is a member variable of the skill class.
	*/
	void	Set_Effects(vector<CEffect*>& effects) {
		m_effects = move(effects);
	}

	/**
	@param conditions: this param is transferred to m_conditions, 
	which is a member variable of the skill class.
	*/
	void	Set_Conditions(vector<CEffect*>& conditions) {
		m_conditions = move(conditions);
	}

//
	void	Set_ChargeEffectTypesTypes(vector<_uint>& effects) {
		m_ChargeEffectTypes = effects;
	}

	void	Set_AttackEffecTypes(vector<_uint>& effects) {
		m_AttackEffectTypes = effects;
	}

	void	Set_CollisionEffectTypes(vector<_uint>& effects) {
		m_CollisionEffectTypes = effects;
	}

	void	Set_ConditionTypes(vector<_uint>& effects) {
		m_ConditionTypes = effects;
	}

public:
	void	Set_Effects_Pos(const _float4& vPos);
	void	Set_Conditions_Pos(const _float4& vPos);

	void	Set_Effects_Look(const _float4& vLook);
	void	Set_Conditions_Look(const _float4& vLook);

public:
	void	Set_Charge_Time(_double chargeTime) {
		m_ChargeTime = chargeTime;
	}

private:
	CTransform* m_pTransformCom = { nullptr };
	CRenderer* m_pRendererCom = { nullptr };

	_double		m_TestDeadDuration = { 10.f };

	_double		m_CurrentTestDeadDuration = { 0.f };

private:
	SKILL_DESC		m_SkillDesc;

	vector<CEffect*> m_effects;
	vector<CEffect*> m_conditions;

	vector<_uint> m_ChargeEffectTypes;
	vector<_uint> m_AttackEffectTypes;
	vector<_uint> m_CollisionEffectTypes;
	vector<_uint> m_ConditionTypes;

	_double		m_ChargeTime = { 0.0 };
	_double		m_ChargeTimeAcc = { 0.0 };

private:
	HRESULT Add_Components();

	HRESULT SetUp_ShaderResources(); /* ���̴� ���������� ���� ������. */

	friend CLoader;
	static CSkill* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
public:
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg = nullptr) override;
	virtual void Free() override;
};

END