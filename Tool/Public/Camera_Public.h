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
		_double					leftMaxMove;
		_double					rightMaxMove;
		_double					upMaxMove;
		_double					downMaxMove;

		_int					leftMoveStart_MousePos;
		_int					rightMoveStart_MousePos;
		_int					upMoveStart_MousePos;
		_int					downMoveStart_MousePos;

		_double					currentLeftMove;
		_double					currentRightMove;
		_double					currentUpMove;
		_double					currentDownMove;

		_double					moveSpeed;
		_double					zoomSpeed;

		_float4					pokemonFocusOffset;
		_float					lookTime;
		_float					freeZoomDistance;

		CCamera::CAMERADESC		CameraDesc;
	}CAMERA_PUBLIC_DESC;

private:
	CCamera_Public(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCamera_Public(const CCamera_Public& rhs);
	virtual ~CCamera_Public() = default;

public:
	virtual HRESULT Initialize_Prototype() override; /* 원형객체의 초기화작업 */
	virtual HRESULT Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg);
	virtual HRESULT Initialize(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath);

	virtual _uint Tick(_double TimeDelta) override;
	virtual _uint LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

public:
	void Control_Off() {
		m_bControl = false;
	}
	void Control_On() {
		m_bControl = true;
	}
	/**
	@return False 움직이는 중, True 도착
	*/
	_bool	Focus_To_Object(const _float4& vPosition, const _float& TImeDelta, const _float& limitDistance);

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
	_bool						m_bControl = { true };

	_float4						m_BackPosition = {};


public:
	/* Prototype */
	/* 원형 객체를 생성한다. */
	static CCamera_Public* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	/* 사본 객체를 생성한다. */
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath = nullptr);
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg = nullptr) override;
	virtual void Free() override;
};

END
