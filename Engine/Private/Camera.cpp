#include "Camera.h"
#include "PipeLine.h"

CCamera::CCamera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
	, m_pPipeLine(CPipeLine::GetInstance())
{
	Safe_AddRef(m_pPipeLine);
}

CCamera::CCamera(const CCamera& rhs)
	: CGameObject(rhs)
	, m_pPipeLine(rhs.m_pPipeLine)
{
	Safe_AddRef(m_pPipeLine);
}

HRESULT CCamera::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCamera::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, pArg)))
		return E_FAIL;

	if (pArg != nullptr)
		memcpy(&m_CameraDesc, pArg, sizeof m_CameraDesc);

	m_pTransform = CTransform::Create(m_pDevice, m_pContext);

	if (m_pTransform == nullptr)
		return E_FAIL;

	m_pTransform->Set_TransforDesc(m_CameraDesc.TransformDesc);
	m_pTransform->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&m_CameraDesc.vEye));
	m_pTransform->LookAt(XMLoadFloat4(&m_CameraDesc.vAt));

	return S_OK;
}

_uint CCamera::Tick(_double TimeDelta)
{
	if (nullptr != m_pPipeLine)
	{
		m_pPipeLine->Set_Transform(CPipeLine::D3DTS_VIEW, m_pTransform->Get_WorldMatrix_Inverse());

		m_pPipeLine->Set_Transform(CPipeLine::D3DTS_PROJ, 
			XMMatrixPerspectiveFovLH(m_CameraDesc.fFovy, m_CameraDesc.fAspect, m_CameraDesc.fNear, m_CameraDesc.fFar));

		m_pPipeLine->Update();
	}

	return __super::Tick(TimeDelta);
}

_uint CCamera::LateTick(_double TimeDelta)
{
	return __super::LateTick(TimeDelta);
}

HRESULT CCamera::Add_Components()
{
	return S_OK;
}


void CCamera::Free()
{
	__super::Free();

	Safe_Release(m_pPipeLine);
	Safe_Release(m_pTransform);
}
