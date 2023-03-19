#include "..\Public\Model.h"
#include "Mesh.h"
#include "Texture.h"
#include "Bone.h"
#include "Animation.h"
#include "Shader.h"
#include "Utility.h"
#include "Channel.h"

CModel::CModel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CGameObject* pOwner)
	: CComponent(pDevice, pContext, pOwner)
{
}

CModel::CModel(const CModel& rhs, CGameObject* pOwner)
	: CComponent(rhs, pOwner)
	, m_iNumMeshes(rhs.m_iNumMeshes)
	, m_Meshes(rhs.m_Meshes)
	, m_PivotMatrix(rhs.m_PivotMatrix)
	, m_eType(rhs.m_eType)
	, m_iNumMaterials(rhs.m_iNumMaterials)
	, m_Materials(rhs.m_Materials)
	, m_iNumAnimations(rhs.m_iNumAnimations)
{
	m_Bones.reserve(rhs.m_Bones.size());
	for (auto& pPrototype : rhs.m_Bones)
	{
		CBone* pBone = pPrototype->Clone();
		if (nullptr == pBone)
			return;
		m_Bones.push_back(pBone);
	}

	m_Animations.reserve(rhs.m_Animations.size());
	for (auto& pPrototype : rhs.m_Animations)
	{
		CAnimation* pAnimatione = pPrototype->Clone();
		if (nullptr == pAnimatione)
			return;
		m_Animations.push_back(pAnimatione);
	}

	for (auto& pMesh : m_Meshes)
		Safe_AddRef(pMesh);

	for (auto& Material : m_Materials)
	{
		for (auto& pTexture : Material.pMtrlTexture)
			Safe_AddRef(pTexture);
	}
}

CBone* CModel::Get_BonePtr(const char* pBoneName)
{
	auto	iter = find_if(m_Bones.begin(), m_Bones.end(), [&](CBone* pBone)-> _bool
	{
		if (false == strcmp(pBoneName, pBone->Get_Name()))
			return true;

		return false;
	});

	if (iter == m_Bones.end())
		return nullptr;

	return *iter;
}

const _int CModel::Get_BoneIndex(const char* pBoneName)
{
	_uint		iIndex = 0;

	for (auto& pBone : m_Bones)
	{
		if (false == strcmp(pBone->Get_Name(), pBoneName))
			break;

		++iIndex;
	}

	return iIndex;
}

const _float CModel::Get_ViewZ(_uint iMeshIndex)
{
	if (nullptr != m_Meshes[iMeshIndex])
		return m_Meshes[iMeshIndex]->Get_ViewZ();
	return 0.f;
}

HRESULT CModel::Set_Texture_In_Material(const _uint& materialIndex, const _uint& aiTextureType, const char* pTexturePath)
{
	if (materialIndex >= m_iNumMaterials)
		return E_FAIL;

	if (aiTextureType >= AI_TEXTURE_TYPE_MAX)
		return E_FAIL;

	if (nullptr == pTexturePath)
		return E_FAIL;;

	char		szFileName[MAX_PATH] = "";
	char		szEXT[MAX_PATH] = "";

	_splitpath_s(pTexturePath, nullptr, 0, nullptr, 0, szFileName, MAX_PATH, szEXT, MAX_PATH);

	char		szDrive[MAX_PATH] = "";
	char		szDir[MAX_PATH] = "";

	_splitpath_s(m_Materials[materialIndex].szModelPath.c_str(), szDrive, MAX_PATH, szDir, MAX_PATH, nullptr, 0, nullptr, 0);

	char		szFullPath[MAX_PATH] = "";

	strcpy_s(szFullPath, szDrive);
	strcat_s(szFullPath, szDir);
	strcat_s(szFullPath, szFileName);
	strcat_s(szFullPath, szEXT);

	_tchar		szFinalPath[MAX_PATH] = TEXT("");

	MultiByteToWideChar(CP_ACP, 0, szFullPath, (int)strlen(szFullPath), szFinalPath, MAX_PATH);

	CTexture* pTexture = CTexture::Create(m_pDevice, m_pContext, szFinalPath);
	if (nullptr == pTexture)
		return E_FAIL;

	Safe_Release(m_Materials[materialIndex].pMtrlTexture[aiTextureType]);

	m_Materials[materialIndex].pMtrlTexture[aiTextureType] = pTexture;

	return S_OK;
}

