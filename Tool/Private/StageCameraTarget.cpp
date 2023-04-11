#include "stdafx.h"
#include "StageCameraTarget.h"

#include "GameInstance.h"

CStageCameraTarget::CStageCameraTarget(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{
}

CStageCameraTarget::CStageCameraTarget(const CStageCameraTarget& rhs)
	: CGameObject(rhs)
{
}

HRESULT CStageCameraTarget::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CStageCameraTarget::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	CTransform* pPlayerTransform = Get_PlayerTransform(L"Player1");
	if (nullptr == pPlayerTransform)
		return E_FAIL;
	
	m_Desc.m_distanceCheckMinLength = 2.5f;

	_float3 playerPos = {}; 
	XMStoreFloat3(&playerPos, pPlayerTransform->Get_State(CTransform::STATE_POSITION));
	m_pTransformCom->Set_Pos(playerPos.x, playerPos.y, playerPos.z);

	m_pTransformCom->Set_Scaled({ 0.5f, 0.5f, 0.5f });

	m_eRenderId = RENDER_NONBLEND;

	return S_OK;
}

_uint CStageCameraTarget::Tick(_double TimeDelta)
{
	switch (m_eCurrentState)
	{
	case STATE_FORMATION:
		Formation_State_Tick(TimeDelta);
		break;
	case STATE_STOP:
		break;
	}

	Change_State();
	return _uint();
}

_uint CStageCameraTarget::LateTick(_double TimeDelta)
{
	m_pRendererCom->Add_RenderGroup(m_eRenderId, this);

	return _uint();
}

