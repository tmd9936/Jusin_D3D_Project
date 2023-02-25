#include "Texture.h"
#include "Shader.h"

CTexture::CTexture(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CGameObject* pOwner)
	: CComponent(pDevice, pContext, pOwner)
{
}

CTexture::CTexture(const CTexture& rhs, CGameObject* pOwner)
	: CComponent(rhs, pOwner)
	, m_iNumTextures(rhs.m_iNumTextures)
	, m_SRVs(rhs.m_SRVs)
{
	for (auto& pSRV : m_SRVs)
		Safe_AddRef(pSRV);
}

HRESULT CTexture::Set_ShaderResource(CShader* pShader, const char* pConstantName, _uint iTextureIndex)
{
	if (iTextureIndex >= m_iNumTextures)
		return E_FAIL;

	return pShader->Set_ShaderResourceView(pConstantName, m_SRVs[iTextureIndex]);
}

HRESULT CTexture::Initialize_Prototype(const _tchar* pTextureFilePath, _uint iNumTextures)
{
	m_iNumTextures = iNumTextures;

	for (_uint i = 0; i < iNumTextures; ++i)
	{
		TCHAR		szFilePath[128] = L"";

		wsprintfW(szFilePath, pTextureFilePath, i);

		_tchar szDir[MAX_PATH] = TEXT("");
		_tchar szFileName[MAX_PATH] = TEXT("");
		_tchar szEXT[MAX_PATH] = TEXT("");

		_wsplitpath_s(szFilePath, nullptr, 0, szDir, MAX_PATH, szFileName, MAX_PATH, szEXT, MAX_PATH);

		ID3D11ShaderResourceView* pSRV = { nullptr };

		if (!lstrcmp(szEXT, TEXT(".dds")))
		{
			CreateDDSTextureFromFile(m_pDevice, szFilePath, nullptr, &pSRV);
		}
		else if (!lstrcmp(szEXT, TEXT(".tga")))
			return E_FAIL;
		else
			CreateWICTextureFromFile(m_pDevice, szFilePath, nullptr, &pSRV);

		m_SRVs.push_back(pSRV);

	}

	return S_OK;
}

HRESULT CTexture::Initialize(void* pArg)
{
	
	return S_OK;
}

CTexture* CTexture::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pTextureFilePath, _uint iNumTextures)
{
	CTexture* pInstance = new CTexture(pDevice, pContext, nullptr);

	if (FAILED(pInstance->Initialize_Prototype(pTextureFilePath, iNumTextures)))
	{
		MSG_BOX("Failed to Created : CTexture");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CTexture::Clone(CGameObject* pOwner, void* pArg)
{
	CTexture* pInstance = new CTexture(*this, pOwner);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CTexture");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTexture::Free()
{
	__super::Free();
	for (_uint i = 0; i < m_SRVs.size(); ++i)
	{
		Safe_Release(m_SRVs[i]);
	}

}