HRESULT CModel::Initialize_Prototype(TYPE eType, const char* pModelFilePath, _fmatrix PivotMatrix, _bool saveJson)
{
	_uint		iFlag = 0;

	m_eType = eType;

	XMStoreFloat4x4(&m_PivotMatrix, PivotMatrix);

	/* aiProcess_PreTransformVertices : 정점정보를 읽어서 저장할 당시에 필요한 변환을 미리 처리해놓는다. */
	/* 처리? 이 모델의 뼈들 중, 메시의 이름과 같은 이름을 가진 뼈를 찾아서 그 뼈의 행렬을 정점들에게 미리 적용한다. */
	if (TYPE_NONANIM == m_eType || TYPE_NONANIM_UI == eType || TYPE_MESH_COLOR_NONANIM == eType)
		iFlag = aiProcess_PreTransformVertices | aiProcessPreset_TargetRealtime_Fast | aiProcess_ConvertToLeftHanded;
	else
		iFlag = aiProcessPreset_TargetRealtime_Fast | aiProcess_ConvertToLeftHanded;

	m_pAIScene = m_Importer.ReadFile(pModelFilePath, iFlag);
	if (nullptr == m_pAIScene)
		return E_FAIL;

	if (FAILED(Ready_Bones(m_pAIScene->mRootNode, nullptr)))
		return E_FAIL;

	/* 모델을 구성하는 메시들을 생성한다.(버텍스, 인덱스버퍼를 생성하는 것이다.) */
	if (FAILED(Ready_Meshes(saveJson)))
		return E_FAIL;

	if (FAILED(Ready_Materials(pModelFilePath)))
		return E_FAIL;

	if (FAILED(Ready_Animations()))
		return E_FAIL;

	if (saveJson)
		Save_Json(eType, pModelFilePath);

	return S_OK;
}

HRESULT CModel::Initialize_Prototype(TYPE eType, const char* pModelJsonFilePath, _fmatrix PivotMatrix)
{
	XMStoreFloat4x4(&m_PivotMatrix, PivotMatrix);

	m_eType = eType;

	if (FAILED(Load_Json(eType, pModelJsonFilePath)))
		return E_FAIL;

	return S_OK;
}

HRESULT CModel::Initialize(void* pArg)
{
	for (auto& pBone : m_Bones)
	{
		_int		iIndex = pBone->Get_ParentIndex();

		if (0 <= iIndex)
			pBone->Set_Parent(m_Bones[iIndex]);
	}

	return S_OK;
}

