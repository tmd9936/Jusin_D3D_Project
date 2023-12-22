#pragma once
#include "Collider.h"

BEGIN(Engine)

class ENGINE_DLL CAABB : public CCollider
{
private:
	explicit CAABB(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CAABB(const CAABB& rhs);
	virtual ~CAABB() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_fmatrix TransformMatrix);

public:
	// CCollider을(를) 통해 상속됨
	virtual void Draw(_vector vColor) override;
	virtual void Set_TransformMatrix() override;

public:
	static CAABB* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(CGameObject* pOwner, void* pArg) override;
	virtual void Free() override;
};

END