#pragma once

#include "Client_Defines.h"
#include "Camera.h"

BEGIN(Engine)

class CTransform;

END

BEGIN(Client)

class  CStageCamera final : public CCamera
{
public:
	enum STATE
	{
		STATE_FADE_IN,
		STATE_BATTLE,
		STATE_MOVE_TO_BOSS,
		STATE_LOOK_AT_BOSS,
		STATE_RETURN_TO_PLAYER,
		STATE_END
	};

public:
	typedef struct tagStageCameraDesc
	{
		_float		m_distance;
		_float		m_shakeTime;
		_float		m_shakePeriodTime;
		_float		m_shakeDegree;

		_float		m_distanceMin;
		_float		m_distanceMax;

		_float		m_moveSpeed;
		_float		m_zoomSpeed;

		CCamera::CAMERADESC		CameraDesc;
	}STAGE_CAMERA_DESC;

private:
	CStageCamera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CStageCamera(const CStageCamera& rhs);
	virtual ~CStageCamera() = default;

public:
	virtual HRESULT Initialize_Prototype() override; /* 원형객체의 초기화작업 */
	virtual HRESULT Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg);
	virtual HRESULT Initialize(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath);

	virtual _uint Tick(_double TimeDelta) override;
	virtual _uint LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

public:
	/**
	@return False 움직이는 중, True 도착
	*/
	_bool	Focus_To_Object(const _float4& vPosition, const _float& TImeDelta, const _float& limitDistance);

protected:
	virtual _bool			Save_By_JsonFile_Impl(Document& doc, Document::AllocatorType& allocator);
	virtual _bool			Load_By_JsonFile_Impl(Document& doc);

private:
	HRESULT					Add_Components();

private:
	STAGE_CAMERA_DESC		m_StageCameraDesc = {};

	CGameObject*			m_pTarget = { nullptr };

public:
	/* Prototype */
	/* 원형 객체를 생성한다. */
	static CStageCamera* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	/* 사본 객체를 생성한다. */
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath = nullptr);
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg = nullptr) override;
	virtual void Free() override;
};

END
