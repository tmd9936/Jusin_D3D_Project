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
	virtual HRESULT Initialize_Prototype() override; /* ������ü�� �ʱ�ȭ�۾� */
	virtual HRESULT Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg) override; /* �纻��ü�� �ʱ�ȭ�۾� */
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
	/* ���� ��ü�� �����Ѵ�. */
	static CCamera_Dynamic* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	/* �纻 ��ü�� �����Ѵ�. */
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg = nullptr) override;
	virtual void Free() override;
};

END
