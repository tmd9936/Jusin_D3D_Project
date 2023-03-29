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
		wstring	m_exPath1;
		wstring	m_exPath2;
		_uint	m_effectType;
		_uint	m_actionType;
		_uint	m_soundEventID;
		wstring	m_soundEventTag;
		_uint	m_underFlag;
		_uint	m_bCollision;
		_double	m_AnimationLoopTime;

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

private:
	CTransform*		m_pTransformCom = { nullptr };
	CRenderer*		m_pRendererCom = { nullptr };
	CShader*		m_pShaderCom = { nullptr };
	CModel*			m_pModelCom = { nullptr };
	COBB*			m_pColliderCom = { nullptr };
	CNavigation*	m_pNavigationCom = { nullptr };

private:
	EFFECT_DESC		m_EffectDesc = {};

	_double			m_CurrontAnimationAcc = { 0.0 };

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