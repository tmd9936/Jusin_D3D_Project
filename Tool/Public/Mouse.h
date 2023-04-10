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

class CMouse final : public CGameObject
{
public:
	enum STATE { STATE_IDLE, STATE_CLICK, STATE_END };

private:
	CMouse(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMouse(const CMouse& rhs);
	virtual ~CMouse() = default;

public:
	virtual HRESULT Initialize_Prototype() override; /* 원형객체의 초기화작업 */
	virtual HRESULT Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg) override; /* 사본객체의 초기화작업 */

	virtual _uint Tick(_double TimeDelta) override;
	virtual _uint LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

private:
	void	Hide_TIme_Check(const _double& TimeDelta);

	void	Hide_State_Init();

	void	Mouse_Move_Check();

private:
	CTransform* m_pTransformCom = { nullptr };
	CRenderer* m_pRendererCom = { nullptr };
	CShader* m_pShaderCom = { nullptr };
	CModel* m_pModelCom = { nullptr };

private:
	STATE			m_eState = { STATE_CLICK };
	_float4x4		m_ViewMatrix = {};
	_float4x4		m_ProjMatrix = {};

	_float			m_alpha = { 1.f };

	_double			m_HideTime = { 1.0 };
	_double			m_HideTimeAcc = { 0.0 };
	_bool			m_Hide = { false };

private:
	HRESULT Add_Components();
	HRESULT SetUp_ShaderResources(); /* 셰이더 전역변수에 값을 던진다. */

public:
	static CMouse* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg = nullptr) override;
	virtual void Free() override;
};

END