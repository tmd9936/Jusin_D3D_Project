#include "stdafx.h"
#include "Camera_Public.h"

#include "GameInstance.h"

CCamera_Public::CCamera_Public(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CCamera(pDevice, pContext)
{
}

CCamera_Public::CCamera_Public(const CCamera_Public& rhs)
	: CCamera(rhs)
{
}

HRESULT CCamera_Public::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCamera_Public::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	memcpy(&m_CameraPublicDesc, pArg, sizeof m_CameraPublicDesc);

	if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, &m_CameraPublicDesc.CameraDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CCamera_Public::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath)
{
	if (filePath)
	{
		Load_By_JsonFile(filePath);
		m_strSaveJsonPath = filePath;
	}

	if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, &m_CameraPublicDesc.CameraDesc)))
		return E_FAIL;

	return S_OK;
}

_uint CCamera_Public::Tick(_double TimeDelta)
{
	Key_Input(TimeDelta);

	m_CameraPublicDesc.CameraDesc = m_CameraDesc;

	return __super::Tick(TimeDelta);
}

_uint CCamera_Public::LateTick(_double TimeDelta)
{
	return __super::LateTick(TimeDelta);
}

HRESULT CCamera_Public::Render()
{
	return S_OK;
}

_bool CCamera_Public::Save_Args_Impl(HANDLE hFile)
{
	DWORD   dwByte = 0;
	if (m_pTransform)
	{
		XMStoreFloat4(&m_CameraPublicDesc.CameraDesc.vEye, m_pTransform->Get_State(CTransform::STATE_POSITION));
		XMStoreFloat4(&m_CameraPublicDesc.CameraDesc.vAt, m_pTransform->Get_State(CTransform::STATE_POSITION) + m_pTransform->Get_State(CTransform::STATE_LOOK));

	}

	return WriteFile(hFile, &m_CameraPublicDesc, sizeof(CAMERA_PUBLIC_DESC), &dwByte, NULL);
}

_bool CCamera_Public::Save_By_JsonFile_Impl(Document& doc, Document::AllocatorType& allocator)
{
	if (m_pTransform)
	{
		XMStoreFloat4(&m_CameraPublicDesc.CameraDesc.vEye, m_pTransform->Get_State(CTransform::STATE_POSITION));
		XMStoreFloat4(&m_CameraPublicDesc.CameraDesc.vAt, m_pTransform->Get_State(CTransform::STATE_POSITION) + m_pTransform->Get_State(CTransform::STATE_LOOK));
	}

	Value CameraDesc(kObjectType);
	{
		Value vEye(kObjectType);
		{
			vEye.AddMember("x", m_CameraPublicDesc.CameraDesc.vEye.x, allocator);
			vEye.AddMember("y", m_CameraPublicDesc.CameraDesc.vEye.y, allocator);
			vEye.AddMember("z", m_CameraPublicDesc.CameraDesc.vEye.z, allocator);
			vEye.AddMember("w", m_CameraPublicDesc.CameraDesc.vEye.w, allocator);

		}
		CameraDesc.AddMember("vEye", vEye, allocator);

		Value vAt(kObjectType);
		{
			vAt.AddMember("x", m_CameraPublicDesc.CameraDesc.vAt.x, allocator);
			vAt.AddMember("y", m_CameraPublicDesc.CameraDesc.vAt.y, allocator);
			vAt.AddMember("z", m_CameraPublicDesc.CameraDesc.vAt.z, allocator);
			vAt.AddMember("w", m_CameraPublicDesc.CameraDesc.vAt.w, allocator);

		}
		CameraDesc.AddMember("vAt", vAt, allocator);

		Value vAxisY(kObjectType);
		{
			vAxisY.AddMember("x", m_CameraPublicDesc.CameraDesc.vAxisY.x, allocator);
			vAxisY.AddMember("y", m_CameraPublicDesc.CameraDesc.vAxisY.y, allocator);
			vAxisY.AddMember("z", m_CameraPublicDesc.CameraDesc.vAxisY.z, allocator);
			vAxisY.AddMember("w", m_CameraPublicDesc.CameraDesc.vAxisY.w, allocator);

		}
		CameraDesc.AddMember("vAxisY", vAxisY, allocator);

		CameraDesc.AddMember("fAspect", m_CameraPublicDesc.CameraDesc.fAspect, allocator);
		CameraDesc.AddMember("fFovy", m_CameraPublicDesc.CameraDesc.fFovy, allocator);
		CameraDesc.AddMember("fNear", m_CameraPublicDesc.CameraDesc.fNear, allocator);
		CameraDesc.AddMember("fFar", m_CameraPublicDesc.CameraDesc.fFar, allocator);

		Value TransformDesc(kObjectType);
		{
			TransformDesc.AddMember("RotationPerSec", m_CameraPublicDesc.CameraDesc.TransformDesc.RotationPerSec, allocator);
			TransformDesc.AddMember("SpeedPerSec", m_CameraPublicDesc.CameraDesc.TransformDesc.SpeedPerSec, allocator);
		}
		CameraDesc.AddMember("TransformDesc", TransformDesc, allocator);
	}

	doc.AddMember("CameraDesc", CameraDesc, allocator);

	return true;
}

