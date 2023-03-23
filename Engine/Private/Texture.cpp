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
	, m_Textures(rhs.m_Textures)
	, m_TexturePath(rhs.m_TexturePath)
{
	for (auto& pSRV : m_SRVs)
		Safe_AddRef(pSRV);

	for (auto& pTexture : m_Textures)
		Safe_AddRef(pTexture);
}

HRESULT CTexture::Set_ShaderResource(CShader* pShader, const char* pConstantName, _uint iTextureIndex)
{
	if (iTextureIndex >= m_iNumTextures)
		return E_FAIL;

	return pShader->Set_ShaderResourceView(pConstantName, m_SRVs[iTextureIndex]);
}

HRESULT CTexture::Set_ShaderResourceArray(CShader* pShader, const char* pConstantName)
{
	ID3D11ShaderResourceView* pSRVs[2] = {
		m_SRVs[0],
		m_SRVs[1],
	};
	return pShader->Set_ShaderResourceViewArray(pConstantName, pSRVs, m_iNumTextures);
}

HRESULT CTexture::Create_New_Texture(_uint iTextureIndex, _uint width, _uint height)
{
	if (iTextureIndex >= m_iNumTextures)
		return E_FAIL;

	ID3D11Texture2D* pTexture2D = { nullptr };

	D3D11_TEXTURE2D_DESC	TextureDesc;
	ZeroMemory(&TextureDesc, sizeof TextureDesc);

	TextureDesc.Width = width;
	TextureDesc.Height = height;
	TextureDesc.MipLevels = 1;
	TextureDesc.ArraySize = 1;
	TextureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	TextureDesc.SampleDesc.Quality = 0;
	TextureDesc.SampleDesc.Count = 1;

	TextureDesc.Usage = D3D11_USAGE_DYNAMIC;
	TextureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	TextureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	TextureDesc.MiscFlags = 0;

	_uint* pPixel = new _uint[TextureDesc.Width * TextureDesc.Height];
	ZeroMemory(pPixel, sizeof(_uint) * TextureDesc.Width * TextureDesc.Height);

	for (_uint i = 0; i < TextureDesc.Height; ++i)
	{
		for (_uint j = 0; j < TextureDesc.Width; ++j)
		{
			_uint		iIndex = i * TextureDesc.Width + j;

			pPixel[iIndex] = D3DCOLOR_ARGB(255, 255, 255, 255);
		}
	}

	D3D11_SUBRESOURCE_DATA		SubResourceData;
	ZeroMemory(&SubResourceData, sizeof SubResourceData);

	SubResourceData.pSysMem = pPixel;
	SubResourceData.SysMemPitch = sizeof(_uint) * TextureDesc.Width;

	if (FAILED(m_pDevice->CreateTexture2D(&TextureDesc, &SubResourceData, &pTexture2D)))
		return E_FAIL;

	for (_uint i = 0; i < TextureDesc.Height; ++i)
	{
		for (_uint j = 0; j < TextureDesc.Width; ++j)
		{
			_uint		iIndex = i * TextureDesc.Width + j;

			//if (j < 50)
			//	pPixel[iIndex] = D3DCOLOR_ARGB(255, 255, 255, 255);
			//else
			pPixel[iIndex] = D3DCOLOR_ARGB(255, 0, 0, 0);

		}
	}

	D3D11_MAPPED_SUBRESOURCE	MappedSubResource;
	ZeroMemory(&MappedSubResource, sizeof MappedSubResource);

	m_pContext->Map(pTexture2D, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedSubResource);

	memcpy(MappedSubResource.pData, pPixel, sizeof(_uint) * TextureDesc.Width * TextureDesc.Height);

	m_pContext->Unmap(pTexture2D, 0);

	if (FAILED(SaveDDSTextureToFile(m_pContext, pTexture2D, m_TexturePath[iTextureIndex].c_str())))
		return E_FAIL;

	Safe_Release(pTexture2D);
	Safe_Delete_Array(pPixel);
	
	Safe_Release(m_SRVs[iTextureIndex]);
	Safe_Release(m_Textures[iTextureIndex]);

	ID3D11ShaderResourceView* pSRV = { nullptr };
	ID3D11Texture2D* pTexture = { nullptr };

	CreateDDSTextureFromFile(m_pDevice, m_TexturePath[iTextureIndex].c_str(), (ID3D11Resource**)&pTexture, &pSRV);

	m_Textures[iTextureIndex] = pTexture;
	m_SRVs[iTextureIndex] = pSRV;

	return S_OK;
}

