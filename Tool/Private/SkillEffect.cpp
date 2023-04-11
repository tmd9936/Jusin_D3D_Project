#include "stdafx.h"
#include "SkillEffect.h"

#include "GameInstance.h"

#include "Loader.h"
#include "Bone.h"

CSkillEffect::CSkillEffect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{

}

CSkillEffect::CSkillEffect(const CSkillEffect& rhs)
	: CGameObject(rhs)
{

}

HRESULT CSkillEffect::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSkillEffect::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	if (pArg)
	{
		m_EffectDesc.m_actionType = (*(EFFECT_DESC*)(pArg)).m_actionType;
		m_EffectDesc.m_effectPath = (*(EFFECT_DESC*)(pArg)).m_effectPath;
		m_EffectDesc.m_ProtoTypeTag = (*(EFFECT_DESC*)(pArg)).m_ProtoTypeTag;

		m_EffectDesc.m_effectType = (*(EFFECT_DESC*)(pArg)).m_effectType;
		m_EffectDesc.m_exPath1 = (*(EFFECT_DESC*)(pArg)).m_exPath1;
		m_EffectDesc.m_exPath2 = (*(EFFECT_DESC*)(pArg)).m_exPath2;
		m_EffectDesc.m_soundEventID = (*(EFFECT_DESC*)(pArg)).m_soundEventID;
		m_EffectDesc.m_soundEventTag = (*(EFFECT_DESC*)(pArg)).m_soundEventTag;
		m_EffectDesc.m_underFlag = (*(EFFECT_DESC*)(pArg)).m_underFlag;

		m_EffectDesc.m_AnimationLoopTime = (*(EFFECT_DESC*)(pArg)).m_AnimationLoopTime;
		m_EffectDesc.m_AnimationStartAcc = (*(EFFECT_DESC*)(pArg)).m_AnimationStartAcc;

		m_EffectDesc.m_AnimationLoop = (*(EFFECT_DESC*)(pArg)).m_AnimationLoop;

		m_EffectDesc.m_AnimationSpeed = (*(EFFECT_DESC*)(pArg)).m_AnimationSpeed;
		m_EffectDesc.m_CurrentLoopCount = (*(EFFECT_DESC*)(pArg)).m_CurrentLoopCount;
		m_EffectDesc.m_IsParts = (*(EFFECT_DESC*)(pArg)).m_IsParts;

	}


	if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	m_eRenderId = RENDER_BLEND;

	_float3 vPos{};
	XMStoreFloat3(&vPos, m_pTransformCom->Get_State(CTransform::STATE_POSITION));

	m_pNavigationCom->Set_Index_By_Position({ vPos.x, vPos.y, vPos.z });

	m_pModelCom->Set_Animation(0);

	return S_OK;
}


_uint CSkillEffect::Tick(_double TimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	if (m_EffectDesc.m_CurrentLoopCount < 0)
		return OBJ_DEAD;

	Loop_Count_Check(TimeDelta);

	return _uint();
}

_uint CSkillEffect::LateTick(_double TimeDelta)
{
	if (nullptr != m_EffectDesc.pBonePtr && nullptr != m_EffectDesc.pParent)
	{
		_matrix		ParentMatrix = m_EffectDesc.pBonePtr->Get_OffsetMatrix() *
			m_EffectDesc.pBonePtr->Get_CombinedTransformationMatrix() * XMLoadFloat4x4(&m_EffectDesc.PivotMatrix);

		REMOVE_SCALE(ParentMatrix)

			if (m_EffectDesc.m_bParentRotateApply)
				XMStoreFloat4x4(&m_EffectDesc.m_FinalWorldMatrix, m_pTransformCom->Get_WorldMatrix_Matrix() * ParentMatrix * m_EffectDesc.pParent->Get_WorldMatrix_Matrix());
			else
				XMStoreFloat4x4(&m_EffectDesc.m_FinalWorldMatrix, m_pTransformCom->Get_WorldMatrix_Matrix() * m_EffectDesc.pParent->Get_Position_Matrix());

		m_pNavigationCom->Set_Index_By_Position({ m_EffectDesc.m_FinalWorldMatrix.m[3][0],
			m_EffectDesc.m_FinalWorldMatrix.m[3][1], m_EffectDesc.m_FinalWorldMatrix.m[3][2] });
	}
	else
	{
		_vector vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		m_pNavigationCom->Set_Index_By_Position({ XMVectorGetX(vPos), XMVectorGetY(vPos), XMVectorGetZ(vPos) });
	}

	m_pRendererCom->Add_RenderGroup(m_eRenderId, this);


#ifdef _DEBUG


	m_pNavigationCom->Render();

#endif // _DEBUG

	return _uint();
}