_bool CCamera_Public::Load_By_JsonFile_Impl(Document& doc)
{
	const Value& CameraDesc = doc["CameraDesc"];

	const Value& vEye = CameraDesc["vEye"];
	m_CameraPublicDesc.CameraDesc.vEye.x = vEye["x"].GetFloat();
	m_CameraPublicDesc.CameraDesc.vEye.y = vEye["y"].GetFloat();
	m_CameraPublicDesc.CameraDesc.vEye.z = vEye["z"].GetFloat();
	m_CameraPublicDesc.CameraDesc.vEye.w = vEye["w"].GetFloat();

	const Value& vAt = CameraDesc["vAt"];
	m_CameraPublicDesc.CameraDesc.vAt.x = vAt["x"].GetFloat();
	m_CameraPublicDesc.CameraDesc.vAt.y = vAt["y"].GetFloat();
	m_CameraPublicDesc.CameraDesc.vAt.z = vAt["z"].GetFloat();
	m_CameraPublicDesc.CameraDesc.vAt.w = vAt["w"].GetFloat();

	const Value& vAxisY = CameraDesc["vAxisY"];
	m_CameraPublicDesc.CameraDesc.vAxisY.x = vAxisY["x"].GetFloat();
	m_CameraPublicDesc.CameraDesc.vAxisY.y = vAxisY["y"].GetFloat();
	m_CameraPublicDesc.CameraDesc.vAxisY.z = vAxisY["z"].GetFloat();
	m_CameraPublicDesc.CameraDesc.vAxisY.w = vAxisY["w"].GetFloat();

	m_CameraPublicDesc.CameraDesc.fAspect = CameraDesc["fAspect"].GetFloat();
	m_CameraPublicDesc.CameraDesc.fFovy = CameraDesc["fFovy"].GetFloat();
	m_CameraPublicDesc.CameraDesc.fNear = CameraDesc["fNear"].GetFloat();
	m_CameraPublicDesc.CameraDesc.fFar = CameraDesc["fFar"].GetFloat();

	const Value& TransformDesc = CameraDesc["TransformDesc"];

	m_CameraPublicDesc.CameraDesc.TransformDesc.RotationPerSec = TransformDesc["RotationPerSec"].GetFloat();
	m_CameraPublicDesc.CameraDesc.TransformDesc.SpeedPerSec = TransformDesc["SpeedPerSec"].GetFloat();


	return true;
}

void CCamera_Public::Key_Input(const _double TimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (KEY_HOLD(KEY::W))
		m_pTransform->Go_Straight(_float(TimeDelta));

	if (KEY_HOLD(KEY::S))
		m_pTransform->Go_Backward(_float(TimeDelta));

	if (KEY_HOLD(KEY::A))
		m_pTransform->Go_Left(_float(TimeDelta));

	if (KEY_HOLD(KEY::D))
		m_pTransform->Go_Right(_float(TimeDelta));

	if (KEY_TAB(KEY::V))
		Save_By_JsonFile("../../Reference/Resources/Data/Scene/BaseCamp/Camera_data.json");

	if (m_bMouseMove)
	{
		_long MouseMove = { 0 };

		if (MouseMove = pGameInstance->Get_MouseMove(CInput_Device::DIMS_X))
		{
			m_pTransform->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), _float(TimeDelta) * (MouseMove * 0.1f));
		}

		if (MouseMove = pGameInstance->Get_MouseMove(CInput_Device::DIMS_Y))
		{
			m_pTransform->Turn(m_pTransform->Get_State(CTransform::STATE_RIGHT), _float(TimeDelta) * (MouseMove * 0.1f));
		}
	}

	if (KEY_TAB(KEY::LSHIFT))
	{
		m_bMouseMove = !m_bMouseMove;
	}


	Safe_Release(pGameInstance);
}

HRESULT CCamera_Public::Add_Components()
{

	return S_OK;
}

CCamera_Public* CCamera_Public::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CCamera_Public* pInstance = new CCamera_Public(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created CCamera_Public");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CCamera_Public::Clone(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath)
{
	CCamera_Public* pInstance = new CCamera_Public(*this);

	if (FAILED(pInstance->Initialize(pLayerTag, iLevelIndex, filePath)))
	{
		MSG_BOX("Failed to Cloned CCamera_Public");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CCamera_Public::Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	CCamera_Public* pInstance = new CCamera_Public(*this);

	if (FAILED(pInstance->Initialize(pLayerTag, iLevelIndex, pArg)))
	{
		MSG_BOX("Failed to Cloned CCamera_Public");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera_Public::Free()
{
	__super::Free();
}
