#pragma once
#include "Collider.h"

BEGIN(Engine)

class ENGINE_DLL COBB : public CCollider
{
private:
	COBB(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	COBB(const COBB& rhs);
	~COBB() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_fmatrix TransformMatrix);

public:
	virtual void Draw(_vector vColor) override;
	virtual void Set_TransformMatrix() override;

public:
	static COBB* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(CGameObject* pOwner, void* pArg) override;
	virtual void Free() override;
};

END