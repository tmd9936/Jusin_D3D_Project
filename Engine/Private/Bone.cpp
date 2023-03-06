#include "..\Public\Bone.h"

CBone::CBone()
{
}

HRESULT CBone::Initialize(aiNode* pAINode)
{
	strcpy_s(m_szName, pAINode->mName.data);

	return S_OK;
}

CBone* CBone::Create(aiNode* pAINode)
{
	CBone* pInstance = new CBone();

	if (FAILED(pInstance->Initialize(pAINode)))
	{
		MSG_BOX("Failed to Created : CBone");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBone::Free()
{
	__super::Free();
}
