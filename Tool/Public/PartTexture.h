#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CVIBuffer_Rect;
class CRenderer;
class CShader;
class CTexture;
class CTransform;
class CModel;
END

BEGIN(Client)

class CPartTexture final : public CGameObject
{
public:
	enum TYPE {
		TYPE_TEXTURE,
		TYPE_COLOR_TEXTURE,
		TYPE_END
	};

public:
	typedef struct UI_Desc
	{
		CTransform*			pParent = { nullptr }; // 기준이 되는 부모
		CModel*				pParentModel = { nullptr }; // 기준이 되는 부모

		_uint				m_eType;
		_float4				m_vColor;
		_float				m_fX;
		_float				m_fY;
		_float				m_fSizeX;
		_float				m_fSizeY;

		_float				m_Progress;

		_uint				m_ShaderPass;
		_uint				m_TextureProtoTypeLevel;
		_tchar				m_TextureProtoTypeName[MAX_PATH];
	} UI_DESC;

private:
	CPartTexture(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPartTexture(const CPartTexture& rhs);
	virtual ~CPartTexture() = default;

public:
	virtual HRESULT Initialize_Prototype() override; /* 원형객체의 초기화작업 */
	virtual HRESULT Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg) override; /* 사본객체의 초기화작업 */
	virtual _uint Tick(_double TimeDelta) override;
	virtual _uint LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;	

public:
	void	Set_Parent_Model(CModel* pModel) {
		m_UIDesc.pParentModel = pModel;
	}

	void	Set_TextureNumber(_uint iNumber) {
		m_TextureNumber = iNumber;
	}

	void Set_Progress(_float progress) {
		m_UIDesc.m_Progress = progress;
	}

private:
	CTransform* m_pTransformCom = { nullptr };
	CRenderer* m_pRendererCom = { nullptr };
	CShader* m_pShaderCom = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };

private:
	UI_DESC		m_UIDesc = {};
	_float4x4	m_ViewMatrix = {};
	_float4x4	m_ProjMatrix = {};

	_float4x4	m_FinalWorldMatrix; /* 원점기준 (내 월드 * 부모월드) */

	TYPE		m_eType = { TYPE_END };

private:
	_uint		m_TextureNumber = { 0 };

private:
	HRESULT Add_Components();
	HRESULT SetUp_ShaderResources();

public:
	/* Prototype */
	/* 원형 객체를 생성한다. */
	static CPartTexture* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	/* 사본 객체를 생성한다. */
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg = nullptr) override;
	virtual void Free() override;
};

END