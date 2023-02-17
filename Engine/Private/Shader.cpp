#include "..\Public\Shader.h"

CShader::CShader(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CComponent(pDevice, pContext)
{

}

CShader::CShader(const CShader& rhs)
	: CComponent(rhs)
	, m_pEffect(rhs.m_pEffect)
	, m_Passes(rhs.m_Passes)
	, m_iNumPasses(rhs.m_iNumPasses)
{
	Safe_AddRef(m_pEffect);

	for (_uint i = 0; i < m_iNumPasses; ++i)
	{
		Safe_AddRef(m_Passes[i].pInputLayout);
	}

}

HRESULT CShader::Initialize_Prototype(const _tchar* pShaderFilePath, D3D11_INPUT_ELEMENT_DESC* pElements, _uint iNumElements)
{
	_uint			iFlag = 0;

#ifdef _DEBUG
	iFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
	iFlag = D3DCOMPILE_OPTIMIZATION_LEVEL1;
#endif	

	if (FAILED(D3DX11CompileEffectFromFile(pShaderFilePath, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, iFlag, 0, m_pDevice, &m_pEffect, nullptr)))
		return E_FAIL;

	ID3D11InputLayout* pInputLayout = nullptr;

	ID3DX11EffectTechnique* pTechnique = m_pEffect->GetTechniqueByIndex(0);

	D3DX11_TECHNIQUE_DESC		TechniqueDesc;
	ZeroMemory(&TechniqueDesc, sizeof TechniqueDesc);

	pTechnique->GetDesc(&TechniqueDesc);

	m_iNumPasses = TechniqueDesc.Passes;

	for (_uint i = 0; i < m_iNumPasses; ++i)
	{
		PASSDESC					PassInfo;
		ZeroMemory(&PassInfo, sizeof PassInfo);

		PassInfo.pPass = pTechnique->GetPassByIndex(i);

		D3DX11_PASS_DESC			PassDesc;
		ZeroMemory(&PassDesc, sizeof PassDesc);

		PassInfo.pPass->GetDesc(&PassDesc);

		if (FAILED(m_pDevice->CreateInputLayout(pElements, iNumElements, PassDesc.pIAInputSignature, PassDesc.IAInputSignatureSize, &PassInfo.pInputLayout)))
			return E_FAIL;

		m_Passes.push_back(PassInfo);
	}

	return S_OK;
}

HRESULT CShader::Initialize(void* pArg)
{
	return S_OK;
}

HRESULT CShader::Begin(_uint iPassIndex)
{
	if (iPassIndex >= m_iNumPasses)
		return E_FAIL;

	m_pContext->IASetInputLayout(m_Passes[iPassIndex].pInputLayout);

	/* dx9 : BeginPass(iIndex) */
	/* �ݵ�� ȣ�� ���� ���̴��� �������� ���ҽ�(�ؽ���, ���, ���� etc) ���� ��� ������. */
	m_Passes[iPassIndex].pPass->Apply(0, m_pContext);

	return S_OK;
}

HRESULT CShader::Set_Matrix(const char* pConstantName, _float4x4* pMatrix)
{
	if (nullptr == m_pEffect)
		return E_FAIL;

	ID3DX11EffectMatrixVariable* pVariable = m_pEffect->GetVariableByName(pConstantName)->AsMatrix();

	if (nullptr == pVariable)
		return E_FAIL;

	return pVariable->SetMatrix((_float*)pMatrix);
}

HRESULT CShader::Set_SRV(const char* pConstantName, ID3D11ShaderResourceView* pSRV)
{
	if (nullptr == m_pEffect)
		return E_FAIL;

	ID3DX11EffectShaderResourceVariable* pVariable = m_pEffect->GetVariableByName(pConstantName)->AsShaderResource();

	if (nullptr == pVariable)
		return E_FAIL;

	return pVariable->SetResource(pSRV);
}

CShader* CShader::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pShaderFilePath, D3D11_INPUT_ELEMENT_DESC* pElements, _uint iNumElements)
{
	CShader* pInstance = new CShader(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(pShaderFilePath, pElements, iNumElements)))
	{
		MSG_BOX("Failed to Created : CShader");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CShader::Clone(void* pArg)
{
	CShader* pInstance = new CShader(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CShader");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CShader::Free()
{
	__super::Free();
	for (_uint i = 0; i < m_iNumPasses; ++i)
	{
		Safe_Release(m_Passes[i].pInputLayout);
	}

	Safe_Release(m_pEffect);
}
