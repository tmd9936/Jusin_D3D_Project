#include "stdafx.h"
#include "Weapon.h"

#include "GameInstance.h"
#include "Bone.h"

CWeapon::CWeapon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CGameObject(pDevice, pContext)
{
}

CWeapon::CWeapon(const CWeapon& rhs)
    : CGameObject(rhs)
{
}

HRESULT CWeapon::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CWeapon::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
    if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, pArg)))
        return E_FAIL;

    if (FAILED(Add_Components()))
        return E_FAIL;

    m_pTransformCom->Set_Scaled(_float3(1.0f, 1.0f, 1.0f));
    m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(90.0f));
    //m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(0.0f, 0.f, 0.f, 1.f));

    memcpy(&m_WeaponDesc, pArg, sizeof m_WeaponDesc);

    m_eRenderId = RENDER_NONBLEND;

    return S_OK;
}

_uint CWeapon::Tick(_double TimeDelta)
{
    return _uint();
}

_uint CWeapon::LateTick(_double TimeDelta)
{
    _matrix		ParentMatrix = m_WeaponDesc.pBonePtr->Get_OffsetMatrix() *
        m_WeaponDesc.pBonePtr->Get_CombinedTransformationMatrix() * XMLoadFloat4x4(&m_WeaponDesc.PivotMatrix);

    XMStoreFloat4x4(&m_FinalWorldMatrix, m_pTransformCom->Get_WorldMatrix_Matrix() * Remove_Scale(ParentMatrix) * m_WeaponDesc.pParent->Get_WorldMatrix_Matrix());

    m_pAABB->Tick(XMLoadFloat4x4(&m_FinalWorldMatrix));

    m_pRendererCom->Add_RenderGroup(m_eRenderId, this);

    return _uint();
}

HRESULT CWeapon::Render()
{
    if (FAILED(SetUp_ShaderResources()))
        return E_FAIL;

    _uint		iNumMeshes = m_pModelCom->Get_NumMeshes();

    for (_uint i = 0; i < iNumMeshes; ++i)
    {
        if (FAILED(m_pModelCom->SetUp_ShaderResource(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
            return E_FAIL;

        m_pShaderCom->Begin(0);

        m_pModelCom->Render(i);
    }

#ifdef _DEBUG
    m_pAABB->Render();
#endif // _DEBUG
}

HRESULT CWeapon::Add_Components()
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
    if (FAILED(pGameInstance->Add_Component(CModel::familyId, this, LEVEL_BASECAMP, TEXT("Prototype_Component_Model_BaseCamp_Stove"),
        (CComponent**)&m_pModelCom, nullptr)))
        return E_FAIL;

    /* For.Com_Shader */
    if (FAILED(pGameInstance->Add_Component(CShader::familyId, this, LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxModelColor"),
        (CComponent**)&m_pShaderCom, nullptr)))
        return E_FAIL;

    /* For.Com_AABB*/
    CCollider::COLLIDER_DESC		ColliderDesc;

    ZeroMemory(&ColliderDesc, sizeof ColliderDesc);
    ColliderDesc.vScale = _float3(1.f, 2.f, 1.f);
    ColliderDesc.vPosition = _float3(0.0f, ColliderDesc.vScale.y * 0.5f, 0.f);
    if (FAILED(pGameInstance->Add_Component(CCollider::familyId, this, LEVEL_STATIC, TEXT("Prototype_Component_Collider_AABB"),
        (CComponent**)&m_pAABB, &ColliderDesc)))
        return E_FAIL;

    return S_OK;
}

HRESULT CWeapon::SetUp_ShaderResources()
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

_matrix CWeapon::Remove_Scale(_fmatrix Matrix)
{
    _matrix		Result = Matrix;

    Result.r[0] = XMVector3Normalize(Result.r[0]);
    Result.r[1] = XMVector3Normalize(Result.r[1]);
    Result.r[2] = XMVector3Normalize(Result.r[2]);

    return Result;
}

CWeapon* CWeapon::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CWeapon* pInstance = new CWeapon(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created CWeapon");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CWeapon::Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
    CWeapon* pInstance = new CWeapon(*this);

    if (FAILED(pInstance->Initialize(pLayerTag, iLevelIndex, pArg)))
    {
        MSG_BOX("Failed to Cloned CWeapon");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CWeapon::Free()
{
    __super::Free();

    Safe_Release(m_pAABB);

    Safe_Release(m_WeaponDesc.pBonePtr);
    Safe_Release(m_WeaponDesc.pParent);

    Safe_Release(m_pTransformCom);
    Safe_Release(m_pShaderCom);
    Safe_Release(m_pModelCom);
    Safe_Release(m_pRendererCom);
}
