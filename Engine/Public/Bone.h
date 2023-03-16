#pragma once

#include "Base.h"

BEGIN(Engine)

class CModel;

class CBone final : public CBase
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

	_matrix Get_CombinedTransformationMatrix() {
		return XMLoadFloat4x4(&m_CombinedTransformationMatrix);
	}

	const _int		Get_ParentIndex() const {
		return m_iParentIndex;
	}

	_float4x4 Get_CombinedTransformationMatrix_float4_4() {
		return m_CombinedTransformationMatrix;
	}

public:
	void	Set_Parent(CBone* pParent) {
		m_pParent = pParent;
		Safe_AddRef(m_pParent);
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
	_float4x4		m_TransformationMatrix = {}; /* ���� ������� : �θ�������� ǥ���� �� ������ ���º�ȯ���. */
	_float4x4		m_CombinedTransformationMatrix = {}; /* ���� ������� : m_TransformationMatrix * �θ�`s  m_CombinedTransformationMatrix*/

public:
	static CBone* Create(aiNode* pAINode, CModel* pModel, CBone* pParent);
	CBone* Clone();
	virtual void Free() override;
};

END