HRESULT CSkillEffect::Render()
{
	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		if (FAILED(m_pModelCom->SetUp_ShaderResource(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
			return E_FAIL;

		if (FAILED(m_pModelCom->SetUp_BoneMatrices(m_pShaderCom, "g_BoneMatrices", i)))
			return E_FAIL;

		m_pShaderCom->Begin(0);

		m_pModelCom->Render(i);
	}

	return S_OK;
}

void CSkillEffect::Set_Pos(const _float4& vPos)
{
	if (m_pTransformCom)
	{
		if (m_EffectDesc.m_effectPath.find(L"E_BD") != wstring::npos)
		{
			m_pTransformCom->Set_Pos(vPos.x, -0.2f, vPos.z);
		}

		else
		{
			m_pTransformCom->Set_Pos(vPos.x, vPos.y, vPos.z);
		}
	}
}

void CSkillEffect::Set_Parent(CBone* pBoneParent, CTransform* pTransformParent, _float4x4 PivotMatrix)
{
	if (nullptr == pBoneParent || nullptr == pTransformParent)
		return;

	m_EffectDesc.PivotMatrix = PivotMatrix;

	Set_ParentBone(pBoneParent);
	Set_ParentTransform(pTransformParent);

	m_EffectDesc.m_IsParts = true;
}

void CSkillEffect::Set_Parent(CBone* pBoneParent, CTransform* pTransformParent, _matrix PivotMatrix)
{
	if (nullptr == pBoneParent || nullptr == pTransformParent)
		return;

	XMStoreFloat4x4(&m_EffectDesc.PivotMatrix, PivotMatrix);

	Set_ParentBone(pBoneParent);
	Set_ParentTransform(pTransformParent);

	m_EffectDesc.m_IsParts = true;
}

void CSkillEffect::Set_Parent(CBone* pBoneParent, CTransform* pTransformParent)
{
	if (nullptr == pBoneParent || nullptr == pTransformParent)
		return;

	Set_ParentBone(pBoneParent);
	Set_ParentTransform(pTransformParent);

	m_EffectDesc.m_IsParts = true;
}

void CSkillEffect::Set_ParentNoParts(CBone* pBoneParent, CTransform* pTransformParent, _matrix PivotMatrix)
{
	if (nullptr == pBoneParent || nullptr == pTransformParent)
		return;

	XMStoreFloat4x4(&m_EffectDesc.PivotMatrix, PivotMatrix);

	Set_ParentBone(pBoneParent);
	Set_ParentTransform(pTransformParent);
}

void CSkillEffect::Set_AnimaitonStartTime(_double time)
{
	m_EffectDesc.m_AnimationStartAcc = time;
	if (m_pModelCom)
	{
		m_pModelCom->Set_Animation_Start_Time(m_EffectDesc.m_AnimationStartAcc);
	}
}

void CSkillEffect::Loop_Count_Check(const _double& TimeDelta)
{
	if (m_pModelCom->Play_Animation(TimeDelta * m_EffectDesc.m_AnimationSpeed, m_EffectDesc.m_AnimationLoop))
	{
		m_pModelCom->Set_Animation_Start_Time(m_EffectDesc.m_AnimationStartAcc);

		--m_EffectDesc.m_CurrentLoopCount;
	}
}

HRESULT CSkillEffect::Add_Components()
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

	/* For.Com_Model */
	if (FAILED(pGameInstance->Add_Component(CModel::familyId, this, LEVEL_STATIC, m_EffectDesc.m_ProtoTypeTag.c_str(),
		(CComponent**)&m_pModelCom, nullptr)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(pGameInstance->Add_Component(CShader::familyId, this, LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimModelColor"),
		(CComponent**)&m_pShaderCom, nullptr)))
		return E_FAIL;

	CNavigation::NAVIDESC		NaviDesc;
	ZeroMemory(&NaviDesc, sizeof NaviDesc);
	NaviDesc.iIndex = 1;
	if (FAILED(pGameInstance->Add_Component(CNavigation::familyId, this, m_iLevelindex, TEXT("Prototype_Component_Navigation"),
		(CComponent**)&m_pNavigationCom, &NaviDesc)))
		return E_FAIL;


	return S_OK;
}


HRESULT CSkillEffect::SetUp_ShaderResources()
{
	if (!m_EffectDesc.m_IsParts)
	{
		if (FAILED(m_pShaderCom->Set_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldMatrix())))
			return E_FAIL;
	}
	else
	{
		if (FAILED(m_pShaderCom->Set_Matrix("g_WorldMatrix", &m_EffectDesc.m_FinalWorldMatrix)))
			return E_FAIL;
	}

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

CSkillEffect* CSkillEffect::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CSkillEffect* pInstance = new CSkillEffect(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created CSkillEffect");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSkillEffect::Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	CSkillEffect* pInstance = new CSkillEffect(*this);

	if (FAILED(pInstance->Initialize(pLayerTag, iLevelIndex, pArg)))
	{
		MSG_BOX("Failed to Cloned CSkillEffect");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CSkillEffect::Free()
{
	__super::Free();

	Safe_Release(m_EffectDesc.pBonePtr);

	Safe_Release(m_EffectDesc.pParent);

	Safe_Release(m_pModelCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pNavigationCom);
}
