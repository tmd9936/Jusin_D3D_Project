#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTransform;
END

BEGIN(Client)

class CStageCameraTarget final : public CGameObject
{
public:
	enum STATE
	{
		STATE_FORMATION,
		STATE_STOP,
		STATE_END
	};

private:
	CStageCameraTarget(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CStageCameraTarget(const CStageCameraTarget& rhs);
	virtual ~CStageCameraTarget() = default;

public:
	virtual HRESULT Initialize_Prototype() override; /* 원형객체의 초기화작업 */
	virtual HRESULT Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg) override; /* 사본객체의 초기화작업 */
	virtual _uint Tick(_double TimeDelta) override;
	virtual _uint LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

private:
	HRESULT Add_Components();
	HRESULT SetUp_ShaderResources();

private:
	void			Change_State();
	void			Formation_State_Tick(const _double& TimeDelta);
	CTransform*		Get_PlayerTransform();
private:
	CTransform* m_pTransformCom = { nullptr };

	STATE		m_eCurrentState = { STATE_FORMATION };
	STATE		m_ePreState = { STATE_END };

public:
	/* Prototype */
	/* 원형 객체를 생성한다. */
	static CStageCameraTarget* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	/* 사본 객체를 생성한다. */
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg = nullptr) override;
	virtual void Free() override;
};

END