#include "Sphere.h"

#include "DebugDraw.h"

CSphere::CSphere(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CCollider(pDevice, pContext)
{
}

CSphere::CSphere(const CSphere& rhs)
    : CCollider(rhs)
    , m_pAABB(nullptr == rhs.m_pAABB ? rhs.m_pAABB : new BoundingBox(*rhs.m_pAABB))
{
}

HRESULT CSphere::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    m_pAABB = new BoundingBox(_float3(0.f, 0.f, 0.f), _float3(0.5f, 0.5f, 0.5f));

    return S_OK;
}

HRESULT CSphere::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    return S_OK;
}

void CSphere::Tick(_fmatrix TransformMatrix)
{
    m_pAABB->Transform(*m_pAABB, XMLoadFloat4x4(&m_TransformationMatrix) * Remove_Rotation(TransformMatrix));
}

void CSphere::Draw(_vector vColor)
{
    DX::Draw(m_pBatch, *m_pAABB, vColor);
}

void CSphere::Set_TransformMatrix()
{
    _matrix ScaleMatrix, TranslationMatrix;

    ScaleMatrix = XMMatrixScaling(m_Collider_Desc.vScale.x, m_Collider_Desc.vScale.y, m_Collider_Desc.vScale.z);
    TranslationMatrix = XMMatrixTranslation(m_Collider_Desc.vPosition.x, m_Collider_Desc.vPosition.y, m_Collider_Desc.vPosition.z);

    XMStoreFloat4x4(&m_TransformationMatrix, ScaleMatrix * TranslationMatrix);

    m_pAABB->Transform(*m_pAABB, XMLoadFloat4x4(&m_TransformationMatrix));
}


CSphere* CSphere::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CSphere* pInstance = new CSphere(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CSphere");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CComponent* CSphere::Clone(CGameObject* pOwner, void* pArg)
{
    CCollider* pInstance = new CSphere(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CSphere");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CSphere::Free()
{
    __super::Free();
    Safe_Delete(m_pAABB);
}