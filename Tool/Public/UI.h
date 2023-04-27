#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
#include "PartText.h"
#include "PartTexture.h"

BEGIN(Engine)
class CVIBuffer_Rect;
class CRenderer;
class CShader;
class CTexture;
class CTransform;
END

BEGIN(Client)

class CUI : public CGameObject
{
public:
	enum UI_TYPE {
		UI_TYPE_TEXTURE,
		UI_TYPE_COLOR_TEXTURE,
		UI_TYPE_END
	};

public:
	typedef struct UI_Desc
	{
		_float				m_fX;
		_float				m_fY;
		_float				m_fSizeX;
		_float				m_fSizeY;
		_uint				m_UIType;
		_float4				m_vColor;
		_uint				m_ShaderPass;
		_uint				m_TextureProtoTypeLevel;
		_tchar				m_TextureProtoTypeName[MAX_PATH];
	} UI_DESC;

protected:
	explicit CUI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CUI(const CUI& rhs);
	virtual ~CUI() = default;

public:
	virtual HRESULT			Initialize_Prototype() override; 
	virtual HRESULT			Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg) override; 
	virtual HRESULT			Initialize(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath);

public:
	virtual _uint			Tick(_double TimeDelta) override;
	virtual _uint			LateTick(_double TimeDelta) override;
	virtual HRESULT			Render() override;

public:
	void					Set_Color(_float4 vColor) {
		m_UIDesc.m_vColor = vColor;
	}

	HRESULT					Change_Texture(const _tchar* pPrototypeTag);

public:
	void					Set_Pos(const _float3& vPos);

public:
	virtual _bool			Check_Is_In();
	virtual _bool			Check_Is_In(const POINT& mousePT);

protected:
	virtual _bool			Save_By_JsonFile_Impl(Document& doc, Document::AllocatorType& allocator);
	virtual _bool			Load_By_JsonFile_Impl(Document& doc);

protected:
	virtual HRESULT			SetUp_ShaderResources();

private:
	HRESULT					Add_Components();
	HRESULT					Add_Components_By_File();

protected:
	CTransform*				m_pTransformCom = { nullptr };
	CRenderer*				m_pRendererCom = { nullptr };
	CShader*				m_pShaderCom = { nullptr };
	CVIBuffer_Rect*			m_pVIBufferCom = { nullptr };
	CTexture*				m_pTextureCom = { nullptr };

protected:
	UI_DESC					m_UIDesc = {};
	_float4x4				m_ViewMatrix = {};
	_float4x4				m_ProjMatrix = {};

protected:
	vector<CPartTexture*>	m_TextureParts;
	vector<CPartText*>		m_TextParts;

protected:
	_uint					m_TextureNumber = { 0 };

protected:
	const LONG				m_mouseInterSize = { 10 };

public:
	static CUI* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg = nullptr) override;
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath = nullptr);
	virtual void Free() override;
};

END