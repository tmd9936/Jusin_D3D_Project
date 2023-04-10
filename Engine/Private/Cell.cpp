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

	_vector vLine[LINE_END];

	vLine[LINE_AB] = XMLoadFloat3(&m_vPoints[POINT_B]) - XMLoadFloat3(&m_vPoints[POINT_A]);
	m_vNormal[LINE_AB] = _float3(XMVectorGetZ(vLine[LINE_AB]) * -1.f, 0.f, XMVectorGetX(vLine[LINE_AB]));

	vLine[LINE_BC] = XMLoadFloat3(&m_vPoints[POINT_C]) - XMLoadFloat3(&m_vPoints[POINT_B]);
	m_vNormal[LINE_BC] = _float3(XMVectorGetZ(vLine[LINE_BC]) * -1.f, 0.f, XMVectorGetX(vLine[LINE_BC]));

	vLine[LINE_CA] = XMLoadFloat3(&m_vPoints[POINT_A]) - XMLoadFloat3(&m_vPoints[POINT_C]);
	m_vNormal[LINE_CA] = _float3(XMVectorGetZ(vLine[LINE_CA]) * -1.f, 0.f, XMVectorGetX(vLine[LINE_CA]));

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

_bool CCell::isIn(_fvector vPosition, _int& iNeighborIndex)
{
	for (_uint i = 0; i < LINE_END; ++i)
	{
		_vector		vDir = vPosition - XMLoadFloat3(&m_vPoints[i]);

		if (0 < XMVectorGetX(XMVector3Dot(XMVector3Normalize(vDir), XMVector3Normalize(XMLoadFloat3(&m_vNormal[i])))))
		{
			iNeighborIndex = m_iNeighborIndices[i];
			return false;
		}
	}

	return true;
}

_vector CCell::Get_SlidePower(_fvector vPosition, _fvector vLook)
{
	_vector		vDir = vPosition - XMLoadFloat3(&m_vPoints[0]);
	_uint		minIndex = 0;
	_float		minLength = XMVectorGetX(XMVector3Length(XMVector3Normalize(vDir)));

	_uint		maxIndex = 0;
	_float		maxLength = XMVectorGetX(XMVector3Length(XMVector3Normalize(vDir)));

	_uint		midIndex = 0;

	for (_uint i = 1; i < POINT_END; ++i)
	{
		vDir = vPosition - XMLoadFloat3(&m_vPoints[i]);

		_float compareLength = XMVectorGetX(XMVector3Length(XMVector3Normalize(vDir)));
		if (minLength > compareLength)
		{
			minLength = compareLength;
			minIndex = i;
		}

		if (maxLength < compareLength)
		{
			maxLength = compareLength;
			maxIndex = i;
		}
	}


	if (POINT_A != maxIndex && POINT_A != minIndex)
	{
		midIndex = POINT_A;
	}
	else if (POINT_B != maxIndex && POINT_B != minIndex)
	{
		midIndex = POINT_B;
	}
	else
	{
		midIndex = POINT_C;
	}
	
	
	_float vMinRad = acosf(XMVectorGetX(XMVector3Dot(XMVector3Normalize(vPosition - XMLoadFloat3(&m_vPoints[minIndex])), XMVector3Normalize(vLook))));
	_float vMidRad = acosf(XMVectorGetX(XMVector3Dot(XMVector3Normalize(vPosition - XMLoadFloat3(&m_vPoints[midIndex])), XMVector3Normalize(vLook))));

	if (vMinRad > vMidRad)
		return  XMVector3Normalize(XMLoadFloat3(&m_vPoints[minIndex]) - XMLoadFloat3(&m_vPoints[midIndex]));
	else
		return  XMVector3Normalize(XMLoadFloat3(&m_vPoints[midIndex]) - XMLoadFloat3(&m_vPoints[minIndex]));
}

void CCell::Compute_Height(_float3& vPosition, _float& fY)
{
	_vector		Plane{};

	Plane = XMPlaneFromPoints(
		XMLoadFloat3(&m_vPoints[POINT_A]),
		XMLoadFloat3(&m_vPoints[POINT_B]),
		XMLoadFloat3(&m_vPoints[POINT_C]));

	_float4 resultPlane{};

	XMStoreFloat4(&resultPlane, Plane);
	if (resultPlane.y > 0.f)
	{
		fY = (-resultPlane.x * vPosition.x - resultPlane.z * vPosition.z - resultPlane.w) / resultPlane.y;
	}
	
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