HRESULT CModel::SetUp_ShaderResource(CShader* pShader, const char* pConstantName, _uint iMeshIndex, aiTextureType eType)
{
	if (iMeshIndex >= m_iNumMeshes)
		return E_FAIL;

	_uint		iMaterialIndex = m_Meshes[iMeshIndex]->Get_MaterialIndex();

	if (nullptr != m_Materials[iMaterialIndex].pMtrlTexture[eType])
	{
		if (FAILED(m_Materials[iMaterialIndex].pMtrlTexture[eType]->Set_ShaderResource(pShader, pConstantName)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CModel::SetUp_BoneMatrices(CShader* pShader, const char* pConstantName, _uint iMeshIndex)
{
	_float4x4		BoneMatrices[256];
	ZeroMemory(BoneMatrices, sizeof(_float4x4) * 256);

	m_Meshes[iMeshIndex]->Get_BoneMatrices(BoneMatrices, m_Bones, XMLoadFloat4x4(&m_PivotMatrix));

	return pShader->Set_MatrixArray(pConstantName, BoneMatrices, 256);
}

_bool CModel::Play_Animation(_double TimeDelta)
{
	/* 실제 파일에 저장되어있었던 뼈의 각각의 시간에 맞는 상태로 뼈들의 상태를 변경하고.
	이렇게 변경된 뼈들의 상태를 부모에서부터 자식으로 순차적으로 누적하여 셋팅해나간다. */
	/*모든 뼈의 상태를 갱신한다.*/

	/* CAnimation : 특정 동작을 표현하는 객체. */
	/* : 이 동작을 구현하기위해 필요한 뼈들의 상태정보(시간에따라 다수, m_TransformationMatrix)를 가진다. */
	_bool result = m_Animations[m_iCurrentAnimationIndex]->Update(m_Bones, TimeDelta);

	/* 최상위 부모뼈부터 시작하여 최하위 자식뼈까지 전체를 순회하며 Combine행렬을 다시 만들어낸다. */
	for (auto& pBone : m_Bones)
	{
		pBone->SetUp_CombinedTransformationMatrix();
	}

	return result;
}

HRESULT CModel::Render(_uint iMeshIndex)
{
	if (nullptr != m_Meshes[iMeshIndex])
		m_Meshes[iMeshIndex]->Render();

	return S_OK;
}

_matrix CModel::Get_BoneCombinedMatrix(const _uint& boneIndex)
{
	return m_Bones[boneIndex]->Get_CombinedTransformationMatrix();
}

_float4x4 CModel::Get_CombinedTransformationMatrix_float4_4(const _uint& boneIndex)
{
	return m_Bones[boneIndex]->Get_CombinedTransformationMatrix_float4_4();
}

HRESULT CModel::Ready_Bones(aiNode* pAINode, CBone* pParent)
{
	CBone* pBone = CBone::Create(pAINode, this, pParent);
	if (nullptr == pBone)
		return E_FAIL;

	m_Bones.push_back(pBone);

	for (_uint i = 0; i < pAINode->mNumChildren; ++i)
	{
		Ready_Bones(pAINode->mChildren[i], pBone);
	}

	return S_OK;
}

HRESULT CModel::Ready_Animations()
{
	m_iNumAnimations = m_pAIScene->mNumAnimations;

	for (_uint i = 0; i < m_iNumAnimations; ++i)
	{
		CAnimation* pAnimation = CAnimation::Create(m_pAIScene->mAnimations[i], this);
		if (nullptr == pAnimation)
			return E_FAIL;

		m_Animations.push_back(pAnimation);
	}

	return S_OK;
}

HRESULT CModel::Save_Json(TYPE eType, const char* pModelFilePath)
{
	char		szDrive[MAX_PATH] = "";
	char		szDir[MAX_PATH] = "";
	char		szFileName[MAX_PATH] = "";

	_splitpath_s(pModelFilePath, szDrive, MAX_PATH, szDir, MAX_PATH, szFileName, MAX_PATH, nullptr, 0);

	char		szFullPath[MAX_PATH] = "";
	strcpy_s(szFullPath, szDrive);
	strcat_s(szFullPath, szDir);
	strcat_s(szFullPath, szFileName);
	strcat_s(szFullPath, ".json");

	Document doc(kObjectType);
	Document::AllocatorType& allocator = doc.GetAllocator();

	//* 구현부

#pragma region BoneSave
	Value Bones(kArrayType);
	{
		for (size_t i = 0; i < m_Bones.size(); ++i)
		{
			Value BoneDesc(kObjectType);
			{
				Value szName;
				string	boneName = m_Bones[i]->Get_Name();
				szName.SetString(boneName.c_str(), (SizeType)boneName.size(), allocator);
				BoneDesc.AddMember("szName", szName, allocator);

				BoneDesc.AddMember("m_iParentIndex", m_Bones[i]->Get_ParentIndex(), allocator);
				
				Value m_OffSetMatrix(kArrayType);
				//_float4x4 offsetMatrix = m_Bones[i]->Get_OffsetMatrix_float4_4();
				CUtility::Save_Matrix_in_json(m_OffSetMatrix, m_Bones[i]->Get_OffsetMatrix_float4_4(), allocator);
				BoneDesc.AddMember("m_OffSetMatrix", m_OffSetMatrix, allocator);

				Value m_TransformationMatrix(kArrayType);
				//_float4x4 offsetMatrix = m_Bones[i]->Get_OffsetMatrix_float4_4();
				CUtility::Save_Matrix_in_json(m_TransformationMatrix, m_Bones[i]->Get_TransformationMatrix_float4_4(), allocator);
				BoneDesc.AddMember("m_TransformationMatrix", m_TransformationMatrix, allocator);
			}
			Bones.PushBack(BoneDesc, allocator);
		}
	}
	doc.AddMember("Bones", Bones, allocator);
#pragma endregion

#pragma region MeshSave
	// Mesh 저장
	Value Meshs(kArrayType);
	{
		for (size_t i = 0; i < m_Meshes.size(); ++i)
		{
			Value MeshDesc(kObjectType);
			{
				Value szName;
				string	meshName = m_Meshes[i]->Get_Name();
				szName.SetString(meshName.c_str(), (SizeType)meshName.size(), allocator);
				MeshDesc.AddMember("szName", szName, allocator);

				MeshDesc.AddMember("m_iMaterialIndex", m_Meshes[i]->Get_MaterialIndex(), allocator);

				MeshDesc.AddMember("m_iNumBones", m_Meshes[i]->Get_iNumBones(), allocator);

				MeshDesc.AddMember("m_ViewZ", m_Meshes[i]->Get_ViewZ(), allocator);

				Value m_Bones(kArrayType);
				{
					vector<_int> Bones;
					m_Meshes[i]->Get_Bones(Bones);

					for (size_t j = 0; j < Bones.size(); ++j)
					{
						m_Bones.PushBack(Bones[j], allocator);
					}
				}

				MeshDesc.AddMember("m_Bones", m_Bones, allocator);

				MeshDesc.AddMember("m_iNumVertices", m_Meshes[i]->Get_NumVertices(), allocator);
				MeshDesc.AddMember("m_iStride", m_Meshes[i]->Get_Stride(), allocator);
				MeshDesc.AddMember("m_iNumBuffers", m_Meshes[i]->Get_NumBuffers(), allocator);
				MeshDesc.AddMember("m_iIndexSizePrimitive", m_Meshes[i]->Get_IndexSizePrimitive(), allocator);
				MeshDesc.AddMember("m_iNumPrimitives", m_Meshes[i]->Get_NumPrimitives(), allocator);
				MeshDesc.AddMember("m_iNumIndicesPrimitive", m_Meshes[i]->Get_NumIndicesPrimitive(), allocator);
				MeshDesc.AddMember("m_eFormat", m_Meshes[i]->Get_Format(), allocator);
				MeshDesc.AddMember("m_eTopology", m_Meshes[i]->Get_Topology(), allocator);

				Value VertexBuffers(kArrayType);
				{
					vector<VTXMODEL_ALL_DATA> VertexBufferData;
					m_Meshes[i]->Get_VertexBufferData(VertexBufferData);

					for (size_t j = 0; j < VertexBufferData.size(); ++j)
					{
						Value VertexBufferDesc(kObjectType);
						{
							VertexBufferDesc.AddMember("vPositionX", VertexBufferData[j].vPosition.x, allocator);
							VertexBufferDesc.AddMember("vPositionY", VertexBufferData[j].vPosition.y, allocator);
							VertexBufferDesc.AddMember("vPositionZ", VertexBufferData[j].vPosition.z, allocator);

							VertexBufferDesc.AddMember("vNormalX", VertexBufferData[j].vNormal.x, allocator);
							VertexBufferDesc.AddMember("vNormalY", VertexBufferData[j].vNormal.y, allocator);
							VertexBufferDesc.AddMember("vNormalZ", VertexBufferData[j].vNormal.z, allocator);

							VertexBufferDesc.AddMember("vColorX", VertexBufferData[j].vColor.x, allocator);
							VertexBufferDesc.AddMember("vColorY", VertexBufferData[j].vColor.y, allocator);
							VertexBufferDesc.AddMember("vColorZ", VertexBufferData[j].vColor.z, allocator);
							VertexBufferDesc.AddMember("vColorW", VertexBufferData[j].vColor.w, allocator);

							VertexBufferDesc.AddMember("vBlendIndexX", VertexBufferData[j].vBlendIndex.x, allocator);
							VertexBufferDesc.AddMember("vBlendIndexY", VertexBufferData[j].vBlendIndex.y, allocator);
							VertexBufferDesc.AddMember("vBlendIndexZ", VertexBufferData[j].vBlendIndex.z, allocator);
							VertexBufferDesc.AddMember("vBlendIndexW", VertexBufferData[j].vBlendIndex.w, allocator);

							VertexBufferDesc.AddMember("vBlendWeightX", VertexBufferData[j].vBlendWeight.x, allocator);
							VertexBufferDesc.AddMember("vBlendWeightY", VertexBufferData[j].vBlendWeight.y, allocator);
							VertexBufferDesc.AddMember("vBlendWeightZ", VertexBufferData[j].vBlendWeight.z, allocator);
							VertexBufferDesc.AddMember("vBlendWeightW", VertexBufferData[j].vBlendWeight.w, allocator);
						}
						VertexBuffers.PushBack(VertexBufferDesc, allocator);
					}
				}
				MeshDesc.AddMember("VertexBuffers", VertexBuffers, allocator);
				
				Value IndexBuffers(kArrayType);
				{
					vector<FACEINDICES32>	 IndexBufferDataArray;
					m_Meshes[i]->Get_IndexBufferData(IndexBufferDataArray);
					for (size_t j = 0; j < IndexBufferDataArray.size(); ++j)
					{
						Value IndexBufferDesc(kObjectType);
						{
							IndexBufferDesc.AddMember("_0", (_uint)IndexBufferDataArray[j]._0, allocator);
							IndexBufferDesc.AddMember("_1", (_uint)IndexBufferDataArray[j]._1, allocator);
							IndexBufferDesc.AddMember("_2", (_uint)IndexBufferDataArray[j]._2, allocator);

						}
						IndexBuffers.PushBack(IndexBufferDesc, allocator);
					}

				}
				MeshDesc.AddMember("IndexBuffers", IndexBuffers, allocator);

			}
			Meshs.PushBack(MeshDesc, allocator);
		}
	}
	doc.AddMember("Meshs", Meshs, allocator);
	// Mesh 저장 끝
#pragma endregion

#pragma region MaterialSave
	Value Materials(kArrayType);
	{
		m_iNumMaterials = m_pAIScene->mNumMaterials;
		
		Document TexturePaths(kArrayType);
		for (_uint i = 0; i < m_iNumMaterials; ++i)
		{
			aiMaterial* pAIMaterial = m_pAIScene->mMaterials[i];

			for (_uint j = 0; j < AI_TEXTURE_TYPE_MAX; ++j)
			{
				_uint	iNumTextures = pAIMaterial->GetTextureCount(aiTextureType(j));

				aiString		TexturePath;

				if (FAILED(pAIMaterial->GetTexture(aiTextureType(j), 0, &TexturePath)))
					continue;

				char		szFileName[MAX_PATH] = "";
				char		szEXT[MAX_PATH] = "";

				_splitpath_s(TexturePath.data, nullptr, 0, nullptr, 0, szFileName, MAX_PATH, szEXT, MAX_PATH);

				char		szDrive[MAX_PATH] = "";
				char		szDir[MAX_PATH] = "";

				_splitpath_s(pModelFilePath, szDrive, MAX_PATH, szDir, MAX_PATH, nullptr, 0, nullptr, 0);

				char		szFullPath[MAX_PATH] = "";

				strcpy_s(szFullPath, szDrive);
				strcat_s(szFullPath, szDir);
				strcat_s(szFullPath, szFileName);
				strcat_s(szFullPath, szEXT);

				Value szName;
				string	texturePath = szFullPath;
				szName.SetString(texturePath.c_str(), (SizeType)texturePath.size(), allocator);
				TexturePaths.PushBack(szName, allocator);
			}
			Materials.PushBack(TexturePaths, allocator);
		}
	}
	doc.AddMember("Materials", Materials, allocator);
#pragma endregion

#pragma region AnimationSave
	Value Animations(kArrayType);
	{
		for (size_t i = 0; i < m_Animations.size(); ++i)
		{
			Value AnimationDesc(kObjectType);
			{
				Value szName;
				string	animationName = m_Animations[i]->Get_Name();
				szName.SetString(animationName.c_str(), (SizeType)animationName.size(), allocator);
				AnimationDesc.AddMember("szName", szName, allocator);

				AnimationDesc.AddMember("m_Duration", m_Animations[i]->Get_Duration(), allocator);

				AnimationDesc.AddMember("m_TickPerSecond", m_Animations[i]->Get_TickPerSecond(), allocator);

				AnimationDesc.AddMember("m_iNumChannels", m_Animations[i]->Get_NumChannels(), allocator);

				Value m_Channels(kArrayType);
				{
					vector<CChannel*> channels;
					m_Animations[i]->Get_Channels(channels);

					Value ChannelDesc(kObjectType);
					for (size_t j = 0; j < channels.size(); ++j)
					{
						Value szName;
						string	channelName = channels[j]->Get_Name();
						szName.SetString(channelName.c_str(), (SizeType)channelName.size(), allocator);
						ChannelDesc.AddMember("szName", szName, allocator);

						ChannelDesc.AddMember("m_iNumKeyFrames", channels[j]->Get_NumKeyFrames(), allocator);

						Value KeyFrames(kArrayType);
						{
							vector<KEYFRAME> keyFrames;
							channels[j]->Get_KeyFrames(keyFrames);

							for (size_t k = 0; k < keyFrames.size(); k++)
							{
								Value KeyFrame(kObjectType);
								{
									KeyFrame.AddMember("vScaleX", keyFrames[k].vScale.x, allocator);
									KeyFrame.AddMember("vScaleY", keyFrames[k].vScale.y, allocator);
									KeyFrame.AddMember("vScaleZ", keyFrames[k].vScale.z, allocator);

									KeyFrame.AddMember("vRotationX", keyFrames[k].vRotation.x, allocator);
									KeyFrame.AddMember("vRotationY", keyFrames[k].vRotation.y, allocator);
									KeyFrame.AddMember("vRotationZ", keyFrames[k].vRotation.z, allocator);
									KeyFrame.AddMember("vRotationW", keyFrames[k].vRotation.w, allocator);

									KeyFrame.AddMember("vPositionX", keyFrames[k].vPosition.x, allocator);
									KeyFrame.AddMember("vPositionY", keyFrames[k].vPosition.y, allocator);
									KeyFrame.AddMember("vPositionZ", keyFrames[k].vPosition.z, allocator);

									KeyFrame.AddMember("Time", keyFrames[k].Time, allocator);
								}
								KeyFrames.PushBack(KeyFrame, allocator);
							}

						}
						ChannelDesc.AddMember("KeyFrames", KeyFrames, allocator);

						ChannelDesc.AddMember("m_iBoneIndex", channels[j]->Get_BoneIndex(), allocator);

					}
					m_Channels.PushBack(ChannelDesc, allocator);

				}
				AnimationDesc.AddMember("m_Channels", m_Channels, allocator);


				Value m_iCurrentKeyFrames(kArrayType);
				{
					vector<_uint> CurrentKeyFrames;
					m_Animations[i]->Get_CurrentKeyFrames(CurrentKeyFrames);

					for (size_t j = 0; j < CurrentKeyFrames.size(); ++j)
					{
						m_iCurrentKeyFrames.PushBack(CurrentKeyFrames[j], allocator);
					}

				}
				AnimationDesc.AddMember("m_iCurrentKeyFrames", m_iCurrentKeyFrames, allocator);

			}
			Animations.PushBack(AnimationDesc, allocator);
		}
	}
	doc.AddMember("Animations", Animations, allocator);

#pragma endregion 

	// 구현부 끝

	FILE* fp = fopen(szFullPath, "wb"); // non-Windows use "w"

	if (doc.MemberCount() <= 0)
		return E_FAIL;

	if (NULL == fp)
	{
		MSG_BOX("Save File Open Error");
		return E_FAIL;
	}
	else
	{
		char* writeBuffer = new char[65536];
		FileWriteStream os(fp, writeBuffer, sizeof(writeBuffer));

		//PrettyWriter<FileWriteStream> writer(os);
		Writer<FileWriteStream> writer(os);

		doc.Accept(writer);

		fclose(fp);

		Safe_Delete_Array(writeBuffer);
	}

	return S_OK;
}

HRESULT CModel::Load_Json(TYPE eType, const char* pModelFilePath)
{
	FILE* fp = fopen(pModelFilePath, "rb"); // non-Windows use "r"

	if (NULL == fp)
	{
		MSG_BOX("Load File Open Error");
		return E_FAIL;
	}
	else
	{
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> convert;

		char* readBuffer = new char[65536];
		FileReadStream is(fp, readBuffer, sizeof(readBuffer));

		Document document;
		document.ParseStream(is);

#pragma region BoneLoad
		const Value& Bones = document["Bones"];
		assert(Bones.IsArray());

		m_Bones.reserve(Bones.Size());
		for (SizeType i = 0; i < Bones.Size(); ++i)
		{
			CBone* pBone = CBone::Create();
			pBone->Set_Name(Bones[i]["szName"].GetString());

			_int parentIndex = Bones[i]["m_iParentIndex"].GetInt();
			pBone->Set_ParentIndex(parentIndex);
			if (parentIndex >= 0)
			{
				pBone->Set_Parent(m_Bones[parentIndex]);
			}

			const Value& OffSetMatrix = Bones[i]["m_OffSetMatrix"];
			assert(OffSetMatrix.IsArray());
			_float4x4 offsetMatrix{};
			
			_uint indexX = 0;
			_uint indexY = 0;
			for (size_t j = 0; j < 16; ++j)
			{
				indexX = _uint(j / 4);
				indexY = j % 4;
				offsetMatrix.m[indexX][indexY] = OffSetMatrix[j].GetFloat();
			}
			pBone->Set_OffSetMatrix(offsetMatrix);

			const Value& TransformationMatrix = Bones[i]["m_TransformationMatrix"];
			assert(TransformationMatrix.IsArray());
			_float4x4 transformationMatrix{};

			indexX = 0;
			indexY = 0;

			for (size_t j = 0; j < 16; ++j)
			{
				indexX = _uint(j / 4);
				indexY = j % 4;
				transformationMatrix.m[indexX][indexY] = TransformationMatrix[j].GetFloat();
			}
			pBone->Set_TransformationMatrix(transformationMatrix);
			
			m_Bones.push_back(pBone);
		}
#pragma endregion 

#pragma region MeshLoad
		const Value& Meshs = document["Meshs"];
		assert(Meshs.IsArray());

		m_iNumMeshes = Meshs.Size();

		m_Meshes.reserve(Meshs.Size());
		for (SizeType i = 0; i < Meshs.Size(); ++i)
		{
			//const Value& Mash = Meshs[i].GetObj();
			CMesh* pMesh = CMesh::Create(m_pDevice, m_pContext, eType, Meshs[i]);

			if (nullptr == pMesh)
				return E_FAIL;

			m_Meshes.push_back(pMesh);
		}
#pragma endregion	

#pragma region MaterialLoad
		const Value& Materials = document["Materials"];
		assert(Materials.IsArray());

		m_iNumMaterials = Materials.Size();

		m_Materials.reserve(Materials.Size());
		for (SizeType i = 0; i < Materials.Size(); ++i)
		{
			MESH_MATERIAL material{};
			
			assert(Materials[i].IsArray());
			for (_uint j = 0; j < Materials[i].Size(); ++j)
			{
				wstring texturePath = convert.from_bytes(Materials[i][j].GetString());

				CTexture* pTexture = CTexture::Create(m_pDevice, m_pContext, texturePath.c_str());
				if (nullptr == pTexture)
					return E_FAIL;

				material.pMtrlTexture[j] = pTexture;
			}

			m_Materials.push_back(material);
		}

#pragma endregion	

#pragma region AnimationLoad
		const Value& Animations = document["Animations"];
		assert(Animations.IsArray());

		m_iNumAnimations = Animations.Size();
		m_Animations.reserve(Animations.Size());
		for (SizeType i = 0; i < Animations.Size(); ++i)
		{
			CAnimation* pAnimation = CAnimation::Create();

			pAnimation->Set_Name(Animations[i]["szName"].GetString());
			pAnimation->Set_Duration(Animations[i]["m_Duration"].GetDouble());
			pAnimation->Set_TickPerSecond(Animations[i]["m_TickPerSecond"].GetDouble());

			pAnimation->Set_NumChannels(Animations[i]["m_iNumChannels"].GetUint());

			const Value& Channels = Animations[i]["m_Channels"];
			assert(Channels.IsArray());
			for (SizeType j = 0; j < Channels.Size(); ++j)
			{
				CChannel* pChannel = CChannel::Create();
				pChannel->Set_Name(Channels[j]["szName"].GetString());
				
				pChannel->Set_NumKeyFrames(Channels[j]["m_iNumKeyFrames"].GetUint());

				const Value& KeyFrames = Channels[j]["KeyFrames"];
				assert(KeyFrames.IsArray());
				for (SizeType k = 0; k < KeyFrames.Size(); ++k)
				{
					KEYFRAME keyFrame{};
					keyFrame.vScale =
					{
						KeyFrames[k]["vScaleX"].GetFloat(),
						KeyFrames[k]["vScaleY"].GetFloat(),
						KeyFrames[k]["vScaleZ"].GetFloat()
					};

					keyFrame.vRotation =
					{
						KeyFrames[k]["vRotationX"].GetFloat(),
						KeyFrames[k]["vRotationY"].GetFloat(),
						KeyFrames[k]["vRotationZ"].GetFloat(),
						KeyFrames[k]["vRotationW"].GetFloat()
					};

					keyFrame.vPosition =
					{
						KeyFrames[k]["vPositionX"].GetFloat(),
						KeyFrames[k]["vPositionY"].GetFloat(),
						KeyFrames[k]["vPositionZ"].GetFloat()
					};

					keyFrame.Time = KeyFrames[k]["Time"].GetDouble();
					pChannel->Add_KeyFrame(keyFrame);
				}

				pChannel->Set_BoneIndex(Channels[j]["m_iBoneIndex"].GetUint());

				pAnimation->Add_Channel(pChannel);
			}

			const Value& CurrentKeyFrames = Animations[i]["m_iCurrentKeyFrames"];
			assert(CurrentKeyFrames.IsArray());
			for (SizeType j = 0; j < CurrentKeyFrames.Size(); ++j)
			{
				pAnimation->Add_CurrentKeyFrames(CurrentKeyFrames[j].GetUint());
			}

			m_Animations.push_back(pAnimation);
		}
#pragma endregion

		fclose(fp);
		Safe_Delete_Array(readBuffer);
	}
	return S_OK;
}

HRESULT CModel::Ready_Meshes(_bool saveJson)
{
	m_iNumMeshes = m_pAIScene->mNumMeshes;

	if (m_pAIScene->mNumCameras > 0)
	{
		//aiCamera camera = m_pAIScene->mCameras[0][0];
		//aiVector3D normalPos = camera.mLookAt.Normalize();

		//XMStoreFloat4x4(&m_PivotMatrix, XMLoadFloat4x4(&m_PivotMatrix) * XMMatrixTranslation(-normalPos.x, -normalPos.y, -normalPos.z));

		for (_uint i = 0; i < m_iNumMeshes; ++i)
		{
			CMesh* pMesh = CMesh::Create(m_pDevice, m_pContext, m_eType, m_pAIScene->mMeshes[i], this, XMLoadFloat4x4(&m_PivotMatrix), saveJson);
			if (nullptr == pMesh)
				return E_FAIL;

			m_Meshes.push_back(pMesh);
		}

	}
	else 
	{
		for (_uint i = 0; i < m_iNumMeshes; ++i)
		{
			CMesh* pMesh = CMesh::Create(m_pDevice, m_pContext, m_eType, m_pAIScene->mMeshes[i], this, XMLoadFloat4x4(&m_PivotMatrix), saveJson);
			if (nullptr == pMesh)
				return E_FAIL;

			m_Meshes.push_back(pMesh);
		}
	}

	return S_OK;
}

HRESULT CModel::Ready_Materials(const char* pModelFilePath)
{
	/* 머테리얼을 표현하기위한 텍스쳐를 로드하자. */
	m_iNumMaterials = m_pAIScene->mNumMaterials;

	for (_uint i = 0; i < m_iNumMaterials; ++i)
	{
		aiMaterial* pAIMaterial = m_pAIScene->mMaterials[i];
		MESH_MATERIAL	ModelMaterial;
		ZeroMemory(&ModelMaterial, sizeof ModelMaterial);

		for (_uint j = 0; j < AI_TEXTURE_TYPE_MAX; ++j)
		{
			_uint	iNumTextures = pAIMaterial->GetTextureCount(aiTextureType(j));

			aiString		TexturePath;

			if (FAILED(pAIMaterial->GetTexture(aiTextureType(j), 0, &TexturePath)))
				continue;

			char		szFileName[MAX_PATH] = "";
			char		szEXT[MAX_PATH] = "";

			_splitpath_s(TexturePath.data, nullptr, 0, nullptr, 0, szFileName, MAX_PATH, szEXT, MAX_PATH);

			char		szDrive[MAX_PATH] = "";
			char		szDir[MAX_PATH] = "";

			_splitpath_s(pModelFilePath, szDrive, MAX_PATH, szDir, MAX_PATH, nullptr, 0, nullptr, 0);

			char		szFullPath[MAX_PATH] = "";

			strcpy_s(szFullPath, szDrive);
			strcat_s(szFullPath, szDir);
			strcat_s(szFullPath, szFileName);
			strcat_s(szFullPath, szEXT);

			_tchar		szFinalPath[MAX_PATH] = TEXT("");

			MultiByteToWideChar(CP_ACP, 0, szFullPath, (int)strlen(szFullPath), szFinalPath, MAX_PATH);

			ModelMaterial.szModelPath = pModelFilePath;

			ModelMaterial.pMtrlTexture[j] = CTexture::Create(m_pDevice, m_pContext, szFinalPath);
			if (nullptr == ModelMaterial.pMtrlTexture[j])
				return E_FAIL;
		}

		m_Materials.push_back(ModelMaterial);
	}

	return S_OK;
}

CModel* CModel::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, TYPE eType, const char* pModelFilePath, _fmatrix PivotMatrix, _bool saveJson)
{
	CModel* pInstance = new CModel(pDevice, pContext, nullptr);

	if (FAILED(pInstance->Initialize_Prototype(eType, pModelFilePath, PivotMatrix, saveJson)))
	{
		MSG_BOX("Failed to Created : CModel");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CModel* CModel::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const char* pModelJsonFilePath, TYPE eType, _fmatrix PivotMatrix)
{
	CModel* pInstance = new CModel(pDevice, pContext, nullptr);

	if (FAILED(pInstance->Initialize_Prototype(eType, pModelJsonFilePath, PivotMatrix)))
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


	for (auto& pAnimation : m_Animations)
		Safe_Release(pAnimation);

	m_Animations.clear();

	for (auto& pBone : m_Bones)
		Safe_Release(pBone);

	m_Bones.clear();

	for (auto& pMesh : m_Meshes)
		Safe_Release(pMesh);

	m_Meshes.clear();

	for (auto& Material : m_Materials)
	{
		for (auto& pTexture : Material.pMtrlTexture)
			Safe_Release(pTexture);

	}

	m_Materials.clear();

	m_Importer.FreeScene();
}
