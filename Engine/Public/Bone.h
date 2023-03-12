#pragma once

#include "Base.h"

BEGIN(Engine)

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

public:
	HRESULT Initialize(aiNode* pAINode, CBone* pParent);
	void SetUp_TransformationMatrix(_fmatrix Matrix);
	void SetUp_CombinedTransformationMatrix();


private:
	char			m_szName[MAX_PATH] = "";
	CBone*			m_pParent = { nullptr };
	_float4x4		m_TransformationMatrix; /* 뼈의 상태행렬 : 부모기준으로 표현된 이 뼈만의 상태변환행렬. */
	_float4x4		m_CombinedTransformationMatrix; /* 뼈의 상태행렬 : m_TransformationMatrix * 부모`s  m_CombinedTransformationMatrix*/

public:
	static CBone* Create(aiNode* pAINode, CBone* pParent);
	virtual void Free() override;
};

END