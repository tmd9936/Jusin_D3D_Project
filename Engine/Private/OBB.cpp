#include "OBB.h"

#include "DebugDraw.h"

COBB::COBB(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CCollider(pDevice, pContext)
{
}

COBB::COBB(const COBB& rhs)
    : CCollider(rhs)
    , m_pOBB_Original(nullptr == rhs.m_pOBB_Original ? rhs.m_pOBB_Original : new BoundingOrientedBox(*rhs.m_pOBB_Original))
{
}

HRESULT COBB::Initialize_Prototype()
{
    m_pOBB_Original = new BoundingOrientedBox(_float3(0.f, 0.f, 0.f), _float3(0.5f, 0.5f, 0.5f), _float4(0.0f, 0.f, 0.f, 1.f));

    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT COBB::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    return S_OK;
}

void COBB::Tick(_fmatrix TransformMatrix)
{
    m_pOBB_Original->Transform(*m_pOBB, TransformMatrix);
}

void COBB::Draw(_vector vColor)
{
    DX::Draw(m_pBatch, *m_pOBB, vColor);
}

void COBB::Set_TransformMatrix()
{
    _matrix ScaleMatrix, RotationXMatrix, RotationYMatrix, RotationZMatrix, TranslationMatrix;

    ScaleMatrix = XMMatrixScaling(m_Collider_Desc.vScale.x, m_Collider_Desc.vScale.y, m_Collider_Desc.vScale.z);
    RotationXMatrix = XMMatrixRotationX(m_Collider_Desc.vRotation.x);
    RotationYMatrix = XMMatrixRotationY(m_Collider_Desc.vRotation.y);
    RotationZMatrix = XMMatrixRotationZ(m_Collider_Desc.vRotation.z);
    TranslationMatrix = XMMatrixTranslation(m_Collider_Desc.vPosition.x, m_Collider_Desc.vPosition.y, m_Collider_Desc.vPosition.z);

    m_pOBB_Original->Transform(*m_pOBB_Original, ScaleMatrix * RotationXMatrix * RotationYMatrix * RotationZMatrix * TranslationMatrix);
    m_pOBB = new BoundingOrientedBox(*m_pOBB_Original);
}


COBB* COBB::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    COBB* pInstance = new COBB(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : COBB");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CComponent* COBB::Clone(CGameObject* pOwner, void* pArg)
{
    CCollider* pInstance = new COBB(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : COBB");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void COBB::Free()
{
    __super::Free();
    Safe_Delete(m_pOBB);
}