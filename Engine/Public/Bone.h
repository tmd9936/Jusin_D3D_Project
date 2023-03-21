#pragma once

#include "Base.h"

BEGIN(Engine)

class CModel;

class ENGINE_DLL CBone final : public CBase
{
private:
	CBone();
	CBone(const CBone& rhs);
	virtual ~CBone() = default;
public:
	const char* Get_Name() const {
		return m_szName;
	}

	_matrix Get_OffsetMatrix() {
		return XMLoadFloat4x4(&m_OffSetMatrix);
	}

	_float4x4 Get_OffsetMatrix_float4_4() {
		return m_OffSetMatrix;
	}

	_matrix Get_CombinedTransformationMatrix() {
		return XMLoadFloat4x4(&m_CombinedTransformationMatrix);
	}

	const _int		Get_ParentIndex() const {
		return m_iParentIndex;
	}

	_float4x4 Get_TransformationMatrix_float4_4() {
		return m_TransformationMatrix;
	}

	_float4x4 Get_CombinedTransformationMatrix_float4_4() {
		return m_CombinedTransformationMatrix;
	}

public:
	void	Set_Parent(CBone* pParent) {
		m_pParent = pParent;
		Safe_AddRef(m_pParent);
	}

	void Set_Name(char szName[MAX_PATH]) {
		strcpy_s(m_szName, szName);
	}

	void Set_Name(string szName) {
		strcpy_s(m_szName, szName.c_str());
	}

	void Set_ParentIndex(_int index) {
		m_iParentIndex = index;
	}

	void Set_OffSetMatrix(const _float4x4& _matrix) {
		m_OffSetMatrix = _matrix;
	}

	void Set_TransformationMatrix(const _float4x4& _matrix) {
		m_TransformationMatrix = _matrix;
	}


public:
	HRESULT Initialize(aiNode* pAINode, CModel* pModel, CBone* pParent);
	void SetUp_TransformationMatrix(_fmatrix Matrix);
	void SetUp_CombinedTransformationMatrix();
	void SetUp_OffsetMatrix(_fmatrix Matrix);


private:
	char			m_szName[MAX_PATH] = "";
	CBone*			m_pParent = { nullptr };
	_int			m_iParentIndex = { -1 };
	_float4x4		m_OffSetMatrix = {};
	_float4x4		m_TransformationMatrix = {}; /* 뼈의 상태행렬 : 부모기준으로 표현된 이 뼈만의 상태변환행렬. */
	_float4x4		m_CombinedTransformationMatrix = {}; /* 뼈의 상태행렬 : m_TransformationMatrix * 부모`s  m_CombinedTransformationMatrix*/

public:
	static CBone* Create(aiNode* pAINode, CModel* pModel, CBone* pParent);
	static CBone* Create();

	CBone* Clone();
	virtual void Free() override;
};

END