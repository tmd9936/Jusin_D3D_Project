#pragma once

#include "Client_Defines.h"
#include "Camera.h"

BEGIN(Engine)

class CTransform;

END

BEGIN(Client)

class CStageCameraTarget;

class  CStageCamera final : public CCamera
{
public:
	enum STATE
	{
		STATE_FADE_IN,
		STATE_FORMATION,
		STATE_SHAKE,
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

		_float3		m_DistancefromAt;

		_double		m_maxZoomTime;
		_double		m_shakeCoolTime;

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

public:
	/// <summary>
	/// 
	/// </summary>
	/// <param name="vPosition"> </param>
	/// <param name="TImeDelta"></param>
	/// <param name="limitDistance"></param>
	/// <returns> False 움직이는 중, True 도착</returns>
	_bool	Focus_To_Object(const _float4& vPosition, const _float& TImeDelta, const _float& limitDistance);

	void	Do_Shake() {
		if (m_CanShake)
		{
			m_eCurState = STATE_SHAKE;
		}
	}

protected:
	virtual _bool			Save_By_JsonFile_Impl(Document& doc, Document::AllocatorType& allocator);
	virtual _bool			Load_By_JsonFile_Impl(Document& doc);

private:
	HRESULT					Add_Components();

private:
	HRESULT					Init_CameraPos();

private:
	void					Data_Save_Logic();

private:
	_uint					State_LateTick(const _double& TimeDelta);
	void					State_Change();

private:
	_uint					FadeIn_Chase_CameraAt(const _double& TimeDelta);

private:
	void					Camera_Shake_Tick(const _double& TimeDelta);
	void					Camemra_Shake_Init();
	void					Camemra_Shake_CoolTimeCheck(const _double& TimeDelta);

private:
	_uint					Chase_CameraAt(const _double& TimeDelta);
	void					Change_AdditionalDistanceByCameraAt(const _double& TimeDelta);
	void					Change_AdditionalDistanceByCulling(const _double& TimeDelta);

	_bool					Get_PlayerCulling(const _tchar* pObjectTag);

	void					Zoom_Out_From_CameraTarget(const _double& TimeDelta);
	_bool					Zoom_In_From_CameraTarget(const _double& TimeDelta);

private:
	STAGE_CAMERA_DESC		m_StageCameraDesc = { };
	/// <summary>
	/// 카메라와 At간 유지할 거리
	/// </summary>
	_vector					m_vDistanceVectorFromAt = { };
	CGameObject*			m_pTarget = { nullptr };

	STATE					m_eCurState = { STATE_FADE_IN };
	STATE					m_ePreState = { STATE_END };

	_float					m_CurAdditionalDistance = { 1.1f };

	_double					m_MaxZoomTimeAcc = { 0.0 };

	_double					m_ShakeCoolTimeAcc = { 2.0 };
	_bool					m_CanShake = { true };

	_double					m_ShakeTimeAcc = { 0.0 };
	_double					m_ShakePeriodTimeAcc = { 0.0 };
	_double					m_CurShakeDegree = { 0.0 };

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