HRESULT CTexture::Update_Texture_Pixels_Info(_uint iTextureIndex, _uint* pPixel)
{
	if (iTextureIndex >= m_iNumTextures)
		return E_FAIL;

	D3D11_TEXTURE2D_DESC	SourTextureDesc{};
	D3D11_TEXTURE2D_DESC	DestTextureDesc{};

	m_Textures[iTextureIndex]->GetDesc(&SourTextureDesc);

	ID3D11Texture2D* pTexture2D = { nullptr };

	D3D11_TEXTURE2D_DESC	TextureDesc;
	ZeroMemory(&TextureDesc, sizeof TextureDesc);

	TextureDesc.Width = SourTextureDesc.Width;
	TextureDesc.Height = SourTextureDesc.Height;
	TextureDesc.MipLevels = 1;
	TextureDesc.ArraySize = 1;
	TextureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	TextureDesc.SampleDesc.Quality = 0;
	TextureDesc.SampleDesc.Count = 1;

	TextureDesc.Usage = D3D11_USAGE_DYNAMIC;
	TextureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	TextureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	TextureDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA		SubResourceData;
	ZeroMemory(&SubResourceData, sizeof SubResourceData);

	_uint* pInPixel = new _uint[TextureDesc.Width * TextureDesc.Height];
	ZeroMemory(pInPixel, sizeof(_uint) * TextureDesc.Width * TextureDesc.Height);

	for (_uint i = 0; i < TextureDesc.Height; ++i)
	{
		for (_uint j = 0; j < TextureDesc.Width; ++j)
		{
			_uint		iIndex = i * TextureDesc.Width + j;

			pInPixel[iIndex] = D3DCOLOR_ARGB(255, 255, 255, 255);
		}
	}

	SubResourceData.pSysMem = pInPixel;
	SubResourceData.SysMemPitch = sizeof(_uint) * TextureDesc.Width;

	if (FAILED(m_pDevice->CreateTexture2D(&TextureDesc, &SubResourceData, &pTexture2D)))
		return E_FAIL;

	for (_uint i = 0; i < TextureDesc.Height; ++i)
	{
		for (_uint j = 0; j < TextureDesc.Width; ++j)
		{
			_uint		iIndex = i * TextureDesc.Width + j;

			pInPixel[iIndex] = pPixel[iIndex];
		}
	}


	D3D11_MAPPED_SUBRESOURCE	MappedSubResource;
	ZeroMemory(&MappedSubResource, sizeof MappedSubResource);

	m_pContext->Map(pTexture2D, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedSubResource);

	memcpy(MappedSubResource.pData, pInPixel, sizeof(_uint) * TextureDesc.Width * TextureDesc.Height);

	m_pContext->Unmap(pTexture2D, 0);

	if (FAILED(SaveDDSTextureToFile(m_pContext, pTexture2D, m_TexturePath[iTextureIndex].c_str())))
		return E_FAIL;

	Safe_Release(pTexture2D);
	Safe_Delete_Array(pInPixel);

	Safe_Release(m_SRVs[iTextureIndex]);
	Safe_Release(m_Textures[iTextureIndex]);

	ID3D11ShaderResourceView* pSRV = { nullptr };
	ID3D11Texture2D* pTexture = { nullptr };

	CreateDDSTextureFromFile(m_pDevice, m_TexturePath[iTextureIndex].c_str(), (ID3D11Resource**)&pTexture, &pSRV);

	m_Textures[iTextureIndex] = pTexture;
	m_SRVs[iTextureIndex] = pSRV;

	return S_OK;
}

