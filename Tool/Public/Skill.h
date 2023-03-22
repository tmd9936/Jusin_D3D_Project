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
	CSkill(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CSkill(const CSkill& rhs);
	virtual ~CSkill() = default;

public:
	virtual HRESULT Initialize_Prototype() override; /* 원형객체의 초기화작업 */
	virtual HRESULT Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg) override; /* 사본객체의 초기화작업 */

	virtual _uint Tick(_double TimeDelta) override;
	virtual _uint LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

private:
	CTransform* m_pTransformCom = { nullptr };
	CRenderer* m_pRendererCom = { nullptr };
	CShader* m_pShaderCom = { nullptr };
	CModel* m_pModelCom = { nullptr };
private:
	SKILL_DESC		m_SkillDesc = {};

private:
	HRESULT Add_Components();

	HRESULT SetUp_ShaderResources(); /* 셰이더 전역변수에 값을 던진다. */

	friend CLoader;
	static CSkill* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
public:
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg = nullptr) override;
	virtual void Free() override;
};

END