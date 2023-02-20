#pragma once

#include "Component.h"


BEGIN(Engine)

class ENGINE_DLL CTexture : public CComponent
{
private:
	CTexture(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTexture(const CTexture& rhs);
	virtual ~CTexture() = default;

public:
	virtual HRESULT Initialize_Prototype(const _tchar* pTextureFilePath, _uint iNumTextures);
	virtual HRESULT Initialize(void* pArg);

public:
	ID3D11ShaderResourceView* Get_SRV(const _uint& index) {
		if (m_iNumTextures <= index)
			return nullptr;
		return m_SRVs[index];
	}

public:
	HRESULT Set_ShaderResource(class CShader* pShader, const char* pConstantName, _uint iTextureIndex = 0);

private:
	vector<ID3D11ShaderResourceView*>			m_SRVs;
	_uint										m_iNumTextures = { 0 };

public:
	static CTexture* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pTextureFilePath, _uint iNumTextures = 1);
	virtual CComponent* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END

