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
class CHP;
END

BEGIN(Client)

class CHpBar final : public CGameObject
{
public:
	typedef struct HpBar_Desc
	{
		CTransform*			pParent = { nullptr };
		CHP*				pParentHpCom = { nullptr };

		_float4x4			PivotMatrix;

		_float				m_fPositionX;
		_float				m_fPositinoY;
		_float				m_fPositinoZ;
		
		_float				m_fSizeX;
		_float				m_fSizeY;

		_tchar				m_TextureProtoTypeName[MAX_PATH];
		_uint				m_TextureLevelIndex;

		_float4				m_vHpColor;

	} HPBAR_DESC;
private:
	CHpBar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CHpBar(const CHpBar& rhs);
	virtual ~CHpBar() = default;

public:
	virtual HRESULT Initialize_Prototype() override; /* 원형객체의 초기화작업 */
	virtual HRESULT Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg) override; /* 사본객체의 초기화작업 */
	virtual _uint Tick(_double TimeDelta) override;
	virtual _uint LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;


public:
	void		Set_HpRatio(_float ratio) {
		m_HpRatio = ratio;
	}

private:
	CTransform* m_pTransformCom = { nullptr };
	CRenderer* m_pRendererCom = { nullptr };
	CShader* m_pShaderCom = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };

private:
	HPBAR_DESC			m_Desc = {};

	_float4x4			m_FinalWorldMatrix; /* 원점기준 (내 월드 * 부모월드) */

	_float4x4			m_ViewMatrix = {};
	_float4x4			m_ProjMatrix = {};

	_float				m_HpRatio = { 0.7f };

private:
	HRESULT Add_Components();
	HRESULT SetUp_ShaderResources();
	_matrix Remove_Scale(_fmatrix Matrix);


public:
	/* Prototype */
	/* 원형 객체를 생성한다. */
	static CHpBar* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	/* 사본 객체를 생성한다. */
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg = nullptr) override;
	virtual void Free() override;
};

END