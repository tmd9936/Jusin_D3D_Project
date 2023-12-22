#pragma once

#include "Client_Defines.h"
#include "Camera.h"

BEGIN(Engine)

class CTransform;
class CMonFSM;

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
		STATE_SKILL_ZOOM_IN,
		STATE_MOVE_TO_MONSTER,
		STATE_LOOK_AT_MONSTER,
		STATE_MOVE_TO_BOSS,
		STATE_LOOK_AT_BOSS,
		STATE_RETURN_TO_PLAYER,
		STATE_STAGE_CLEAR,
		STATE_END
	};

public:
	enum SHAKE_DIR
	{
		SHAKE_DIR_UP,
		SHAKE_DIR_DOWN,
		SHAKE_DIR_END
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

		_double		m_skillZoomInCoolTime;
		_double		m_skillZoomInAdditionalDistance;
		_double		m_skillZoomInAdditionalSpeed;


		CCamera::CAMERADESC		CameraDesc;
	}STAGE_CAMERA_DESC;

private:
	explicit CStageCamera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CStageCamera(const CStageCamera& rhs);
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

	void	Do_Shake();

	void	Do_Skill_Zoom_In(CGameObject* pObject);

	void	Set_Move_To_Point(CGameObject* pObject, _bool isBoss);

	void	Set_Stage_Clear() {
		m_eCurState = STATE_STAGE_CLEAR;
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
	void					CameraTarget_Formation_Start();
	void					CameraTarget_Formation_Stop();

private:
	void					Camera_Shake_LateTick(const _double& TimeDelta);
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
	void					Skill_Zoom_In_LateTick(const _double& TimeDelta);
	void					Skill_Zoom_In_End();
	void					Skill_Zoom_In_CoolTImeCheck(const _double& TimeDelta);

private:
	void					Move_To_LookTargetMonster(const _double& TimeDelta);
	void					LookTargetMonster_TimeCheck(const _double& TimeDelta);
	void					Return_To_Player(const _double& TimeDelta);

private:
	void					Move_To_LookTargetBoss(const _double& TimeDelta);
	void					Boss_Roar_Action();

private:
	STAGE_CAMERA_DESC		m_StageCameraDesc = { };
	/// <summary>
	/// 카메라와 At간 유지할 거리
	/// </summary>
	_vector					m_vDistanceVectorFromAt = { };
	CStageCameraTarget*		m_pStageCameraTarget = { nullptr };

	STATE					m_eCurState = { STATE_FADE_IN };
	STATE					m_ePreState = { STATE_END };

	_float					m_CurAdditionalDistance = { 1.1f };

	_double					m_MaxZoomTimeAcc = { 0.0 };

	_double					m_ShakeCoolTimeAcc = { 0.0 };
	_bool					m_CanShake = { true };

	_double					m_ShakeTimeAcc = { 0.0 };
	_double					m_ShakePeriodTimeAcc = { 0.0 };
	_bool					m_ShakePeriod = { true };
	SHAKE_DIR				m_CurShakeDirection = { SHAKE_DIR_UP };

	CGameObject*			m_pSkillZoomInTarget = { nullptr };
	_double					m_SkillZoomInCoolTimeAcc = { 0.0 };
	_bool					m_CanSkillZoomIn = { true };

	CGameObject*			m_pMoveTargetPoint = { nullptr };

	_double					m_movePointLookTime = { 1.5 };
	_double					m_movePointLookTimeAcc = { 0.0 };

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
