#pragma once
#include "Component.h"

BEGIN(Engine)

class CGameObject;

class ENGINE_DLL CBillboard : public CComponent
{
public:
	static const FamilyId familyId = FAMILY_ID_BILLBOARD;
protected:
	explicit CBillboard(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CGameObject* pOwner);
	explicit CBillboard(const CBillboard& rhs, CGameObject* pOwner);
	virtual ~CBillboard() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;

public:
	_uint Late_Tick(_double TimeDelta);

	_uint Late_Tick(_float4x4& customMatrix);

	_matrix Get_BillBoarMatrix();

public:
	static CBillboard* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(CGameObject* pOwner, void* pArg = nullptr) override;

private:
	virtual void Free();

};

END