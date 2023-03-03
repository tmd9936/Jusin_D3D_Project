#include "..\Public\Model.h"
#include "Mesh.h"

CModel::CModel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CGameObject* pOwner)
	: CComponent(pDevice, pContext, pOwner)
{
}

CModel::CModel(const CModel& rhs, CGameObject* pOwner)
	: CComponent(rhs, pOwner)
	, m_Meshes(rhs.m_Meshes)
	, m_eType(rhs.m_eType)
{
	for (auto& pMesh : m_Meshes)
		Safe_AddRef(pMesh);
}

HRESULT CModel::Initialize_Prototype(TYPE eType, const char* pModelFilePath)
{
	_uint		iFlag = 0;

	m_eType = eType;

	/* aiProcess_PreTransformVertices : 정점정보를 읽어서 저장할 당시에 필요한 변환을 미리 처리해놓는다. */
	/* 처리? 이 모델의 뼈들 중, 메시의 이름과 같은 이름을 가진 뼈를 찾아서 그 뼈의 행렬을 정점들에게 미리 적용한다. */
	if (TYPE_NONANIM == m_eType)
		iFlag = aiProcess_PreTransformVertices | aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_ConvertToLeftHanded;
	else
		iFlag = aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_ConvertToLeftHanded;

	m_pAIScene = m_Importer.ReadFile(pModelFilePath, iFlag);
	if (nullptr == m_pAIScene)
		return E_FAIL;

	/* 모델을 구성하는 메시들을 생성한다.(버텍스, 인덱스버퍼를 생성하는 것이다.) */
	if (FAILED(Ready_Meshes()))
		return E_FAIL;

	return S_OK;
}

HRESULT CModel::Initialize(void* pArg)
{
	return S_OK;
}

HRESULT CModel::Render()
{
	for (auto& pMesh : m_Meshes)
	{
		if (nullptr != pMesh)
			pMesh->Render();
	}

	return S_OK;
}

HRESULT CModel::Ready_Meshes()
{
	m_iNumMeshes = m_pAIScene->mNumMeshes;

	for (_uint i = 0; i < m_iNumMeshes; ++i)
	{
		CMesh* pMesh = CMesh::Create(m_pDevice, m_pContext, m_eType, m_pAIScene->mMeshes[i]);
		if (nullptr == pMesh)
			return E_FAIL;

		m_Meshes.push_back(pMesh);
	}


	return S_OK;
}

CModel* CModel::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, TYPE eType, const char* pModelFilePath)
{
	CModel* pInstance = new CModel(pDevice, pContext, nullptr);

	if (FAILED(pInstance->Initialize_Prototype(eType, pModelFilePath)))
	{
		MSG_BOX("Failed to Created : CModel");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CModel::Clone(CGameObject* pOwner, void* pArg)
{
	CModel* pInstance = new CModel(*this, pOwner);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CModel");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CModel::Free()
{
	__super::Free();

	for (auto& pMesh : m_Meshes)
		Safe_Release(pMesh);

	m_Meshes.clear();
}
