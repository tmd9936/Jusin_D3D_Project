#pragma once

#include "Model.h"
#include "VIBuffer.h"

/* 모델을 구성하는 하나의 메시. 메시의 교체를 노리기위해. */
/* 메시별로 정점, 인덱스버퍼를 가진다. */
BEGIN(Engine)

class CBone;

class CMesh final : public CVIBuffer
{
private:
	CMesh(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CGameObject* pOwner);
	CMesh(const CMesh& rhs, CGameObject* pOwner);
	virtual ~CMesh() = default;

public:
	_uint Get_MaterialIndex() const {
		return m_iMaterialIndex;
	}

	void Get_BoneMatrices(_Out_ _float4x4* pBoneMatrices, _In_ const vector<class CBone*>& Bones, _In_ _matrix PivotMatrix);

	const _float Get_ViewZ() const {
		return m_ViewZ;
	}

	string Get_Name() {
		return m_szName;
	}

	_uint	Get_iNumBones() {
		return m_iNumBones;
	}

	void	 Get_Bones(vector<_int>& Bones) {
		for (size_t i = 0; i < m_Bones.size(); i++)
		{
			Bones.push_back(m_Bones[i]);
		}
	}

public:
	virtual HRESULT Initialize_Prototype(CModel::TYPE eType, aiMesh* pAIMesh, CModel* pModel, _fmatrix PivotMatrix);
	virtual HRESULT Initialize(void* pArg) override;

private:
	char					m_szName[MAX_PATH] = "";
	_uint					m_iMaterialIndex = { 0 };

private:
	_uint					m_iNumBones = { 0 };
	vector<_int>			m_Bones;

	_float					m_ViewZ = { 0.f };

private:
	HRESULT Ready_VertexBuffer_ForNonAnim(aiMesh* pAIMesh, _fmatrix PivotMatrix);
	HRESULT Ready_VertexBuffer_ForAnim(aiMesh* pAIMesh, CModel* pModel);

	HRESULT Ready_VertexBuffer_ForNonAnimUI(aiMesh* pAIMesh, _fmatrix PivotMatrix);
	HRESULT Ready_VertexBuffer_ForAnimUI(aiMesh* pAIMesh, CModel* pModel);

	HRESULT Ready_VertexBuffer_ForColorNonAnim(aiMesh* pAIMesh, _fmatrix PivotMatrix);
	HRESULT Ready_VertexBuffer_ForColorAnim(aiMesh* pAIMesh, CModel* pModel);


public:
	static CMesh* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CModel::TYPE eType, aiMesh* pAIMesh, CModel* pModel, _fmatrix PivotMatrix);
	virtual CComponent* Clone(CGameObject* pOwner, void* pArg = nullptr) override;
	virtual void Free() override;
};

END