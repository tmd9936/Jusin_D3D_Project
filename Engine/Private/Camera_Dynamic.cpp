#include "Camera_Dynamic.h"

#include "GameInstance.h"

CCamera_Dynamic::CCamera_Dynamic(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CCamera(pDevice, pContext)
{
}

CCamera_Dynamic::CCamera_Dynamic(const CCamera_Dynamic& rhs)
	: CCamera(rhs)
{
}

HRESULT CCamera_Dynamic::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCamera_Dynamic::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	memcpy(&m_CameraDynamicDesc, pArg, sizeof m_CameraDynamicDesc);

	if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, &m_CameraDynamicDesc.CameraDesc)))
		return E_FAIL;

	return S_OK;
}

_uint CCamera_Dynamic::Tick(_double TimeDelta)
{
	Key_Input(TimeDelta);

	return __super::Tick(TimeDelta);
}

_uint CCamera_Dynamic::LateTick(_double TimeDelta)
{
	return __super::LateTick(TimeDelta);
}

HRESULT CCamera_Dynamic::Render()
{
	return S_OK;
}

void CCamera_Dynamic::Key_Input(const _double TimeDelta)
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

HRESULT CCamera_Dynamic::Add_Components()
{

	return S_OK;
}

CCamera_Dynamic* CCamera_Dynamic::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CCamera_Dynamic* pInstance = new CCamera_Dynamic(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created CCamera_Dynamic");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CCamera_Dynamic::Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	CCamera_Dynamic* pInstance = new CCamera_Dynamic(*this);

	if (FAILED(pInstance->Initialize(pLayerTag, iLevelIndex, pArg)))
	{
		MSG_BOX("Failed to Cloned CCamera_Dynamic");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera_Dynamic::Free()
{
	__super::Free();
}
