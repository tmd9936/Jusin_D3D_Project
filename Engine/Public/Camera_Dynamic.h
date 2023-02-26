#pragma once
#include "Camera.h"
#include "Engine_Defines.h"

BEGIN(Engine)
class CTransform;

class ENGINE_DLL CCamera_Dynamic final : public CCamera
{
public:
	typedef struct tagCameraDynamicDesc
	{

		CCamera::CAMERADESC		CameraDesc;
	}CAMERA_DYNAMIC_DESC;
private:
	CCamera_Dynamic(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCamera_Dynamic(const CCamera_Dynamic& rhs);
	virtual ~CCamera_Dynamic() = default;

public:
	virtual HRESULT Initialize_Prototype() override; /* 원형객체의 초기화작업 */
	virtual HRESULT Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg) override; /* 사본객체의 초기화작업 */
	virtual _uint Tick(_double TimeDelta) override;
	virtual _uint LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

private:
	void Key_Input(const _double TimeDelta);

private:
	HRESULT Add_Components();

private:
	CAMERA_DYNAMIC_DESC			m_CameraDynamicDesc = {};
	_bool						m_bMouseMove = { true };

public:
	/* Prototype */
	/* 원형 객체를 생성한다. */
	static CCamera_Dynamic* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	/* 사본 객체를 생성한다. */
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg = nullptr) override;
	virtual void Free() override;
};

END
