#include "PipeLine.h"

IMPLEMENT_SINGLETON(CPipeLine)

CPipeLine::CPipeLine()
{
}

void CPipeLine::Set_Transform(TRANSFORMSTATE eState, _fmatrix TransformMatrix)
{
	XMStoreFloat4x4(&m_TransformStateMatrix[eState], TransformMatrix);
}

_matrix CPipeLine::Get_ViewPort_Matrix(float x, float y, float w, float h, float minZ, float maxZ)
{
	return XMMatrixSet(
		w / 2.f, 0.f, 0.f, 0.f,
		0.f, -h / 2.f, 0.f, 0.f,
		0.f, 0.f, maxZ - minZ, 0.f,
		x + w / 2.f, y + h / 2.f, minZ, 1.f
	);
}

void CPipeLine::Update()
{
	XMStoreFloat4(&m_vCamPosition, XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_TransformStateMatrix[D3DTS_VIEW])).r[3]);
}

void CPipeLine::Free(void)
{
}
