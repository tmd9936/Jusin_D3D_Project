#include "stdafx.h"
#include "StageCamera.h"

#include "GameInstance.h"

#include "StageCameraTarget.h"

CStageCamera::CStageCamera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CCamera(pDevice, pContext)
{
}

CStageCamera::CStageCamera(const CStageCamera& rhs)
	: CCamera(rhs)
{
}

HRESULT CStageCamera::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CStageCamera::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	memcpy(&m_StageCameraDesc, pArg, sizeof m_StageCameraDesc);

	if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, &m_StageCameraDesc.CameraDesc)))
		return E_FAIL;


	return S_OK;
}

HRESULT CStageCamera::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath)
{
	if (filePath)
	{
		Load_By_JsonFile(filePath);
		m_strSaveJsonPath = filePath;
	}

	if (FAILED(Init_CameraPos()))
		return E_FAIL;
	
	if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, &m_StageCameraDesc.CameraDesc)))
		return E_FAIL;

	return S_OK;
}

_uint CStageCamera::Tick(_double TimeDelta)
{
	if (KEY_TAB(KEY::K))
		m_pTransform->Go_Straight((_float)TimeDelta);
	else if (KEY_TAB(KEY::M))
		m_pTransform->Go_Backward((_float)TimeDelta);

	Camemra_Shake_CoolTimeCheck(TimeDelta);

	return __super::Tick(TimeDelta);
}

_uint CStageCamera::LateTick(_double TimeDelta)
{
	State_LateTick(TimeDelta);

	State_Change();

	return __super::LateTick(TimeDelta);
}

_uint CStageCamera::State_LateTick(const _double& TimeDelta)
{
	switch (m_eCurState)
	{
	case STATE_FADE_IN:
		FadeIn_Chase_CameraAt(TimeDelta);
		break;
	case STATE_FORMATION:
		Chase_CameraAt(TimeDelta);
		break;
	case STATE_BATTLE:
		Chase_CameraAt(TimeDelta);
		break;
	case STATE_SHAKE:
		Camera_Shake_Tick(TimeDelta);
		break;
	case STATE_MOVE_TO_BOSS:
		break;
	case STATE_LOOK_AT_BOSS:
		break;
	case STATE_RETURN_TO_PLAYER:
		break;
	case STATE_END:
		break;
	default:
		break;
	}

	return _uint();
}

void CStageCamera::State_Change()
{
	if (m_eCurState != m_ePreState)
	{
		switch (m_eCurState)
		{
		case STATE_FADE_IN:
			break;
		case STATE_FORMATION:
			break;
		case STATE_SHAKE:
			Camemra_Shake_Init();
			break;
		case STATE_BATTLE:
			break;
		case STATE_MOVE_TO_BOSS:
			break;
		case STATE_LOOK_AT_BOSS:
			break;
		case STATE_RETURN_TO_PLAYER:
			break;
		case STATE_END:
			break;
		default:
			break;
		}

		m_ePreState = m_eCurState;
	}
}

HRESULT CStageCamera::Init_CameraPos()
{
	CGameObject* pCameraTarget = CGameInstance::GetInstance()->Get_Object(LEVEL_STAGE, L"Layer_CameraTarget", L"CameraTarget");
	if (nullptr == pCameraTarget)
		return E_FAIL;
	CTransform* pTransform = pCameraTarget->Get_As<CTransform>();

	_float4 targetPos = {};
	XMStoreFloat4(&targetPos, pTransform->Get_State(CTransform::STATE_POSITION));

	m_StageCameraDesc.CameraDesc.vAt = targetPos;
	m_StageCameraDesc.CameraDesc.vEye = _float4(targetPos.x + m_StageCameraDesc.m_DistancefromAt.x, targetPos.y +
		m_StageCameraDesc.m_DistancefromAt.y, targetPos.z + m_StageCameraDesc.m_DistancefromAt.z, 1.f);

	m_vDistanceVectorFromAt = XMVector4Normalize(XMLoadFloat4(&m_StageCameraDesc.CameraDesc.vEye) - XMLoadFloat4(&m_StageCameraDesc.CameraDesc.vAt));

	return S_OK;
}

