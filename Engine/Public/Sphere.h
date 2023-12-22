#pragma once
#include "Collider.h"

BEGIN(Engine)

class ENGINE_DLL CSphere : public CCollider
{
private:
	explicit CSphere(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CSphere(const CSphere& rhs);
	virtual ~CSphere() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_fmatrix TransformMatrix);

public:
	// CCollider��(��) ���� ��ӵ�
	virtual void Draw(_vector vColor) override;
	virtual void Set_TransformMatrix() override;
	
public:
	static CSphere* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(CGameObject* pOwner, void* pArg) override;
	virtual void Free() override;
};

END