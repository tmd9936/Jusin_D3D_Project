#pragma once
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CCollider abstract :
    public CComponent
{
public:
    static const FamilyId familyId = FAMILY_ID_COLLISION;

public:
    typedef struct tagCollider_Desc
    {
        _float3		vScale;
        _float3		vRotation;
        _float3		vPosition;
    }   COLLIDER_DESC;

protected:
    CCollider(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CCollider(const CCollider& rhs);
    ~CCollider() = default;

#ifdef _DEBUG
public:
    HRESULT Render();
#endif // _DEBUG

public:
    virtual HRESULT Initialize_Prototype();
    virtual HRESULT Initialize(void* pArg) override;
    virtual void Tick(_fmatrix TransformMatrix);

    virtual void Draw(_vector vColor) PURE;

    virtual void Set_TransformMatrix() PURE;

protected:
    _float4x4		m_TransformationMatrix = {};
    _bool			m_isCollision = { false };

    COLLIDER_DESC   m_Collider_Desc = {};

#ifdef _DEBUG
protected:
    PrimitiveBatch<DirectX::VertexPositionColor>* m_pBatch = { nullptr };
    BasicEffect* m_pEffect = { nullptr };
    ID3D11InputLayout* m_pInputLayout = { nullptr };
#endif // _DEBUG

protected:
    _matrix Remove_Rotation(_fmatrix TranformMatrix);

public:
    virtual void Free() override;
};

END

