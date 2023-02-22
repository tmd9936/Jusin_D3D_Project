#pragma once

#include "GameObject.h"
#include "Engine_Defines.h"
#include "Transform.h"

BEGIN(Engine)

class CPipeLine;

class ENGINE_DLL CCamera abstract : public CGameObject
{
public:
	typedef struct CameraDesc
	{
		_float4		vEye, vAt, vAxisY;

		_float		fFovy, fAspect, fNear, fFar;

		CTransform::TRANSFORMDESC		TransformDesc;
	}CAMERADESC;

protected:
	CCamera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCamera(const CCamera& rhs);
	virtual ~CCamera() = default;

public:
	virtual HRESULT Initialize_Prototype() override; /* 원형객체의 초기화작업 */
	virtual HRESULT Initialize(void* pArg) override; /* 사본객체의 초기화작업 */
	virtual _uint Tick(_double TimeDelta) override;
	virtual _uint LateTick(_double TimeDelta) override;

protected:
	CTransform* m_pTransform = { nullptr };
	CPipeLine* m_pPipeLine = { nullptr };
	CAMERADESC					m_CameraDesc = { };

private:
	HRESULT Add_Components();

public:
	virtual CGameObject* Clone(void* pArg = nullptr) = 0;
	virtual void Free() override;

};

END