_uint CStageCamera::Chase_CameraAt(const _double& TimeDelta)
{
	CGameObject* pCameraTarget = CGameInstance::GetInstance()->Get_Object(LEVEL_STAGE, L"Layer_CameraTarget", L"CameraTarget");
	if (nullptr == pCameraTarget)
		return 0;
	CTransform* pTransform = pCameraTarget->Get_As<CTransform>();

	if (nullptr == pTransform)
		return 0;

	Change_AdditionalDistanceByCameraAt(TimeDelta);

	_vector movePoint = pTransform->Get_State(CTransform::STATE_POSITION) + (m_vDistanceVectorFromAt * m_StageCameraDesc.m_distance * m_CurAdditionalDistance);

	m_pTransform->ChaseNoLook(movePoint, (_float)TimeDelta * m_StageCameraDesc.m_moveSpeed, 0.2f);

	m_StageCameraDesc.CameraDesc = m_CameraDesc;

	return 0;
}

_uint CStageCamera::FadeIn_Chase_CameraAt(const _double& TimeDelta)
{
	CGameObject* pCameraTarget = CGameInstance::GetInstance()->Get_Object(LEVEL_STAGE, L"Layer_CameraTarget", L"CameraTarget");
	if (nullptr == pCameraTarget)
		return 0;
	CTransform* pTransform = pCameraTarget->Get_As<CTransform>();

	if (nullptr == pTransform)
		return 0;

	_vector movePoint = pTransform->Get_State(CTransform::STATE_POSITION) + (m_vDistanceVectorFromAt * m_StageCameraDesc.m_distance);
	if (m_pTransform->ChaseNoLook(movePoint, (_float)TimeDelta * m_StageCameraDesc.m_moveSpeed * 10.f, 1.8f))
	{
		m_eCurState = STATE_FORMATION;
	}

	m_StageCameraDesc.CameraDesc = m_CameraDesc;

	return 0;
}

void CStageCamera::Change_AdditionalDistanceByCameraAt(const _double& TimeDelta)
{
	CGameObject* pCameraTarget = CGameInstance::GetInstance()->Get_Object(LEVEL_STAGE, L"Layer_CameraTarget", L"CameraTarget");
	
	if (nullptr == pCameraTarget)
		return;

	CStageCameraTarget::MOVE_STATE moveState = static_cast<CStageCameraTarget*>(pCameraTarget)->Get_MoveState();

	switch (moveState)
	{
	case Client::CStageCameraTarget::MOVE_STATE_NONE:
		break;
	case Client::CStageCameraTarget::MOVE_STATE_WIDEN:
		m_CurAdditionalDistance += (_float)TimeDelta * 1.5f;

		if (m_CurAdditionalDistance > m_StageCameraDesc.m_distanceMax)
		{
			m_CurAdditionalDistance = m_StageCameraDesc.m_distanceMax;
		}
		m_MaxZoomTimeAcc = m_StageCameraDesc.m_maxZoomTime;
		break;
	case Client::CStageCameraTarget::MOVE_STATE_NARROW:
		if (m_MaxZoomTimeAcc > 0.0)
		{
			m_MaxZoomTimeAcc -= TimeDelta;
			return;
		}
		else
		{
			m_CurAdditionalDistance -= (_float)TimeDelta;

			if (m_CurAdditionalDistance < m_StageCameraDesc.m_distanceMin)
			{
				m_CurAdditionalDistance = m_StageCameraDesc.m_distanceMin;
			}
		}
		break;
	case Client::CStageCameraTarget::MOVE_STATE_END:
		break;
	default:
		break;
	}
}

void CStageCamera::Change_AdditionalDistanceByCulling(const _double& TimeDelta)
{
	if (Get_PlayerCulling(L"Player1"))
	{
		m_CurAdditionalDistance += (_float)TimeDelta * 2.f;

		if (m_CurAdditionalDistance > m_StageCameraDesc.m_distanceMax)
		{
			m_CurAdditionalDistance = m_StageCameraDesc.m_distanceMax;
		}
		m_MaxZoomTimeAcc = m_StageCameraDesc.m_maxZoomTime;
		return;
	}
	if (Get_PlayerCulling(L"Player2"))
	{
		m_CurAdditionalDistance += (_float)TimeDelta * 2.f;

		if (m_CurAdditionalDistance > m_StageCameraDesc.m_distanceMax)
		{
			m_CurAdditionalDistance = m_StageCameraDesc.m_distanceMax;
		}
		m_MaxZoomTimeAcc = m_StageCameraDesc.m_maxZoomTime;
		return;
	}
	if (Get_PlayerCulling(L"Player3"))
	{
		m_CurAdditionalDistance += (_float)TimeDelta * 2.f;

		if (m_CurAdditionalDistance > m_StageCameraDesc.m_distanceMax)
		{
			m_CurAdditionalDistance = m_StageCameraDesc.m_distanceMax;
		}
		m_MaxZoomTimeAcc = m_StageCameraDesc.m_maxZoomTime;
		return;
	}

	if (m_MaxZoomTimeAcc > 0.0)
	{
		m_MaxZoomTimeAcc -= TimeDelta;
		return;
	}
	else
	{
		m_CurAdditionalDistance -= (_float)TimeDelta;

		if (m_CurAdditionalDistance < m_StageCameraDesc.m_distanceMin)
		{
			m_CurAdditionalDistance = m_StageCameraDesc.m_distanceMin;
		}
	}
	
}

