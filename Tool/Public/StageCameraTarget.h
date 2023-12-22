#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTransform;
class CModel;
class CRenderer;
class CShader;
END

BEGIN(Client)

class CStageCameraTarget final : public CGameObject
{
public:
	typedef struct Stage_Camera_Target_Desc
	{
		_float		m_distanceCheckMinLength;
	} STAGE_CAMERA_TARGET_DESC;

public:
	enum STATE
	{
		STATE_FORMATION,
		STATE_STOP,
		STATE_END
	};

	enum MOVE_STATE
	{
		MOVE_STATE_NONE,
		MOVE_STATE_WIDEN, // 간격 벌어짐
		MOVE_STATE_NARROW, // 간격 좁아짐
		MOVE_STATE_END
	};

private:
	explicit CStageCameraTarget(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CStageCameraTarget(const CStageCameraTarget& rhs);
	virtual ~CStageCameraTarget() = default;

public:
	virtual HRESULT Initialize_Prototype() override; /* 원형객체의 초기화작업 */
	virtual HRESULT Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg) override; /* 사본객체의 초기화작업 */
	virtual _uint Tick(_double TimeDelta) override;
	virtual _uint LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

public:
	const MOVE_STATE	Get_MoveState() const {
		return m_eMoveState;
	}

	void	Start_Formation() {
		m_eCurrentState = STATE_FORMATION;
	}

	void	Stop_Formation() {
		m_eCurrentState = STATE_STOP;
	}

private:
	HRESULT Add_Components();
	HRESULT SetUp_ShaderResources();

private:
	void			Change_State();
	void			Formation_State_Tick(const _double& TimeDelta);
	CTransform*		Get_PlayerTransform(const _tchar* pObjectTag);

private:
	CTransform* m_pTransformCom = { nullptr };
	CRenderer* m_pRendererCom = { nullptr };
	CShader* m_pShaderCom = { nullptr };
	CModel* m_pModelCom = { nullptr };

private:
	STAGE_CAMERA_TARGET_DESC	m_Desc = {};
	STATE						m_eCurrentState = { STATE_FORMATION };
	STATE						m_ePreState = { STATE_END };

	_float						m_CurMaxDistanceFromPlayer = { 0.f };
	_float						m_PreMaxDistanceFromPlayer = { 0.f };

	MOVE_STATE					m_eMoveState = { MOVE_STATE_NONE };

public:
	/* Prototype */
	/* 원형 객체를 생성한다. */
	static CStageCameraTarget* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	/* 사본 객체를 생성한다. */
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg = nullptr) override;
	virtual void Free() override;
};

END