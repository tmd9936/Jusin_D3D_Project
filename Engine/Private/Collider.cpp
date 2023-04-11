#include "Collider.h"

#include "DebugDraw.h"
#include "PipeLine.h"
#include "GameObject.h"

_uint CCollider::g_iColliderID = 0;

CCollider::CCollider(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CComponent(pDevice, pContext)
	, m_iID(g_iColliderID++)
{
}

#ifdef _DEBUG
CCollider::CCollider(const CCollider& rhs)
	: CComponent(rhs)
	, m_iID(g_iColliderID++)
	, m_pBatch(rhs.m_pBatch)
	, m_pEffect(rhs.m_pEffect)
	, m_pInputLayout(rhs.m_pInputLayout)
	, m_eType(rhs.m_eType)
	, m_pSphere_Original(nullptr == rhs.m_pSphere_Original ? rhs.m_pSphere_Original : new BoundingSphere(*rhs.m_pSphere_Original))
	, m_pAABB_Original(nullptr == rhs.m_pAABB_Original ? rhs.m_pAABB_Original : new BoundingBox(*rhs.m_pAABB_Original))
	, m_pOBB_Original(nullptr == rhs.m_pOBB_Original ? rhs.m_pOBB_Original : new BoundingOrientedBox(*rhs.m_pOBB_Original))
{
	Safe_AddRef(m_pInputLayout);
}
#else
CCollider::CCollider(const CCollider& rhs)
	: CComponent(rhs)
	, m_eType(rhs.m_eType)
	, m_pSphere_Original(nullptr == rhs.m_pSphere_Original ? rhs.m_pSphere_Original : new BoundingSphere(*rhs.m_pSphere_Original))
	, m_pAABB_Original(nullptr == rhs.m_pAABB_Original ? rhs.m_pAABB_Original : new BoundingBox(*rhs.m_pAABB_Original))
	, m_pOBB_Original(nullptr == rhs.m_pOBB_Original ? rhs.m_pOBB_Original : new BoundingOrientedBox(*rhs.m_pOBB_Original))
{

}
#endif // _DEBUG

#ifdef _DEBUG
HRESULT CCollider::Render()
{
	m_pEffect->SetWorld(XMMatrixIdentity());

	CPipeLine* pPipeLine = CPipeLine::GetInstance();
	Safe_AddRef(pPipeLine);

	m_pEffect->SetView(pPipeLine->Get_Transform_Matrix(CPipeLine::D3DTS_VIEW));
	m_pEffect->SetProjection(pPipeLine->Get_Transform_Matrix(CPipeLine::D3DTS_PROJ));

	Safe_Release(pPipeLine);

	m_pContext->IASetInputLayout(m_pInputLayout);

	m_pContext->GSSetShader(nullptr, nullptr, 0);

	m_pEffect->Apply(m_pContext);

	m_pBatch->Begin();

	_vector		vColor = true == m_isCollision ? XMVectorSet(1.f, 0.f, 0.f, 1.f) : XMVectorSet(0.f, 1.f, 0.f, 1.f);
	
	Draw(vColor);

	m_pBatch->End();

	return S_OK;
}
#endif // _DEBUG

HRESULT CCollider::Initialize_Prototype()
{
	if (m_eType == TYPE_END)
		return E_FAIL;

	switch (m_eType)
	{
	case TYPE_AABB:
		m_pAABB_Original = new BoundingBox(_float3(0.f, 0.f, 0.f), _float3(0.5f, 0.5f, 0.5f));
		break;
	case TYPE_OBB:
		m_pOBB_Original = new BoundingOrientedBox(_float3(0.f, 0.f, 0.f), _float3(0.5f, 0.5f, 0.5f), _float4(0.0f, 0.f, 0.f, 1.f));

		break;
	case TYPE_SPHERE:
		m_pSphere_Original = new BoundingSphere(_float3(0.f, 0.f, 0.f), 0.5f);
		break;
	}

#ifdef _DEBUG
	m_pBatch = new PrimitiveBatch<DirectX::VertexPositionColor>(m_pContext);
	if (nullptr == m_pBatch)
		return E_FAIL;

	m_pEffect = new BasicEffect(m_pDevice);
	if (nullptr == m_pEffect)
		return E_FAIL;

	m_pEffect->SetVertexColorEnabled(true);

	const void* pShaderByteCode = { nullptr };
	size_t			iShaderByteCodeLength = { 0 };

	m_pEffect->GetVertexShaderBytecode(&pShaderByteCode, &iShaderByteCodeLength);

	if (FAILED(m_pDevice->CreateInputLayout(DirectX::VertexPositionColor::InputElements,
		DirectX::VertexPositionColor::InputElementCount,
		pShaderByteCode, iShaderByteCodeLength, &m_pInputLayout)))
		return E_FAIL;

#endif // _DEBUG

	return S_OK;
}

HRESULT CCollider::Initialize(void* pArg)
{
	m_Collider_Desc = *(COLLIDER_DESC*)pArg;

	//_matrix ScaleMatrix, RotationXMatrix, RotationYMatrix, RotationZMatrix, TranslationMatrix;

	//ScaleMatrix = XMMatrixScaling(m_Collider_Desc.vScale.x, m_Collider_Desc.vScale.y, m_Collider_Desc.vScale.z);
	//RotationXMatrix = XMMatrixRotationX(m_Collider_Desc.vRotation.x);
	//RotationYMatrix = XMMatrixRotationY(m_Collider_Desc.vRotation.y);
	//RotationZMatrix = XMMatrixRotationZ(m_Collider_Desc.vRotation.z);
	//TranslationMatrix = XMMatrixTranslation(m_Collider_Desc.vPosition.x, m_Collider_Desc.vPosition.y, m_Collider_Desc.vPosition.z);
	//
	//XMStoreFloat4x4(&m_TransformationMatrix, ScaleMatrix * RotationXMatrix * TranslationMatrix);

	Set_TransformMatrix();

	return S_OK;
}

void CCollider::Tick(_fmatrix TransformMatrix)
{

}

void CCollider::On_Collision(CCollider* pOther, const _float& fX, const _float& fY, const _float& fZ)
{
	m_pOwner->On_Collision(pOther, fX, fY, fZ);
	m_eState = COLLISION_STATE_ON;
}

void CCollider::On_CollisionEnter(CCollider* pOther, const _float& fX, const _float& fY, const _float& fZ)
{
	m_pOwner->On_CollisionEnter(pOther, fX, fY, fZ);
	m_eState = COLLISION_STATE_ENTER;
}

void CCollider::On_CollisionExit(CCollider* pOther, const _float& fX, const _float& fY, const _float& fZ)
{
	m_pOwner->On_CollisionExit(pOther, fX, fY, fZ);
	m_eState = COLLISION_STATE_EXIT;
}

const _vector CCollider::Get_Center() const
{
	switch (m_eType)
	{
	case TYPE_AABB:
		return XMLoadFloat3(&m_pAABB->Center);
		break;
	case TYPE_OBB:
		return XMLoadFloat3(&m_pOBB->Center);
		break;
	case TYPE_SPHERE:
		return XMLoadFloat3(&m_pSphere->Center);
		break;
	}

	return _vector();
}

_bool CCollider::Collision(CCollider* pTarget)
{
	if (TYPE_AABB == m_eType)
	{
		if (TYPE_AABB == pTarget->m_eType)
			m_isCollision = m_pAABB->Intersects(*pTarget->m_pAABB);
		else if (TYPE_OBB == pTarget->m_eType)
			m_isCollision = m_pAABB->Intersects(*pTarget->m_pOBB);
		else if (TYPE_SPHERE == pTarget->m_eType)
			m_isCollision = m_pAABB->Intersects(*pTarget->m_pSphere);
	}


	else if (TYPE_OBB == m_eType)
	{
		if (TYPE_AABB == pTarget->m_eType)
			m_isCollision = m_pOBB->Intersects(*pTarget->m_pAABB);
		else if (TYPE_OBB == pTarget->m_eType)
			m_isCollision = m_pOBB->Intersects(*pTarget->m_pOBB);
		else if (TYPE_SPHERE == pTarget->m_eType)
			m_isCollision = m_pOBB->Intersects(*pTarget->m_pSphere);
	}


	else if (TYPE_SPHERE == m_eType)
	{
		if (TYPE_AABB == pTarget->m_eType)
			m_isCollision = m_pSphere->Intersects(*pTarget->m_pAABB);
		else if (TYPE_OBB == pTarget->m_eType)
			m_isCollision = m_pSphere->Intersects(*pTarget->m_pOBB);
		else if (TYPE_SPHERE == pTarget->m_eType)
			m_isCollision = m_pSphere->Intersects(*pTarget->m_pSphere);
	}

	if (true == m_isCollision)
		pTarget->m_isCollision = true;

	return m_isCollision;
}

_matrix CCollider::Remove_Rotation(_fmatrix TranformMatrix)
{
	_matrix		Matrix = TranformMatrix;

	Matrix.r[0] = XMVectorSet(1.f, 0.f, 0.f, 0.f) * XMVectorGetX(XMVector3Length(Matrix.r[0]));
	Matrix.r[1] = XMVectorSet(0.f, 1.f, 0.f, 0.f) * XMVectorGetX(XMVector3Length(Matrix.r[1]));
	Matrix.r[2] = XMVectorSet(0.f, 0.f, 1.f, 0.f) * XMVectorGetX(XMVector3Length(Matrix.r[2]));

	return Matrix;
}

void CCollider::Free()
{
	__super::Free();

#ifdef _DEBUG
	if (false == m_bClone)
	{
		Safe_Delete(m_pBatch);
		Safe_Delete(m_pEffect);
	}
	Safe_Release(m_pInputLayout);
#endif // _DEBUG

	Safe_Delete(m_pSphere_Original);
	Safe_Delete(m_pAABB_Original);
	Safe_Delete(m_pOBB_Original);

	Safe_Delete(m_pSphere);
	Safe_Delete(m_pAABB);
	Safe_Delete(m_pOBB);

}
