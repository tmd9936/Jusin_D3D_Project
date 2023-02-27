#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CShader final : public CComponent
{
public:
	static const FamilyId familyId = FAMILY_ID_SHADER;

public:
	typedef struct tagPassDesc
	{
		ID3DX11EffectPass* pPass = { nullptr };
		ID3D11InputLayout* pInputLayout = { nullptr };

	}PASSDESC;
private:
	explicit CShader(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CGameObject* pOwner);
	explicit CShader(const CShader& rhs, CGameObject* pOwner);
	virtual ~CShader() = default;

public:
	virtual HRESULT Initialize_Prototype(const _tchar* pShaderFilePath, D3D11_INPUT_ELEMENT_DESC* pElements, _uint iNumElements);
	virtual HRESULT Initialize(void* pArg) override;

public:
	HRESULT Begin(_uint iPassIndex);
	HRESULT	Set_Matrix(const char* pConstantName, _float4x4* pMatrix);
	HRESULT Set_ShaderResourceView(const char* pConstantName, ID3D11ShaderResourceView* pSRV);

	HRESULT	Set_Matrix(const char* pConstantName, const _float4x4& pMatrix);
	HRESULT Set_ShaderResourceView(const char* pConstantName, ID3D11ShaderResourceView& pSRV);

	HRESULT	Set_RawValue(const char* pConstantName, const void* pData, _uint iLength);


private:
	ID3DX11Effect* m_pEffect = { nullptr };

private:
	vector<PASSDESC>			m_Passes;
	_uint						m_iNumPasses = { 0 };

public:
	static CShader* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pShaderFilePath, D3D11_INPUT_ELEMENT_DESC* pElements, _uint iNumElements);
	virtual CComponent* Clone(CGameObject* pOwner, void* pArg = nullptr) override;
	virtual void Free();
};

END