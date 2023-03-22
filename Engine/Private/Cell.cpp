#include "..\Public\Cell.h"
#include "VIBuffer_Cell.h"

CCell::CCell(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice(pDevice)
	, m_pContext(pContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

HRESULT CCell::Initialize(const _float3* pPoints, _uint iIndex)
{
	memcpy(m_vPoints, pPoints, sizeof(_float3) * POINT_END);

	m_iIndex = iIndex;

#ifdef _DEBUG
	m_pVIBuffer = CVIBuffer_Cell::Create(m_pDevice, m_pContext, m_vPoints);
	if (nullptr == m_pVIBuffer)
		return E_FAIL;
#endif // _DEBUG

	return S_OK;
}

_bool CCell::Compare_Points(const _float3* pSour, const _float3* pDest)
{
	if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_A]), XMLoadFloat3(pSour)))
	{
		if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_B]), XMLoadFloat3(pDest)))
			return true;
		if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_C]), XMLoadFloat3(pDest)))
			return true;
	}

	if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_B]), XMLoadFloat3(pSour)))
	{
		if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_C]), XMLoadFloat3(pDest)))
			return true;
		if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_A]), XMLoadFloat3(pDest)))
			return true;
	}

	if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_C]), XMLoadFloat3(pSour)))
	{
		if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_A]), XMLoadFloat3(pDest)))
			return true;
		if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_B]), XMLoadFloat3(pDest)))
			return true;
	}

	return false;
}

#ifdef _DEBUG
HRESULT CCell::Render()
{
	if (nullptr == m_pVIBuffer)
		return E_FAIL;

	return m_pVIBuffer->Render();
}
#endif // _DEBUG

CCell* CCell::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _float3* pPoints, _uint iIndex)
{
	CCell* pInstance = new CCell(pDevice, pContext);

	if (FAILED(pInstance->Initialize(pPoints, iIndex)))
	{
		MSG_BOX("Failed to Created : CCell");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCell::Free()
{
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);

#ifdef _DEBUG
	Safe_Release(m_pVIBuffer);
#endif // _DEBUG
}
