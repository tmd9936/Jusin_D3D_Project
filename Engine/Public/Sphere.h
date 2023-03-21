#pragma once
#include "Collider.h"

BEGIN(Engine)

class ENGINE_DLL CSphere : public CCollider
{
private:
	CSphere(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CSphere(const CSphere& rhs);
	~CSphere() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_fmatrix TransformMatrix);

public:
	// CCollider을(를) 통해 상속됨
	virtual void Draw(_vector vColor) override;
	virtual void Set_TransformMatrix() override;

private:
	BoundingBox* m_pAABB = { nullptr };

public:
	static CSphere* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(CGameObject* pOwner, void* pArg) override;
	virtual void Free() override;
};

END