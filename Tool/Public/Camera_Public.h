#pragma once

#include "Client_Defines.h"
#include "Camera.h"

BEGIN(Engine)

class CTransform;

END

BEGIN(Client)

class  CCamera_Public final : public CCamera
{
public:
	typedef struct tagCameraPublicDesc
	{
		_float					leftMaxMove;
		_float					rightMaxMove;
		_float					upMaxMove;
		_float					downMaxMove;

		_int					leftMoveStart_MousePos;
		_int					rightMoveStart_MousePos;
		_int					upMoveStart_MousePos;
		_int					downMoveStart_MousePos;

		_float					currentLeftMove;
		_float					currentRightMove;
		_float					currentUpMove;
		_float					currentDownMove;

		CCamera::CAMERADESC		CameraDesc;
	}CAMERA_PUBLIC_DESC;

private:
	CCamera_Public(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCamera_Public(const CCamera_Public& rhs);
	virtual ~CCamera_Public() = default;

public:
	virtual HRESULT Initialize_Prototype() override; /* ������ü�� �ʱ�ȭ�۾� */
	virtual HRESULT Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg);
	virtual HRESULT Initialize(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath);

	virtual _uint Tick(_double TimeDelta) override;
	virtual _uint LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

protected:
	virtual _bool			Save_Args_Impl(HANDLE hFile) override;
protected:
	virtual _bool			Save_By_JsonFile_Impl(Document& doc, Document::AllocatorType& allocator);
	virtual _bool			Load_By_JsonFile_Impl(Document& doc);

private:
	void Key_Input(const _double TimeDelta);

private:
	HRESULT Add_Components();

private:
	CAMERA_PUBLIC_DESC			m_CameraPublicDesc = {};
	_bool						m_bMouseMove = { true };

public:
	/* Prototype */
	/* ���� ��ü�� �����Ѵ�. */
	static CCamera_Public* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	/* �纻 ��ü�� �����Ѵ�. */
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath = nullptr);
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg = nullptr) override;
	virtual void Free() override;
};

END