HRESULT CTexture::Copy_Texture_Pixels(_uint iTextureIndex, _uint** pPixel)
{
	if (iTextureIndex >= m_iNumTextures)
		return E_FAIL;

	Safe_Delete_Array(*pPixel);

	ID3D11Texture2D* pStagingTexture = nullptr;

	D3D11_TEXTURE2D_DESC	SourTextureDesc{};
	D3D11_TEXTURE2D_DESC	DestTextureDesc{};

	m_Textures[iTextureIndex]->GetDesc(&SourTextureDesc);

	*pPixel = new _uint[SourTextureDesc.Width * SourTextureDesc.Height];

	DestTextureDesc.Width = SourTextureDesc.Width;
	DestTextureDesc.Height = SourTextureDesc.Height;
	DestTextureDesc.MipLevels = SourTextureDesc.MipLevels;
	DestTextureDesc.ArraySize = SourTextureDesc.ArraySize;
	DestTextureDesc.Format = SourTextureDesc.Format;

	DestTextureDesc.SampleDesc = SourTextureDesc.SampleDesc;

	DestTextureDesc.Usage = D3D11_USAGE_STAGING;
	DestTextureDesc.BindFlags = 0;
	DestTextureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	DestTextureDesc.MiscFlags = 0;

	if (FAILED(m_pDevice->CreateTexture2D(&DestTextureDesc, nullptr, &pStagingTexture)))
		return E_FAIL;

	if (nullptr == pStagingTexture)
		return E_FAIL;

	m_pContext->CopyResource(pStagingTexture, m_Textures[iTextureIndex]);

	// now, map the staging resource
	D3D11_MAPPED_SUBRESOURCE mapInfo;
	HRESULT hr = m_pContext->Map(
		pStagingTexture,
		0,
		D3D11_MAP_READ,
		0,
		&mapInfo);

	memcpy(*pPixel, mapInfo.pData, sizeof(_uint) * SourTextureDesc.Width * SourTextureDesc.Height);

	m_pContext->Unmap(pStagingTexture, 0);

	Safe_Release(pStagingTexture);

	return S_OK;
}

HRESULT CTexture::Initialize_Prototype(const _tchar* pTextureFilePath, _uint iNumTextures)
{
	m_iNumTextures = iNumTextures;

	m_Textures.reserve(m_iNumTextures);
	m_SRVs.reserve(m_iNumTextures);
	m_TexturePath.reserve(m_iNumTextures);

	for (_uint i = 0; i < iNumTextures; ++i)
	{
		TCHAR		szFilePath[128] = L"";

		wsprintfW(szFilePath, pTextureFilePath, i);

		_tchar szDir[MAX_PATH] = TEXT("");
		_tchar szFileName[MAX_PATH] = TEXT("");
		_tchar szEXT[MAX_PATH] = TEXT("");

		_wsplitpath_s(szFilePath, nullptr, 0, szDir, MAX_PATH, szFileName, MAX_PATH, szEXT, MAX_PATH);

		ID3D11ShaderResourceView* pSRV = { nullptr };
		ID3D11Texture2D* pTexture = { nullptr };
		wstring	texturePath = szFilePath;

		if (!lstrcmp(szEXT, TEXT(".dds")))
		{
			CreateDDSTextureFromFile(m_pDevice, szFilePath, (ID3D11Resource**)&pTexture, &pSRV);
		}
		else if (!lstrcmp(szEXT, TEXT(".tga")))
			return E_FAIL;
		else
			CreateWICTextureFromFile(m_pDevice, szFilePath, (ID3D11Resource**)&pTexture, &pSRV);

		if (nullptr == pTexture)
			return E_FAIL;

		m_SRVs.push_back(pSRV);
		m_Textures.push_back(pTexture);
		m_TexturePath.push_back(texturePath);
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
	m_SRVs.clear();

	for (_uint i = 0; i < m_Textures.size(); ++i)
	{
		Safe_Release(m_Textures[i]);
	}
	m_Textures.clear();

	//for (auto iter = m_TexturePath.begin(); iter != m_TexturePath.end();)
	//{
	//	iter = m_TexturePath.erase(iter);
	//}
	m_TexturePath.clear();

}
