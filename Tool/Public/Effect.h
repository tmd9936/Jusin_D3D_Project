#pragma once

#include "Client_Defines.h"
#include "GameObject.h"


BEGIN(Engine)
class CTransform;
class CRenderer;
class CShader;
class CModel;
class COBB;
class CNavigation;
class CBone;

END

BEGIN(Client)

class CLoader;

class CEffect final : public CGameObject
{
public:
	enum ACTION_TYPE { ACTION_ATTACK, ACTION_NONE, ACTION_MIND, ACTION_LOOP, ACTION_END };

public:
	typedef struct Effect_Desc
	{
		wstring	m_effectPath;
		wstring	m_ProtoTypeTag;
		wstring	m_exPath1;
		wstring	m_exPath2;
		_uint	m_effectType;
		_uint	m_actionType;
		_uint	m_soundEventID;
		wstring	m_soundEventTag;
		_uint	m_underFlag;
		_uint	m_bCollision;
		_double	m_AnimationLoopTime;

		CBone*		pBonePtr = { nullptr };
		CTransform* pParent = { nullptr };
		_float4x4	PivotMatrix;

	} EFFECT_DESC;

private:
	CEffect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEffect(const CEffect& rhs);
	virtual ~CEffect() = default;

public:
	virtual HRESULT Initialize_Prototype() override; /* 원형객체의 초기화작업 */
	virtual HRESULT Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg) override; /* 사본객체의 초기화작업 */

	virtual _uint Tick(_double TimeDelta) override;
	virtual _uint LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

public:
	virtual void On_Collision(CCollider* pOther, const _float& fX, const _float& fY, const _float& fZ);
	virtual void On_CollisionEnter(CCollider* pOther, const _float& fX, const _float& fY, const _float& fZ);
	virtual void On_CollisionExit(CCollider* pOther, const _float& fX, const _float& fY, const _float& fZ);

public:
	void	Set_Pos(const _float4& vPos);

	void	Set_Parent(CBone* pBoneParent, CTransform* pTransformParent, _float4x4	PivotMatrix);

	void	Set_SmallRotation(_float speed) {
		if (speed > 0.f)
		{
			m_SmallRotation = true;
			m_SmallRotationSpeed = speed;
		}
	}

	void	Set_BigRotation(_float speed, _float radius) {
		if (speed > 0.f)
		{
			m_BigRotation = true;
			m_BigRotationSpeed = speed;
			m_BigRotationRadius = radius;
		}
	}

	void	Init_LoopCount(_uint loopCount) {
		m_LoopCount = loopCount;
		m_CurrentLoopCount = loopCount;
	}

	void	Set_AnimaitonStartTime(_double time);

private:
	void	Set_ParentBone(CBone* pParent) {
		m_EffectDesc.pBonePtr = pParent;
		Safe_AddRef(m_EffectDesc.pBonePtr);
	}

	void	Set_ParentTransform(CTransform* pParent) {
		m_EffectDesc.pParent = pParent;
		Safe_AddRef(m_EffectDesc.pParent);
	}

private:
	CTransform*		m_pTransformCom = { nullptr };
	CRenderer*		m_pRendererCom = { nullptr };
	CShader*		m_pShaderCom = { nullptr };
	CModel*			m_pModelCom = { nullptr };
	COBB*			m_pColliderCom = { nullptr };
	CNavigation*	m_pNavigationCom = { nullptr };

private:
	EFFECT_DESC		m_EffectDesc = {};

	_double			m_CurrentAnimationAcc = { 0.0 };

	wstring			m_EffectTypeName = {};

	_bool			m_IsParts = { false };
	_bool			m_IsHomming = { false };

	_bool			m_SmallRotation = { false };
	_float			m_SmallRotationSpeed = { 0.f };

	_bool			m_BigRotation = { false };
	_float			m_BigRotationRadius = { 0.f };
	_float			m_BigRotationSpeed = { 0.f };

	_float4x4		m_FinalWorldMatrix = {}; /* 원점기준 (내 월드 * 부모월드) */

	_double			m_AnimationStartAcc = { 0 };

	_uint			m_LoopCount = { 0 };
	_uint			m_CurrentLoopCount = { 0 };

private:
	HRESULT Add_Components();

	HRESULT SetUp_ShaderResources(); /* 셰이더 전역변수에 값을 던진다. */

	friend CLoader;
	static CEffect* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
public:
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg = nullptr) override;
	virtual void Free() override;
};

END