#include "Texture.h"

CTexture::CTexture(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CComponent(pDevice, pContext)
{
}

CTexture::CTexture(const CTexture& rhs)
	: CComponent(rhs)
	, m_iNumTextures(rhs.m_iNumTextures)
{
	m_SRVs.reserve(rhs.m_iNumTextures);

	for (_uint i = 0; i < rhs.m_iNumTextures; ++i)
	{
		m_SRVs[i] = rhs.m_SRVs[i];
		Safe_AddRef(m_SRVs[i]);
	}

}

HRESULT CTexture::Initialize_Prototype(const _tchar* pTextureFilePath, _uint iNumTextures)
{
	m_iNumTextures = iNumTextures;

	for (_uint i = 0; i < iNumTextures; ++i)
	{
		_tchar szDir[MAX_PATH] = TEXT("");
		_tchar szFileName[MAX_PATH] = TEXT("");
		_tchar szEXT[MAX_PATH] = TEXT("");

		_wsplitpath_s(pTextureFilePath, nullptr, 0, szDir, MAX_PATH, szFileName, MAX_PATH, szEXT, MAX_PATH);

		ID3D11ShaderResourceView* pSRV = { nullptr };

		if (!lstrcmp(szEXT, TEXT(".dds")))
		{
			CreateDDSTextureFromFile(m_pDevice, pTextureFilePath, nullptr, &pSRV);
		}
		else if (!lstrcmp(szEXT, TEXT(".tga")))
			return E_FAIL;
		else
			CreateWICTextureFromFile(m_pDevice, pTextureFilePath, nullptr, &pSRV);

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
	CTexture* pInstance = new CTexture(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(pTextureFilePath, iNumTextures)))
	{
		MSG_BOX("Failed to Created : CTexture");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CTexture::Clone(void* pArg)
{
	CTexture* pInstance = new CTexture(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CTexture");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTexture::Free()
{
	for (_uint i = 0; i < m_SRVs.size(); ++i)
	{
		Safe_Release(m_SRVs[i]);
	}
	__super::Free();

}
