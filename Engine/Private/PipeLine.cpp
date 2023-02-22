#include "PipeLine.h"

IMPLEMENT_SINGLETON(CPipeLine)

CPipeLine::CPipeLine()
{
}

void CPipeLine::Set_Transform(TRANSFORMSTATE eState, _fmatrix TransformMatrix)
{
	XMStoreFloat4x4(&m_TransformStateMatrix[eState], TransformMatrix);
}

void CPipeLine::Update()
{
	XMStoreFloat4(&m_vCamPosition, XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_TransformStateMatrix[D3DTS_VIEW])).r[3]);
}

void CPipeLine::Free(void)
{
}
