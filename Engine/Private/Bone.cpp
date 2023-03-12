#include "..\Public\Bone.h"

CBone::CBone()
{
}

CBone::CBone(const CBone& rhs)
	: m_pParent(rhs.m_pParent)
	, m_TransformationMatrix(rhs.m_TransformationMatrix)
	, m_CombinedTransformationMatrix(rhs.m_CombinedTransformationMatrix)
{
	strcpy_s(m_szName, rhs.m_szName);

}

HRESULT CBone::Initialize(aiNode* pAINode, CBone* pParent)
{
	strcpy_s(m_szName, pAINode->mName.data);

	m_pParent = pParent;

	Safe_AddRef(m_pParent);

	return S_OK;
}

void CBone::SetUp_TransformationMatrix(_fmatrix Matrix)
{
	XMStoreFloat4x4(&m_TransformationMatrix, Matrix);
}

void CBone::SetUp_CombinedTransformationMatrix()
{
	if (nullptr == m_pParent)
	{
		XMStoreFloat4x4(&m_CombinedTransformationMatrix,
			XMLoadFloat4x4(&m_TransformationMatrix));

		return;
	}

	XMStoreFloat4x4(&m_CombinedTransformationMatrix,
		XMLoadFloat4x4(&m_TransformationMatrix) *
		XMLoadFloat4x4(&m_pParent->m_CombinedTransformationMatrix));
}

CBone* CBone::Create(aiNode* pAINode, CBone* pParent)
{
	CBone* pInstance = new CBone();

	if (FAILED(pInstance->Initialize(pAINode, pParent)))
	{
		MSG_BOX("Failed to Created : CBone");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBone::Free()
{
	__super::Free();

	Safe_Release(m_pParent);
}