_bool CStageCamera::Get_PlayerCulling(const _tchar* pObjectTag)
{
	CGameObject* pPlyaer = CGameInstance::GetInstance()->Get_Object(LEVEL_STAGE, L"Layer_Player", pObjectTag);
	if (nullptr == pPlyaer)
		return false;

	return pPlyaer->Is_BeCulling();
}

void CStageCamera::Camera_Shake_Tick(const _double& TimeDelta)
{
	if (m_ShakeTimeAcc > m_StageCameraDesc.m_shakeTime)
	{
		m_eCurState = STATE_FORMATION;
		return;
	}

	m_CurShakeDegree += TimeDelta;
	m_ShakePeriodTimeAcc += TimeDelta;
	m_ShakeTimeAcc += TimeDelta;
}

void CStageCamera::Camemra_Shake_Init()
{
	m_ShakeTimeAcc = 0.0;
	m_ShakePeriodTimeAcc = 0.0;
	m_CurShakeDegree = 0.0;
}

void CStageCamera::Camemra_Shake_CoolTimeCheck(const _double& TimeDelta)
{
	if (!m_CanShake)
	{
		if (m_ShakeCoolTimeAcc >= m_StageCameraDesc.m_shakeCoolTime)
		{
			m_CanShake = true;
		}
		m_ShakeCoolTimeAcc += TimeDelta;
	}
}

_bool CStageCamera::Focus_To_Object(const _float4& vPosition, const _float& TImeDelta, const _float& limitDistance)
{
	return m_pTransform->ChaseNoLook(XMLoadFloat4(&vPosition), TImeDelta, limitDistance);
}

void CStageCamera::Data_Save_Logic()
{
	CGameObject* pCameraTarget = CGameInstance::GetInstance()->Get_Object(LEVEL_STAGE, L"Layer_CameraTarget", L"CameraTarget");
	if (nullptr == pCameraTarget)
		return;

	CTransform* pTargetTransform = pCameraTarget->Get_As<CTransform>();

	_float4 targetPos = {};
	XMStoreFloat4(&targetPos, pTargetTransform->Get_State(CTransform::STATE_POSITION));

	m_StageCameraDesc.m_DistancefromAt.x = -targetPos.x + m_StageCameraDesc.CameraDesc.vEye.x;
	m_StageCameraDesc.m_DistancefromAt.y = -targetPos.y + m_StageCameraDesc.CameraDesc.vEye.y;
	m_StageCameraDesc.m_DistancefromAt.z = -targetPos.z + m_StageCameraDesc.CameraDesc.vEye.z;

	if (KEY_TAB(KEY::V))
		Save_By_JsonFile("../../Reference/Resources/Data/Scene/Stage/Stage_Camera.json");
}

