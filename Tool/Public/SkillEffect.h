#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTransform;
class CRenderer;
class CShader;
class CModel;
class CNavigation;
class CBone;

END

BEGIN(Client)

class CLoader;

class CSkillEffect : public CGameObject
{
public:
	enum ACTION_TYPE { ACTION_ATTACK, ACTION_NONE, ACTION_MIND, ACTION_LOOP, ACTION_END };

public:
	typedef struct Effect_Desc
	{
		wstring					m_effectPath;
		wstring					m_ProtoTypeTag;
		wstring					m_exPath1;
		wstring					m_exPath2;
		_uint					m_effectType;
		_uint					m_actionType;
		_uint					m_soundEventID;
		wstring					m_soundEventTag;
		_uint					m_underFlag;

		_bool					m_IsParts = { false };
		_bool					m_bParentRotateApply = { true };

		_float4x4				m_FinalWorldMatrix = {}; /* 원점기준 (내 월드 * 부모월드) */

		_double					m_AnimationLoopTime = { 0.0 };
		_double					m_AnimationStartAcc = { 0.0 };
		_double					m_AnimationSpeed = { 1.0 };

		_int					m_CurrentLoopCount = { 0 };

		CBone*					pBonePtr = { nullptr };
		CTransform*				pParent = { nullptr };
		_float4x4				PivotMatrix;

	} EFFECT_DESC;

protected:
	CSkillEffect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CSkillEffect(const CSkillEffect& rhs);
	virtual ~CSkillEffect() = default;

public:
	virtual HRESULT Initialize_Prototype() override; /* 원형객체의 초기화작업 */
	virtual HRESULT Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg) override; /* 사본객체의 초기화작업 */

	virtual _uint Tick(_double TimeDelta) override;
	virtual _uint LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

public:
	void	Set_Pos(const _float4& vPos);

	void	Set_Parent(CBone* pBoneParent, CTransform* pTransformParent, _float4x4	PivotMatrix);
	void	Set_Parent(CBone* pBoneParent, CTransform* pTransformParent, _matrix PivotMatrix);

	void	Set_Parent(CBone* pBoneParent, CTransform* pTransformParent);

	void	Init_LoopCount(_uint loopCount) {
		m_EffectDesc.m_CurrentLoopCount = loopCount;
	}

	void	Set_AnimaitonStartTime(_double time);

	void	Set_Animation_Speed(_double speed) {
		m_EffectDesc.m_AnimationSpeed = speed;
	}

	void	Set_ParentRotateApply(_bool parentRotateApply) {
		m_EffectDesc.m_bParentRotateApply = parentRotateApply;
	}

protected:
	void	Set_ParentBone(CBone* pParent) {
		m_EffectDesc.pBonePtr = pParent;
		Safe_AddRef(m_EffectDesc.pBonePtr);
	}

	void	Set_ParentTransform(CTransform* pParent) {
		m_EffectDesc.pParent = pParent;
		Safe_AddRef(m_EffectDesc.pParent);
	}

protected:
	/// <summary>
	/// 여기에서 이펙트의 애니메이션을 재생하고 정해진 루프 카운터를 감소시킴
	/// </summary>
	/// <param name="TimeDelta">프레임 틱</param>
	void		Loop_Count_Check(const _double& TimeDelta);

protected:
	CTransform*			m_pTransformCom = { nullptr };
	CRenderer*			m_pRendererCom = { nullptr };
	CShader*			m_pShaderCom = { nullptr };
	CModel*				m_pModelCom = { nullptr };
	CNavigation*		m_pNavigationCom = { nullptr };

protected:
	EFFECT_DESC				m_EffectDesc = {};

	_double					m_CurrentAnimationAcc = { 0.0 };

protected:
	virtual HRESULT Add_Components();

	HRESULT SetUp_ShaderResources(); /* 셰이더 전역변수에 값을 던진다. */

	friend CLoader;
	static CSkillEffect* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
public:
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg = nullptr) override;
	virtual void Free() override;
};

END