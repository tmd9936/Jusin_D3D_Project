#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTransform;
class CRenderer;
class CTexture;
class CShader;
class CModel;
END

BEGIN(Client)

class CCauldron final : public CGameObject
{
public:
	enum STATE
	{
		STATE_BREAK,
		STATE_CHANGE,
		STATE_CLOSE,
		STATE_COOK,
		STATE_FIX,
		STATE_IDLE,
		STATE_PRESS,
		STATE_SELECT,
		STATE_SET,
		STATE_END
	};

public:
	typedef struct Cauldron_Desc
	{
		_float3		vPos;
		_uint		eState;
	} CAULDRON_DESC;

private:
	explicit CCauldron(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CCauldron(const CCauldron& rhs);
	virtual ~CCauldron() = default;

public:
	virtual HRESULT		Initialize_Prototype() override; 
	virtual HRESULT		Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg) override; 

	virtual _uint		Tick(_double TimeDelta) override;
	virtual _uint		LateTick(_double TimeDelta) override;
	virtual HRESULT		Render() override;
	virtual HRESULT		Render_Laplacian() override;


private:
	HRESULT Add_Components();
	HRESULT SetUp_ShaderResources();

private:
	CTransform*		m_pTransformCom = { nullptr };
	CRenderer*		m_pRendererCom = { nullptr };
	CShader*		m_pShaderCom = { nullptr };
	CModel*			m_pModelCom = { nullptr };

private:
	CAULDRON_DESC	m_Desc = { };
	STATE			m_eCurState = { STATE_END };

public:
	static CCauldron* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg = nullptr) override;
	virtual void Free() override;
};

END