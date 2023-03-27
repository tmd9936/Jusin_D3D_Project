#pragma once

#include "Component.h"

BEGIN(Engine)

class CBone;
class CAnimation;
class CMesh;
class CShader;
class CChannel;

class ENGINE_DLL CModel final : public CComponent
{
public:
	static const FamilyId familyId = FAMILY_ID_VIBUFFER;

public:
	enum TYPE { TYPE_NONANIM, TYPE_ANIM, TYPE_NONANIM_UI, TYPE_ANIM_UI, TYPE_MESH_COLOR_NONANIM, TYPE_MESH_COLOR_ANIM, TYPE_END };
private:
	explicit CModel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CGameObject* pOwner);
	explicit CModel(const CModel& rhs, CGameObject* pOwner);
	virtual ~CModel() = default;

public:
	_uint Get_NumMeshes() const {
		return m_iNumMeshes;
	}

	CBone* Get_BonePtr(const char* pBoneName);

	const _int	Get_BoneIndex(const char* pBoneName);

	const _float Get_ViewZ(_uint iMeshIndex);

	_matrix Get_PivotMatrix() const {
		return XMLoadFloat4x4(&m_PivotMatrix);
	}

public:
	void Set_Animation(_uint iIndex);
public:
	HRESULT Set_Texture_In_Material(const _uint& materialIndex, const _uint& aiTextureType, const char* pTexturePath);

public:
	virtual HRESULT Initialize_Prototype(TYPE eType, const char* pModelFilePath, _fmatrix PivotMatrix, _bool saveJson);

	virtual HRESULT Initialize_Prototype(TYPE eType, const char* pModelJsonFilePath, _fmatrix PivotMatrix);

	virtual HRESULT Initialize(void* pArg);

public:
	HRESULT SetUp_ShaderResource(CShader* pShader, const char* pConstantName, _uint iMeshIndex, aiTextureType eType);
	HRESULT SetUp_BoneMatrices(CShader* pShader, const char* pConstantName, _uint iMeshIndex);
	_bool Play_Animation(_double TimeDelta, _bool isLoop = true);
	HRESULT Render(_uint iMeshIndex);

public:
	_matrix Get_BoneCombinedMatrix(const _uint& boneIndex);

	_float4x4 Get_CombinedTransformationMatrix_float4_4(const _uint& boneIndex);

	HRESULT Get_Mesh_VertexBuffer_Data(_uint meshIndex, vector<VTXMODEL_ALL_DATA>& VertexBufferData);
	HRESULT Get_Mesh_IndexBuffer_Data(_uint meshIndex, vector<FACEINDICES32>& indexBuffer);

	
public:
	static const _double		Get_LerpDuration();
	static void					Set_LerpDuration(_double duration);

private:
	Assimp::Importer				m_Importer; /* 경로의 파일을 읽어서 저장하는 기능을 한다. */
	const aiScene*					m_pAIScene = { nullptr }; /* 실제 데이터들을 저장하고 있으며 변수를 통해 접근 가능하도록 처리해주는 객체. */

private:
	TYPE					m_eType = { TYPE_END };

private:
	_uint					m_iNumMeshes = { 0 };
	vector<CMesh*>			m_Meshes;

private:
	_float4x4				m_PivotMatrix = { };		

private:
	/* 모델에게 정의되어있는 머테리얼의 갯수. */
	_uint					m_iNumMaterials = { 0 };
	vector<MESH_MATERIAL>	m_Materials;

private:
	/* 이 하나의 모델이 사용하는 모든 뼈들. */
	vector<CBone*>			m_Bones;

private:
	_uint					m_iPreAnimationIndex = { 0 };
	_uint					m_iCurrentAnimationIndex = { 0 };
	_uint					m_iNumAnimations = { 0 };
	vector<CAnimation*>		m_Animations;

private:
	HRESULT Ready_Meshes(_bool saveJson);
	HRESULT Ready_Materials(const char* pModelFilePath);
	HRESULT Ready_Bones(aiNode* pAINode, CBone* pParent);
	HRESULT Ready_Animations();

private:
	HRESULT Save_Json(TYPE eType, const char* pModelFilePath);
	HRESULT Load_Json(TYPE eType, const char* pModelFilePath);

public:
	static CModel* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, TYPE eType, const char* pModelFilePath, _fmatrix PivotMatrix = XMMatrixIdentity(), _bool saveJson = false);

	static CModel* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const char* pModelJsonFilePath, TYPE eType, _fmatrix PivotMatrix);

	virtual CComponent* Clone(CGameObject* pOwner, void* pArg = nullptr) override;
	virtual void Free() override;
};

END

