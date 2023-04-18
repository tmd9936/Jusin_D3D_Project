#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
#include "StageCamera.h"
#include "EnemySpawnPoint.h"

BEGIN(Engine)
class CCamera;
class CVIBuffer_Rect;
class CRenderer;
class CShader;
class CTexture;
class CTransform;

END

BEGIN(Client)

class CStageCamera;
class CPokemonSkillButton;
class CMonster;
class CEffect_Manager;
class CPokering;
class CEnemySpawnPoint;
class CEnemyPack;
class CStageMessageInfo;

class CStage_Manager final : public CGameObject
{
public:
	enum WORLDMAP_MANAGER_STATE {
		MANAGER_FADE_IN,
		MANAGER_IDLE,
		MANAGER_OPEN_STATE_INFO,
		MANAGER_END
	};

public:
	typedef struct WorldMap_Manager_Desc
	{
		_float m_FadeSecond;
		_float4 m_FadeInStartColor;
		_float4 m_FadeInEndColor;

	} WORLDMAP_MANAGER_DESC;

private:
	CStage_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CStage_Manager(const CStage_Manager& rhs);
	virtual ~CStage_Manager() = default;

public:
	virtual HRESULT				Initialize_Prototype() override;
	virtual HRESULT				Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg) override;
	virtual HRESULT				Initialize(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath);

	virtual _uint				Tick(_double TimeDelta) override;
	virtual _uint				LateTick(_double TimeDelta) override;
	virtual HRESULT				Render() override;

public:
	void	Be_Idle() {
		m_eCurState = MANAGER_IDLE;
	}

	void						Boss_DeadEffect(_bool isEnd, _fvector vPos);
	void						Add_EnemySpawnPoint(CEnemySpawnPoint* pEnemySpawnPoint);
	void						Set_EnemyPack(CEnemyPack* pEnemyPack);
	void						Set_StageMessageInfo(CStageMessageInfo* pStageMessageInfo);

private:
	HRESULT						Init_ManagerInfo();
	HRESULT						Init_PlayersPos();

private:
	void						Fade_In(const _double& TimeDelta);

protected:
	virtual _bool				Save_By_JsonFile_Impl(Document& doc, Document::AllocatorType& allocator);
	virtual _bool				Load_By_JsonFile_Impl(Document& doc);

private:
	HRESULT						Add_Components();
	HRESULT						Add_Components_By_File();

	HRESULT						SetUp_ShaderResources();

private:
	void						State_Tick(const _double& TimeDelta);
	void						Change_State();

private:
	WORLDMAP_MANAGER_DESC		m_Desc = {};
	WORLDMAP_MANAGER_STATE		m_ePreState = { MANAGER_END };
	WORLDMAP_MANAGER_STATE		m_eCurState = { MANAGER_FADE_IN };

	CStageCamera*				m_pMainCamera = { nullptr };
	CEnemyPack*					m_pEnemyPack = { nullptr };
	CStageMessageInfo*			m_pStageMessageInfo = { nullptr };
	vector<CEnemySpawnPoint*>	m_enemySpawnPoints;

	_int						m_CurrentEnemyCount = { 0 };

private:
	CTransform*					m_pTransformCom = { nullptr };
	CRenderer*					m_pRendererCom = { nullptr };
	CShader*					m_pShaderCom = { nullptr };
	CVIBuffer_Rect*				m_pVIBufferCom = { nullptr };
	CTexture*					m_pTextureCom = { nullptr };

private:
	_float						m_fX = { 0.f };
	_float						m_fY = { 0.f };
	_float						m_fSizeX = { 0.f };
	_float						m_fSizeY = { 0.f };

	_float4x4					m_ViewMatrix = {};
	_float4x4					m_ProjMatrix = {};

	_float4						m_vCurrentFadeColor = {};
	_double						m_fCurrentFadeTime = { 0.f };

	size_t						m_CurrentSpawnIndex = { 0 };

public:
	static CStage_Manager* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg = nullptr) override;
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath = nullptr);
	virtual void Free() override;
};

END