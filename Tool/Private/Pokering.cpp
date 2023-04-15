#include "stdafx.h"
#include "Pokering.h"
#include "Monster.h"

#include "GameInstance.h"

#include "Bone.h"

CPokering::CPokering(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{

}

CPokering::CPokering(const CPokering& rhs)
	: CGameObject(rhs)
{

}

HRESULT CPokering::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPokering::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	if (nullptr != pArg)
	{
		m_desc.pParent = (*(POKERING_DESC*)(pArg)).pParent;
		m_desc.pBonePtr = (*(POKERING_DESC*)(pArg)).pBonePtr;
		m_desc.pParentTransform = (*(POKERING_DESC*)(pArg)).pParentTransform;
		m_desc.PivotMatrix = (*(POKERING_DESC*)(pArg)).PivotMatrix;

		lstrcpy(m_desc.modelPrototypeTag, (*(POKERING_DESC*)(pArg)).modelPrototypeTag);
	}

	if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	Safe_AddRef(m_desc.pParent);
	Safe_AddRef(m_desc.pBonePtr);
	Safe_AddRef(m_desc.pParentTransform);

	m_eRenderId = RENDER_NONBLEND;

	m_IdleStateTimeAcc = m_pModelCom->Get_Animation_Duration(0) - 1;

	return S_OK;
}

_uint CPokering::Tick(_double TimeDelta)
{
	State_Tick(TimeDelta);

	State_Change();

	return _uint();
}

_uint CPokering::LateTick(_double TimeDelta)
{
	if (nullptr != m_desc.pParent)
	{
		_matrix		ParentMatrix = m_desc.pBonePtr->Get_OffsetMatrix() *
			m_desc.pBonePtr->Get_CombinedTransformationMatrix() * XMLoadFloat4x4(&m_desc.PivotMatrix);

		REMOVE_SCALE(ParentMatrix)

		XMStoreFloat4x4(&m_FinalWorldMatrix, m_pTransformCom->Get_WorldMatrix_Matrix() * ParentMatrix * m_desc.pParentTransform->Get_WorldMatrix_Matrix());
	}

	m_pRendererCom->Add_RenderGroup(m_eRenderId, this);

	return _uint();
}

HRESULT CPokering::Render()
{
	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		//if (FAILED(m_pModelCom->SetUp_ShaderResource(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
		//	return E_FAIL;

		if (FAILED(m_pModelCom->SetUp_BoneMatrices(m_pShaderCom, "g_BoneMatrices", i)))
			return E_FAIL;

		if (m_eCurState == STATE_COOLTIME_END)
			m_pShaderCom->Begin(1);
		else
			m_pShaderCom->Begin(0);


		m_pModelCom->Render(i);
	}


	return S_OK;
}

void CPokering::State_Change()
{
	if (m_eCurState != m_ePreState)
	{
		switch (m_eCurState)
		{
		case STATE_IDLE:
			m_pModelCom->Set_StartTimeAcc(0, m_IdleStateTimeAcc);
			break;
		case STATE_COOLTIME:
			m_pModelCom->Set_StartTimeAcc(0, 0.0);
			break;
		case STATE_COOLTIME_END:
			m_pModelCom->Set_StartTimeAcc(0, m_IdleStateTimeAcc);
			m_CoolTimeEndTimeAcc = 0.0;
			break;
		}

		m_ePreState = m_eCurState;
	}
}

void CPokering::State_Tick(const _double& TimeDelta)
{
	switch (m_eCurState)
	{
	case STATE_IDLE:
		m_pModelCom->Play_Animation(TimeDelta, false);
		if (false == m_desc.pParent->Get_CanSkillAttack())
		{
			m_eCurState = STATE_COOLTIME;
			m_CoolTimeSpeed = 100.0 / (m_desc.pParent->Get_SkillCoomTime() * 60.0);
		}
		break;
	case STATE_COOLTIME:
		// 60 * coolTIme / 100 => coolTime * 0.6;
		if (m_pModelCom->Play_Animation(TimeDelta * m_CoolTimeSpeed))
		{
			m_eCurState = STATE_COOLTIME_END;
		}
		break;
	case STATE_COOLTIME_END:
		m_pModelCom->Play_Animation(TimeDelta, false);
		CoolTimeEnd_Check(TimeDelta);
		break;

	}
}

void CPokering::CoolTimeEnd_Check(const _double& TimeDelta)
{
	if (m_CoolTimeEndTimeAcc >= m_CoolTimeEndTime)
	{
		m_eCurState = STATE_IDLE;
	}

	if (m_CoolTimeEndTimeAcc < m_CoolTimeEndTime)
		m_CoolTimeEndTimeAcc += TimeDelta;
}


HRESULT CPokering::Add_Components()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc = { 10.f, XMConvertToRadians(90.0f) };
	if (FAILED(pGameInstance->Add_Component(CTransform::familyId, this, LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		(CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(pGameInstance->Add_Component(CRenderer::familyId, this, LEVEL_STATIC, TEXT("Prototype_Component_Renderer"),
		(CComponent**)&m_pRendererCom, nullptr)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Component(CShader::familyId, this, LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimModelColor"),
		(CComponent**)&m_pShaderCom, nullptr)))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(pGameInstance->Add_Component(CModel::familyId, this, LEVEL_STATIC, m_desc.modelPrototypeTag,
		(CComponent**)&m_pModelCom, nullptr)))
		return E_FAIL;


	return S_OK;
}


HRESULT CPokering::SetUp_ShaderResources()
{
	if (FAILED(m_pShaderCom->Set_Matrix("g_WorldMatrix", &m_FinalWorldMatrix)))
		return E_FAIL;

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(m_pShaderCom->Set_Matrix("g_ViewMatrix",
		&pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_Matrix("g_ProjMatrix",
		&pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_vColor",
		&m_CoolTimeEndColor, sizeof(_float4))))
		return E_FAIL;

	_float cameraFar = pGameInstance->Get_CameraFar();
	if (FAILED(m_pShaderCom->Set_RawValue("g_CameraFar",
		&cameraFar, sizeof(_float))))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

CPokering* CPokering::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPokering* pInstance = new CPokering(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created CPokering");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CPokering::Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	CPokering* pInstance = new CPokering(*this);

	if (FAILED(pInstance->Initialize(pLayerTag, iLevelIndex, pArg)))
	{
		MSG_BOX("Failed to Cloned CPokering");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CPokering::Free()
{
	__super::Free();

	Safe_Release(m_desc.pParent);
	Safe_Release(m_desc.pBonePtr);
	Safe_Release(m_desc.pParentTransform);

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pRendererCom);

}
