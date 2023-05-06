#pragma once
#include "Component.h"

BEGIN(Engine)

class CGameObject;

class ENGINE_DLL CCollider abstract :
    public CComponent
{
public:
    static const FamilyId familyId = FAMILY_ID_COLLISION;

public:
    enum TYPE { TYPE_SPHERE, TYPE_AABB, TYPE_OBB, TYPE_END };

public:
    enum COLLISION_STATE
    {
        COLLISION_STATE_NONE,
        COLLISION_STATE_ENTER,
        COLLISION_STATE_ON,
        COLLISION_STATE_EXIT,
        COLLISION_STATE_END,
    };

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

#ifdef DEBUG_COMPONENT_RENDER
public:
    virtual HRESULT Render();
#endif // _DEBUG

public:
    virtual HRESULT Initialize_Prototype();
    virtual HRESULT Initialize(void* pArg) override;
    virtual void Tick(_fmatrix TransformMatrix);

public:
    void On_Collision(CCollider* pOther, const _float& fX, const _float& fY, const _float& fZ); // call on collising
    void On_CollisionEnter(CCollider* pOther, const _float& fX, const _float& fY, const _float& fZ);
    void On_CollisionExit(CCollider* pOther, const _float& fX, const _float& fY, const _float& fZ);


public:
    _uint Get_ID() { 
        return m_iID; 
    }
    const _vector Get_Center() const;


public:
    virtual void Draw(_vector vColor) PURE;
    virtual void Set_TransformMatrix() PURE;
    _bool Collision(CCollider* pTarget);

public:
    const _bool     Get_CanCollision() const {
        return m_bCanCollision;
    }

    void            Set_CanCollision(_bool bCanCollision) {
        m_bCanCollision = bCanCollision;
    }

public:
    const TYPE   Get_Type() const {
        return m_eType;
    }

    const BoundingBox* Get_AABB() const {
        return m_pAABB;
    }

    const BoundingOrientedBox* Get_OBB() const {
        return m_pOBB;
    }

    const BoundingSphere* Get_Sphere() const {
        return m_pSphere;
    }

    const COLLISION_STATE Get_Collision_State() const {
        return m_eState;
    }

protected:
    _matrix Remove_Rotation(_fmatrix TranformMatrix);


protected:
    BoundingBox* m_pAABB_Original = { nullptr };
    BoundingBox* m_pAABB = { nullptr };

protected:
    BoundingOrientedBox* m_pOBB_Original = { nullptr };
    BoundingOrientedBox* m_pOBB = { nullptr };

protected:
    BoundingSphere* m_pSphere_Original = { nullptr };
    BoundingSphere* m_pSphere = { nullptr };

protected:
    _bool			m_isCollision = { false };

    COLLIDER_DESC   m_Collider_Desc = {};
    TYPE	        m_eType = { TYPE_END };

    _bool           m_bCanCollision = { true };

protected:
    COLLISION_STATE m_eState = { COLLISION_STATE_END };

protected:
    static _uint g_iColliderID;
    _uint m_iID = { 0 };

#ifdef DEBUG_COMPONENT_RENDER
protected:
    PrimitiveBatch<DirectX::VertexPositionColor>* m_pBatch = { nullptr };
    BasicEffect* m_pEffect = { nullptr };
    ID3D11InputLayout* m_pInputLayout = { nullptr };
#endif // _DEBUG

public:
    virtual void Free() override;
};

END

