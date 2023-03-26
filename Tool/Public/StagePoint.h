#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTransform;
class CRenderer;
class CShader;
class CModel;
class CAABB;
END

BEGIN(Client)

class CStagePoint final : public CGameObject
{
public:
	typedef struct Stage_Point_Desc
	{
		_float3		vPos;
		_uint		eState;
		_uint		stageNumber;
	} STAGE_POINT_DESC;

public:
	enum STAGE_POINT_STATE
	{
		STAGE_POINT_STATE_NO_VIEW,
		STAGE_POINT_STATE_STANDARD,
		STAGE_POINT_STATE_CLEAR,
		STAGE_POINT_STATE_BOSS,
		STAGE_POINT_STATE_EFFECT_CLEAR,
		STAGE_POINT_STATE_EFFECT_APPEAR,
		STAGE_POINT_STATE_EFFECT_APPEAR_LAST,
		STAGE_POINT_STATE_END
	};

private:
	CStagePoint(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CStagePoint(const CStagePoint& rhs);
	virtual ~CStagePoint() = default;

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
	CPickingCube*	m_pPickingCube = { nullptr };
	CAABB*			m_pAABB = { nullptr };

	STAGE_POINT_DESC		m_Desc = { };
	STAGE_POINT_STATE		m_eState = { STAGE_POINT_STATE_STANDARD };

private:
	HRESULT Add_Components();

	HRESULT SetUp_ShaderResources(); /* 셰이더 전역변수에 값을 던진다. */

public:
	static CStagePoint* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg = nullptr) override;
	virtual void Free() override;
};

END