#pragma once

#include "Component.h"


BEGIN(Engine)

class ENGINE_DLL CTexture : public CComponent
{
public:
	static const FamilyId familyId = FAMILY_ID_TEXTURE;

private:
	explicit CTexture(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CGameObject* pOwner);
	explicit CTexture(const CTexture& rhs, CGameObject* pOwner);
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
	HRESULT	Set_ShaderResourceArray(CShader* pShader, const char* pConstantName);

public:
	void Set_BrushPos(_float4 vBrushPos) {
		m_vBrushPos = vBrushPos;
	}
	const _float4 Get_BrushPos() const {
		return m_vBrushPos;
	}

private:
	vector<ID3D11ShaderResourceView*>			m_SRVs;
	_uint										m_iNumTextures = { 0 };
	_float4										m_vBrushPos = { 0.f, 0.f, 0.f, 1.f };

public:
	static CTexture* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pTextureFilePath, _uint iNumTextures = 1);
	virtual CComponent* Clone(CGameObject* pOwner, void* pArg = nullptr) override;
	virtual void Free() override;
};

END

