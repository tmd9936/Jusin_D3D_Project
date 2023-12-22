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

class CStageEnv final : public CGameObject
{
public:
	typedef struct Stage_Env_Desc
	{
		_float3		vPos;
		_float3		vScale;
		_float3		vRotate;

		wstring		ModelPrototypeTag;
	} STAGE_ENV_DESC;

private:
	explicit CStageEnv(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CStageEnv(const CStageEnv& rhs);
	virtual ~CStageEnv() = default;

public:
	virtual HRESULT			Initialize_Prototype() override;
	virtual HRESULT			Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg) override;
	virtual HRESULT			Initialize(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath);

	virtual _uint			Tick(_double TimeDelta) override;
	virtual _uint			LateTick(_double TimeDelta) override;
	virtual HRESULT			Render() override;
	virtual HRESULT			Render_Laplacian() override;
	virtual HRESULT			Render_ShadowDepth() override;


public:
	void On_CollisionEnter(CCollider* pOther, const _float& fX, const _float& fY, const _float& fZ);
	void On_Collision(CCollider* pOther, const _float& fX, const _float& fY, const _float& fZ);
	void On_CollisionExit(CCollider* pOther, const _float& fX, const _float& fY, const _float& fZ);


protected:
	virtual _bool			Save_By_JsonFile_Impl(Document& doc, Document::AllocatorType& allocator);
	virtual _bool			Load_By_JsonFile_Impl(Document& doc);


private:
	HRESULT					Add_Components();
	HRESULT					Add_Components_By_File();

	HRESULT					SetUp_ShaderResources();

	HRESULT					SetUp_Shadow_ShaderResources();

private:
	CTransform*				m_pTransformCom = { nullptr };
	CRenderer*				m_pRendererCom = { nullptr };
	CShader*				m_pShaderCom = { nullptr };
	CModel*					m_pModelCom = { nullptr };
	CAABB*					m_pAABB = { nullptr };

private:
	STAGE_ENV_DESC			m_StageEnvDesc = { };

public:
	static CStageEnv* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg = nullptr) override;
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath = nullptr);
	virtual void Free() override;
};

END