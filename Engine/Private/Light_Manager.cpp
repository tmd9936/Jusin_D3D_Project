#include "Light_Manager.h"
#include "Light.h"

IMPLEMENT_SINGLETON(CLight_Manager)

CLight_Manager::CLight_Manager()
{
}

void CLight_Manager::Initialize()
{
    _vector	    vLightEye = XMVectorSet(-10.f, 100.f, -10.f, 1.f);
    //_vector	    vLightEye = XMVectorSet(-30.f, 50.f, -30.f, 1.f);
    _vector		vLightAt = XMVectorSet(30.f, 0.f, 30.f, 1.f);
    _vector		vLightUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);

  // _matrix		RotationMatrix = XMMatrixRotationAxis(XMVectorSet(1.f, 0.f, 0.f, 0.f), XMConvertToRadians(-80.f));

    //vLightUp = XMVector3TransformNormal(vLightUp, RotationMatrix);

    XMStoreFloat4x4(&m_shadowDepthLightView, XMMatrixLookAtLH(vLightEye, vLightAt, vLightUp));

    XMStoreFloat4x4(&m_shadowDepthLightProj, XMMatrixPerspectiveFovLH(XMConvertToRadians(30.f), 930.f / 432.f, 0.2f, 200.f));
}

_matrix CLight_Manager::Get_ShadowDepthLightView_Matrix() const
{
    return XMLoadFloat4x4(&m_shadowDepthLightView);
}

_matrix CLight_Manager::Get_ShadowDepthLightProj_Matrix() const
{
    return XMLoadFloat4x4(&m_shadowDepthLightProj);
}

_float4x4 CLight_Manager::Get_ShadowDepthLightView() const
{
    return m_shadowDepthLightView;
}

_float4x4 CLight_Manager::Get_ShadowDepthLightProj() const
{
    return m_shadowDepthLightProj;
}

const LIGHTDESC* CLight_Manager::Get_Light(_uint iIndex)
{
    auto	iter = m_Lights.begin();

    for (_uint i = 0; i < iIndex; ++i)
        ++iter;

    return (*iter)->Get_LightDesc();
}


HRESULT CLight_Manager::Render(CShader* pShader, CVIBuffer_Rect* pVIBuffer)
{
    for (auto& pLight : m_Lights)
    {
        if (nullptr != pLight)
            pLight->Render(pShader, pVIBuffer);
    }

    return E_NOTIMPL;
}

HRESULT CLight_Manager::Add_Light(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const LIGHTDESC& LightDesc)
{
    CLight* pLight = CLight::Create(pDevice, pContext, LightDesc);
    if (nullptr == pLight)
        return E_FAIL;

    m_Lights.push_back(pLight);

    return S_OK;
}

void CLight_Manager::Free()
{
    for (auto& pLight : m_Lights)
        Safe_Release(pLight);

    m_Lights.clear();
}
