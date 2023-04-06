#include "Frustum.h"

#include "PipeLine.h"

IMPLEMENT_SINGLETON(CFrustum)

CFrustum::CFrustum()
{
}

HRESULT CFrustum::Initialize()
{
	/* ���������̽� ���� ����ü �������� �����Ѵ�. */
	m_vPoints_Original[0] = _float3(-1.f, 1.f, 0.f);
	m_vPoints_Original[1] = _float3(1.f, 1.f, 0.f);
	m_vPoints_Original[2] = _float3(1.f, -1.f, 0.f);
	m_vPoints_Original[3] = _float3(-1.f, -1.f, 0.f);

	m_vPoints_Original[4] = _float3(-1.f, 1.f, 1.f);
	m_vPoints_Original[5] = _float3(1.f, 1.f, 1.f);
	m_vPoints_Original[6] = _float3(1.f, -1.f, 1.f);
	m_vPoints_Original[7] = _float3(-1.f, -1.f, 1.f);

	return S_OK;
}

void CFrustum::Update()
{
	CPipeLine* pPipeLine = CPipeLine::GetInstance();
	Safe_AddRef(pPipeLine);

	_matrix		ProjMatrixInverse = pPipeLine->Get_Transform_Matrix_Inverse(CPipeLine::D3DTS_PROJ);
	_matrix		ViewMatrixInverse = pPipeLine->Get_Transform_Matrix_Inverse(CPipeLine::D3DTS_VIEW);

	/* �佺���̽� */
	for (_uint i = 0; i < 8; ++i)
	{
		XMStoreFloat3(&m_vPoints[i],
			XMVector3TransformCoord(XMLoadFloat3(&m_vPoints_Original[i]), ProjMatrixInverse));
	}

	/* ���彺���̽� */
	for (_uint i = 0; i < 8; ++i)
	{
		XMStoreFloat3(&m_vPoints[i],
			XMVector3TransformCoord(XMLoadFloat3(&m_vPoints[i]), ViewMatrixInverse));
	}

	Safe_Release(pPipeLine);

	if (FAILED(Make_Plane(m_vPoints, m_Planes_World)))
		return;
}

_bool CFrustum::Is_In(_fvector vPosition, _float fRange)
{
	for (_uint i = 0; i < 6; ++i)
	{
		if (fRange < XMVectorGetX(XMPlaneDotCoord(XMLoadFloat4(&m_Planes_World[i]), vPosition)))
			return false;
	}

	return true;
}

HRESULT CFrustum::Make_Plane(const _float3* pPoints, _float4* pPlanes)
{
	/*+x*/
	XMStoreFloat4(&pPlanes[0], XMPlaneFromPoints(XMLoadFloat3(&pPoints[1]),
		XMLoadFloat3(&pPoints[5]),
		XMLoadFloat3(&pPoints[6])));

	/*-x*/
	XMStoreFloat4(&pPlanes[1], XMPlaneFromPoints(XMLoadFloat3(&pPoints[4]),
		XMLoadFloat3(&pPoints[0]),
		XMLoadFloat3(&pPoints[3])));

	/*+y*/
	XMStoreFloat4(&pPlanes[2], XMPlaneFromPoints(XMLoadFloat3(&pPoints[4]),
		XMLoadFloat3(&pPoints[5]),
		XMLoadFloat3(&pPoints[1])));

	/*-y*/
	XMStoreFloat4(&pPlanes[3], XMPlaneFromPoints(XMLoadFloat3(&pPoints[3]),
		XMLoadFloat3(&pPoints[2]),
		XMLoadFloat3(&pPoints[6])));

	/*+z*/
	XMStoreFloat4(&pPlanes[4], XMPlaneFromPoints(XMLoadFloat3(&pPoints[5]),
		XMLoadFloat3(&pPoints[4]),
		XMLoadFloat3(&pPoints[7])));

	/*-z*/
	XMStoreFloat4(&pPlanes[5], XMPlaneFromPoints(XMLoadFloat3(&pPoints[0]),
		XMLoadFloat3(&pPoints[1]),
		XMLoadFloat3(&pPoints[2])));

	return S_OK;
}

void CFrustum::Free()
{
}
