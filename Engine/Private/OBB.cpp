#include "OBB.h"

#include "DebugDraw.h"

COBB::COBB(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CCollider(pDevice, pContext)
{
}

COBB::COBB(const COBB& rhs)
    : CCollider(rhs)
{

}

HRESULT COBB::Initialize_Prototype()
{
    m_eType = TYPE_OBB;

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

    m_isCollision = false;

    m_eState = COLLISION_STATE_NONE;
}

void COBB::Draw(_vector vColor)
{
#ifdef _DEBUG
    DX::Draw(m_pBatch, *m_pOBB, vColor);
#endif
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
    m_pOBB_Original->Extents = _float3(m_Collider_Desc.vScale.x * 0.5f, m_Collider_Desc.vScale.y * 0.5f, m_Collider_Desc.vScale.z * 0.5f);
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

}