_bool CStageCamera::Save_By_JsonFile_Impl(Document& doc, Document::AllocatorType& allocator)
{
	if (m_pTransform)
	{
		XMStoreFloat4(&m_StageCameraDesc.CameraDesc.vEye, m_pTransform->Get_State(CTransform::STATE_POSITION));
		XMStoreFloat4(&m_StageCameraDesc.CameraDesc.vAt, m_pTransform->Get_State(CTransform::STATE_POSITION) + m_pTransform->Get_State(CTransform::STATE_LOOK));
	}

	Value CameraDesc(kObjectType);
	{
		CameraDesc.AddMember("m_distance", m_StageCameraDesc.m_distance, allocator);
		CameraDesc.AddMember("m_shakeTime", m_StageCameraDesc.m_shakeTime, allocator);
		CameraDesc.AddMember("m_shakePeriodTime", m_StageCameraDesc.m_shakePeriodTime, allocator);
		CameraDesc.AddMember("m_shakeDegree", m_StageCameraDesc.m_shakeDegree, allocator);

		CameraDesc.AddMember("m_distanceMin", m_StageCameraDesc.m_distanceMin, allocator);
		CameraDesc.AddMember("m_distanceMax", m_StageCameraDesc.m_distanceMax, allocator);

		CameraDesc.AddMember("m_moveSpeed", m_StageCameraDesc.m_moveSpeed, allocator);
		CameraDesc.AddMember("m_zoomSpeed", m_StageCameraDesc.m_zoomSpeed, allocator);

		CameraDesc.AddMember("m_maxZoomTime", m_StageCameraDesc.m_maxZoomTime, allocator);
		CameraDesc.AddMember("m_shakeCoolTime", m_StageCameraDesc.m_shakeCoolTime, allocator);

		Value m_DistancefromAt(kObjectType);
		{
			m_DistancefromAt.AddMember("x", m_StageCameraDesc.m_DistancefromAt.x, allocator);
			m_DistancefromAt.AddMember("y", m_StageCameraDesc.m_DistancefromAt.y, allocator);
			m_DistancefromAt.AddMember("z", m_StageCameraDesc.m_DistancefromAt.z, allocator);
		}
		CameraDesc.AddMember("m_DistancefromAt", m_DistancefromAt, allocator);

		Value vEye(kObjectType);
		{
			vEye.AddMember("x", m_StageCameraDesc.CameraDesc.vEye.x, allocator);
			vEye.AddMember("y", m_StageCameraDesc.CameraDesc.vEye.y, allocator);
			vEye.AddMember("z", m_StageCameraDesc.CameraDesc.vEye.z, allocator);
			vEye.AddMember("w", m_StageCameraDesc.CameraDesc.vEye.w, allocator);
		}
		CameraDesc.AddMember("vEye", vEye, allocator);

		Value vAt(kObjectType);
		{
			vAt.AddMember("x", m_StageCameraDesc.CameraDesc.vAt.x, allocator);
			vAt.AddMember("y", m_StageCameraDesc.CameraDesc.vAt.y, allocator);
			vAt.AddMember("z", m_StageCameraDesc.CameraDesc.vAt.z, allocator);
			vAt.AddMember("w", m_StageCameraDesc.CameraDesc.vAt.w, allocator);
		}
		CameraDesc.AddMember("vAt", vAt, allocator);

		Value vAxisY(kObjectType);
		{
			vAxisY.AddMember("x", m_StageCameraDesc.CameraDesc.vAxisY.x, allocator);
			vAxisY.AddMember("y", m_StageCameraDesc.CameraDesc.vAxisY.y, allocator);
			vAxisY.AddMember("z", m_StageCameraDesc.CameraDesc.vAxisY.z, allocator);
			vAxisY.AddMember("w", m_StageCameraDesc.CameraDesc.vAxisY.w, allocator);
		}
		CameraDesc.AddMember("vAxisY", vAxisY, allocator);

		CameraDesc.AddMember("fAspect", m_StageCameraDesc.CameraDesc.fAspect, allocator);
		CameraDesc.AddMember("fFovy", m_StageCameraDesc.CameraDesc.fFovy, allocator);
		CameraDesc.AddMember("fNear", m_StageCameraDesc.CameraDesc.fNear, allocator);
		CameraDesc.AddMember("fFar", m_StageCameraDesc.CameraDesc.fFar, allocator);

		Value TransformDesc(kObjectType);
		{
			TransformDesc.AddMember("RotationPerSec", m_StageCameraDesc.CameraDesc.TransformDesc.RotationPerSec, allocator);
			TransformDesc.AddMember("SpeedPerSec", m_StageCameraDesc.CameraDesc.TransformDesc.SpeedPerSec, allocator);
		}
		CameraDesc.AddMember("TransformDesc", TransformDesc, allocator);
	}

	doc.AddMember("CameraDesc", CameraDesc, allocator);

	return true;
}

