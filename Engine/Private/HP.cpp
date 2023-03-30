#include "HP.h"
#include "GameObject.h"
#include "PipeLine.h"

CHP::CHP(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CGameObject* pOwner)
	: CComponent(pDevice, pContext, pOwner)
{
}

CHP::CHP(const CHP& rhs, CGameObject* pOwner)
	: CComponent(rhs, pOwner)
{
}


HRESULT CHP::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CHP::Initialize(void* pArg)
{
	return S_OK;
}

_uint CHP::Late_Tick(_double TimeDelta)
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

_uint CHP::Late_Tick(_float4x4& customMatrix)
{
	CTransform* pOwnerTransform = m_pOwner->Get_As<CTransform>();
	if (nullptr == pOwnerTransform)
		return 0;


	_float4x4 matView, matBill, matScale, matScaleInv;;
	XMStoreFloat4x4(&matBill, XMMatrixIdentity());

	_float3 vScale = pOwnerTransform->Get_Scaled();

	XMStoreFloat4x4(&matView, CPipeLine::GetInstance()->Get_Transform_Matrix(CPipeLine::D3DTS_VIEW));

	matBill._11 = matView._11;
	matBill._13 = matView._13;
	matBill._31 = matView._31;
	matBill._33 = matView._33;

	XMStoreFloat4x4(&matScale, XMMatrixScaling(vScale.x, vScale.y, vScale.z));
	XMStoreFloat4x4(&matScaleInv, XMMatrixInverse(0, XMLoadFloat4x4(&matScale)));

	XMStoreFloat4x4(&matBill, XMMatrixInverse(0, XMLoadFloat4x4(&matBill)));
	XMStoreFloat4x4(&customMatrix, XMLoadFloat4x4(&matScale) * XMLoadFloat4x4(&matBill) * XMLoadFloat4x4(&matScaleInv) * XMLoadFloat4x4(&customMatrix));

	return _uint();
}

_matrix CHP::Get_BillBoarMatrix()
{
	_float4x4 matView, matBill;
	XMStoreFloat4x4(&matBill, XMMatrixIdentity());

	XMStoreFloat4x4(&matView, CPipeLine::GetInstance()->Get_Transform_Matrix(CPipeLine::D3DTS_VIEW));

	matBill._11 = matView._11;
	matBill._13 = matView._13;
	matBill._31 = matView._31;
	matBill._33 = matView._33;

	XMStoreFloat4x4(&matBill, XMMatrixInverse(0, XMLoadFloat4x4(&matBill)));
	return XMLoadFloat4x4(&matBill);
}

CHP* CHP::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CHP* pInstance = new CHP(pDevice, pContext, nullptr);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CHP");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CHP::Clone(CGameObject* pOwner, void* pArg)
{
	CHP* pInstance = new CHP(*this, pOwner);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CHP");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CHP::Free()
{
	__super::Free();
}
