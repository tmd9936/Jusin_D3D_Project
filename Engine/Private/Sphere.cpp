#include "Sphere.h"

#include "DebugDraw.h"

CSphere::CSphere(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CCollider(pDevice, pContext)
{
}

CSphere::CSphere(const CSphere& rhs)
    : CCollider(rhs)
{
}

HRESULT CSphere::Initialize_Prototype()
{
    m_eType = TYPE_SPHERE;

    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

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
    m_pSphere_Original->Transform(*m_pSphere, TransformMatrix);

    m_isCollision = false;

    m_eState = COLLISION_STATE_NONE;
}

void CSphere::Draw(_vector vColor)
{
#ifdef _DEBUG
    DX::Draw(m_pBatch, *m_pSphere, vColor);
#endif
}

void CSphere::Set_TransformMatrix()
{
    _matrix ScaleMatrix, TranslationMatrix;

    ScaleMatrix = XMMatrixScaling(m_Collider_Desc.vScale.x, m_Collider_Desc.vScale.y, m_Collider_Desc.vScale.z);
    TranslationMatrix = XMMatrixTranslation(m_Collider_Desc.vPosition.x, m_Collider_Desc.vPosition.y, m_Collider_Desc.vPosition.z);

    m_pSphere_Original->Transform(*m_pSphere_Original, ScaleMatrix * TranslationMatrix);
    m_pSphere = new BoundingSphere(*m_pSphere_Original);
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

}