_bool CStageCamera::Load_By_JsonFile_Impl(Document& doc)
{
	const Value& CameraDesc = doc["CameraDesc"];

	m_StageCameraDesc.m_distance = CameraDesc["m_distance"].GetFloat();
	m_StageCameraDesc.m_shakeTime = CameraDesc["m_shakeTime"].GetFloat();
	m_StageCameraDesc.m_shakePeriodTime = CameraDesc["m_shakePeriodTime"].GetFloat();
	m_StageCameraDesc.m_shakeDegree = CameraDesc["m_shakeDegree"].GetFloat();

	m_StageCameraDesc.m_distanceMin = CameraDesc["m_distanceMin"].GetFloat();
	m_StageCameraDesc.m_distanceMax = CameraDesc["m_distanceMax"].GetFloat();

	m_StageCameraDesc.m_moveSpeed = CameraDesc["m_moveSpeed"].GetFloat();
	m_StageCameraDesc.m_zoomSpeed = CameraDesc["m_zoomSpeed"].GetFloat();

	m_StageCameraDesc.m_maxZoomTime = CameraDesc["m_maxZoomTime"].GetDouble();
	m_StageCameraDesc.m_shakeCoolTime = CameraDesc["m_shakeCoolTime"].GetDouble();

	const Value& m_DistancefromAt = CameraDesc["m_DistancefromAt"];
	m_StageCameraDesc.m_DistancefromAt.x = m_DistancefromAt["x"].GetFloat();
	m_StageCameraDesc.m_DistancefromAt.y = m_DistancefromAt["y"].GetFloat();
	m_StageCameraDesc.m_DistancefromAt.z = m_DistancefromAt["z"].GetFloat();

	const Value& vEye = CameraDesc["vEye"];
	m_StageCameraDesc.CameraDesc.vEye.x = vEye["x"].GetFloat();
	m_StageCameraDesc.CameraDesc.vEye.y = vEye["y"].GetFloat();
	m_StageCameraDesc.CameraDesc.vEye.z = vEye["z"].GetFloat();
	m_StageCameraDesc.CameraDesc.vEye.w = vEye["w"].GetFloat();

	const Value& vAt = CameraDesc["vAt"];
	m_StageCameraDesc.CameraDesc.vAt.x = vAt["x"].GetFloat();
	m_StageCameraDesc.CameraDesc.vAt.y = vAt["y"].GetFloat();
	m_StageCameraDesc.CameraDesc.vAt.z = vAt["z"].GetFloat();
	m_StageCameraDesc.CameraDesc.vAt.w = vAt["w"].GetFloat();

	const Value& vAxisY = CameraDesc["vAxisY"];
	m_StageCameraDesc.CameraDesc.vAxisY.x = vAxisY["x"].GetFloat();
	m_StageCameraDesc.CameraDesc.vAxisY.y = vAxisY["y"].GetFloat();
	m_StageCameraDesc.CameraDesc.vAxisY.z = vAxisY["z"].GetFloat();
	m_StageCameraDesc.CameraDesc.vAxisY.w = vAxisY["w"].GetFloat();

	m_StageCameraDesc.CameraDesc.fAspect = CameraDesc["fAspect"].GetFloat();
	m_StageCameraDesc.CameraDesc.fFovy = CameraDesc["fFovy"].GetFloat();
	m_StageCameraDesc.CameraDesc.fNear = CameraDesc["fNear"].GetFloat();
	m_StageCameraDesc.CameraDesc.fFar = CameraDesc["fFar"].GetFloat();

	const Value& TransformDesc = CameraDesc["TransformDesc"];

	m_StageCameraDesc.CameraDesc.TransformDesc.RotationPerSec = TransformDesc["RotationPerSec"].GetFloat();
	m_StageCameraDesc.CameraDesc.TransformDesc.SpeedPerSec = TransformDesc["SpeedPerSec"].GetFloat();

	return true;
}


HRESULT CStageCamera::Add_Components()
{
	return S_OK;
}

CStageCamera* CStageCamera::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CStageCamera* pInstance = new CStageCamera(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created CStageCamera");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CStageCamera::Clone(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath)
{
	CStageCamera* pInstance = new CStageCamera(*this);

	if (FAILED(pInstance->Initialize(pLayerTag, iLevelIndex, filePath)))
	{
		MSG_BOX("Failed to Cloned CStageCamera");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CStageCamera::Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	CStageCamera* pInstance = new CStageCamera(*this);

	if (FAILED(pInstance->Initialize(pLayerTag, iLevelIndex, pArg)))
	{
		MSG_BOX("Failed to Cloned CStageCamera");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStageCamera::Free()
{
	__super::Free();
}