HRESULT CStageCameraTarget::Render()
{
	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		if (FAILED(m_pModelCom->SetUp_ShaderResource(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
			return E_FAIL;
		/*if (FAILED(m_pModelCom->SetUp_ShaderResource(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS)))
			return E_FAIL;*/

		m_pShaderCom->Begin(0);

		m_pModelCom->Render(i);
	}

	return S_OK;
}

void CStageCameraTarget::Change_State()
{
	if (m_eCurrentState != m_ePreState)
	{
		switch (m_eCurrentState)
		{
		case STATE_FORMATION:
			break;
		case STATE_STOP:
			break;
		}

		m_ePreState = m_eCurrentState;
	}
}

void CStageCameraTarget::Formation_State_Tick(const _double& TimeDelta)
{
	_vector vArrivePos = {};
	_vector vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	CTransform* pPlayer1Transform = Get_PlayerTransform(L"Player1");
	if (nullptr == pPlayer1Transform)
		return;

	vArrivePos = pPlayer1Transform->Get_State(CTransform::STATE_POSITION);

	m_CurMaxDistanceFromPlayer = XMVectorGetX(XMVector4Length(vArrivePos - vPos));

	CTransform* pPlayer2Transform = Get_PlayerTransform(L"Player2");
	if (nullptr != pPlayer2Transform)
	{
		vArrivePos = (vArrivePos + pPlayer2Transform->Get_State(CTransform::STATE_POSITION)) * 0.5f;
		_float distanceFromPlayer2	= XMVectorGetX(XMVector4Length(
			pPlayer2Transform->Get_State(CTransform::STATE_POSITION) - vPos));

		if (distanceFromPlayer2 > m_CurMaxDistanceFromPlayer)
		{
			m_CurMaxDistanceFromPlayer = distanceFromPlayer2;
		}
	}

	CTransform* pPlayer3Transform = Get_PlayerTransform(L"Player3");
	if (nullptr != pPlayer3Transform)
	{
		vArrivePos = (vArrivePos + pPlayer3Transform->Get_State(CTransform::STATE_POSITION)) * 0.5f;
		_float distanceFromPlayer3 = XMVectorGetX(XMVector4Length(
			pPlayer3Transform->Get_State(CTransform::STATE_POSITION) - vPos));

		if (distanceFromPlayer3 > m_CurMaxDistanceFromPlayer)
		{
			m_CurMaxDistanceFromPlayer = distanceFromPlayer3;
		}
	}

	if (m_Desc.m_distanceCheckMinLength < m_CurMaxDistanceFromPlayer)
	{
		if (m_CurMaxDistanceFromPlayer > m_PreMaxDistanceFromPlayer)
		{
			m_eMoveState = MOVE_STATE_WIDEN;
		}
		else if (m_CurMaxDistanceFromPlayer < m_PreMaxDistanceFromPlayer)
		{
			m_eMoveState = MOVE_STATE_NARROW;
		}
		else
		{
			m_eMoveState = MOVE_STATE_NARROW;
		}
	}

	m_PreMaxDistanceFromPlayer = m_CurMaxDistanceFromPlayer;

	m_pTransformCom->ChaseNoLook(vArrivePos, (_float)TimeDelta, 0.4f);
}

CTransform* CStageCameraTarget::Get_PlayerTransform(const _tchar* pObjectTag)
{
	CGameObject* pPlyaer = CGameInstance::GetInstance()->Get_Object(LEVEL_STAGE, L"Layer_Player", pObjectTag);
	if (nullptr == pPlyaer)
		return nullptr;

	return pPlyaer->Get_As<CTransform>();
}

HRESULT CStageCameraTarget::Add_Components()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	/* For.Com_Transform */ /* 타켓 속도 1.5 이상되면 순간이동하는 느낌으로 움직여서 이상해짐, 몬스터 속도랑 맞춰야 할듯*/
	CTransform::TRANSFORMDESC		TransformDesc = { 1.5f, XMConvertToRadians(90.0f) };
	if (FAILED(pGameInstance->Add_Component(CTransform::familyId, this, LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		(CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(pGameInstance->Add_Component(CRenderer::familyId, this, LEVEL_STATIC, TEXT("Prototype_Component_Renderer"),
		(CComponent**)&m_pRendererCom, nullptr)))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(pGameInstance->Add_Component(CModel::familyId, this, LEVEL_STATIC, TEXT("Prototype_Component_Model_BaseCamp_Stove"),
		(CComponent**)&m_pModelCom, nullptr)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(pGameInstance->Add_Component(CShader::familyId, this, LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxModelColor"),
		(CComponent**)&m_pShaderCom, nullptr)))
		return E_FAIL;

	return S_OK;
}

HRESULT CStageCameraTarget::SetUp_ShaderResources()
{
	if (FAILED(m_pShaderCom->Set_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldMatrix())))
		return E_FAIL;

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(m_pShaderCom->Set_Matrix("g_ViewMatrix",
		&pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_Matrix("g_ProjMatrix",
		&pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_vCamPosition",
		&pGameInstance->Get_CamPosition(), sizeof(_float4))))
		return E_FAIL;

	const LIGHTDESC* pLightDesc = pGameInstance->Get_Light(0);
	if (nullptr == pLightDesc)
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_vLightDir",
		&pLightDesc->vDirection, sizeof(_float4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_vLightDiffuse",
		&pLightDesc->vDiffuse, sizeof(_float4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_vLightAmbient",
		&pLightDesc->vAmbient, sizeof(_float4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_vLightSpecular",
		&pLightDesc->vSpecular, sizeof(_float4))))
		return E_FAIL;

	Safe_Release(pGameInstance);
	return S_OK;
}

CStageCameraTarget* CStageCameraTarget::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CStageCameraTarget* pInstance = new CStageCameraTarget(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created CStageCameraTarget");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CStageCameraTarget::Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	CStageCameraTarget* pInstance = new CStageCameraTarget(*this);

	if (FAILED(pInstance->Initialize(pLayerTag, iLevelIndex, pArg)))
	{
		MSG_BOX("Failed to Cloned CStageCameraTarget");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStageCameraTarget::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pRendererCom);
}
