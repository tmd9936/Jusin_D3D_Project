#pragma once

#include "Base.h"

BEGIN(Engine)

class CPipeLine;
class CCamera;

class CFrustum final : public CBase
{
    DECLARE_SINGLETON(CFrustum)

private:
    explicit CFrustum();
    virtual ~CFrustum() = default;

private:
    static const int PLANE_CNT = 6;

public:
    HRESULT         Initialize();
    void			Update();

public:
    _bool           Is_In(_fvector vPosition, _float fRange);

private:
    HRESULT Make_Plane(const _float3* pPoints, _float4* pPlanes);

private:
    _float3				m_vPoints_Original[8];
    _float3				m_vPoints[8];

    _float4				m_Planes_World[PLANE_CNT];

public:
    virtual void Free() override;

};


END
