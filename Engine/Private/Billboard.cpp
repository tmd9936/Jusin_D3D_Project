#include "Billboard.h"
#include "GameObject.h"
#include "PipeLine.h"

CBillboard::CBillboard(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CGameObject* pOwner)
	: CComponent(pDevice, pContext, pOwner)
{
}

CBillboard::CBillboard(const CBillboard& rhs, CGameObject* pOwner)
	: CComponent(rhs, pOwner)
{
}


HRESULT CBillboard::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBillboard::Initialize(void* pArg)
{
	return S_OK;
}

_uint CBillboard::Late_Tick(_double TimeDelta)
{
	CTransform* pOwnerTransform = m_pOwner->Get_As<CTransform>();
	if (nullptr == pOwnerTransform)
		return 0;

	_float4x4 matWorld, matView, matBill, matScale, matScaleInv;
	_float3 vScale = pOwnerTransform->Get_Scaled();
	XMStoreFloat4x4(&matBill, XMMatrixIdentity());

	XMStoreFloat4x4(&matWorld, pOwnerTransform->Get_WorldMatrix_Matrix());

	XMStoreFloat4x4(&matView, CPipeLine::GetInstance()->Get_Transform_Matrix(CPipeLine::D3DTS_VIEW));

	matBill._11 = matView._11;
	matBill._13 = matView._13;
	matBill._31 = matView._31;
	matBill._33 = matView._33;

	XMStoreFloat4x4(&matScale, XMMatrixScaling(vScale.x, vScale.y, vScale.z));
	XMStoreFloat4x4(&matScaleInv, XMMatrixInverse(0, XMLoadFloat4x4(&matScale)));

	XMStoreFloat4x4(&matBill, XMMatrixInverse(0, XMLoadFloat4x4(&matBill)));
	pOwnerTransform->Set_Matrix(XMLoadFloat4x4(&matScale) * XMLoadFloat4x4(&matBill) * XMLoadFloat4x4(&matScaleInv) * XMLoadFloat4x4(&matWorld));
	
	return _uint();
}

CBillboard* CBillboard::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CBillboard* pInstance = new CBillboard(pDevice, pContext, nullptr);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CBillboard");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CBillboard::Clone(CGameObject* pOwner, void* pArg)
{
	CBillboard* pInstance = new CBillboard(*this, pOwner);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CBillboard");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBillboard::Free()
{
	__super::Free();
}
