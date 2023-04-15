#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTransform;
class CRenderer;
class CTexture;
class CShader;
class CModel;
class CBone;
END

BEGIN(Client)

class CMonster;

class CPokering final : public CGameObject
{
public:
	enum STATE
	{
		STATE_IDLE,
		STATE_COOLTIME_FIX,
		STATE_COOLTIME,
		STATE_COOLTIME_END,
		STATE_END
	};

public:
	typedef struct Pokering_Desc
	{
		CMonster*			pParent = { nullptr };

		CBone*				pBonePtr = { nullptr };
		CTransform*			pParentTransform = { nullptr };
		_float4x4			PivotMatrix;

		_tchar				modelPrototypeTag[MAX_PATH];
	} POKERING_DESC;

private:
	CPokering(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPokering(const CPokering& rhs);
	virtual ~CPokering() = default;

public:
	virtual HRESULT Initialize_Prototype() override; /* 원형객체의 초기화작업 */
	virtual HRESULT Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg) override; /* 사본객체의 초기화작업 */

	virtual _uint Tick(_double TimeDelta) override;
	virtual _uint LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

private:
	void	State_Change();

	void	State_Tick(const _double& TimeDelta);

	void	CoolTimeEnd_Check(const _double& TimeDelta);

private:
	CTransform*			m_pTransformCom = { nullptr };
	CRenderer*			m_pRendererCom = { nullptr };
	CShader*			m_pShaderCom = { nullptr };
	CModel*				m_pModelCom = { nullptr };

	POKERING_DESC		m_desc = { };
	
	STATE				m_eCurState = { STATE_IDLE };
	STATE				m_ePreState = { STATE_END };

	_float4x4			m_FinalWorldMatrix = {};

	_double				m_IdleStateTimeAcc = { 0.0 };

	_double				m_CoolTimeEndTime = { 0.4 };
	_double				m_CoolTimeEndTimeAcc = { 0.0 };

	_float4				m_CoolTimeEndColor = { 1.f, 1.f, 1.f, 1.f };

	_double				m_CoolTimeSpeed = { 0.0 };

	
private:
	HRESULT Add_Components();

	HRESULT SetUp_ShaderResources(); /* 셰이더 전역변수에 값을 던진다. */

public:
	static CPokering* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg = nullptr) override;
	virtual void Free() override;
};

END