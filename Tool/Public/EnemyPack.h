#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTransform;
class CNavigation;
class CHP;
END

BEGIN(Client)

class CStage_Manager;
class CStageEnemyMonster;
class CStageProgressUI;
class CStageClearUI;

class CEnemyPack final : public CGameObject
{
public:
	typedef struct Wave_Progress_Desc
	{
		_uint		m_setIndex;
		_uint		m_waveIndex;
		_float		m_progress;
	} WAVE_PROGRESS_DESC;

public:
	typedef struct Regist_Data_Desc
	{
		string		m_enemyFilePath;
		_uint		m_enemyNumber;
		_bool		m_isBoss;
		_uint		m_waveIndex;  // 몇 번째 웨이브인지
		_uint		m_setIndex; // 몇 번째 EnemySpawnPoint에 스폰될지
		_uint		m_spawnMode; // 스폰모드 (처음에 스폰, 1라운드 끝나면 스폰, 기습 스폰, 보스 스폰..)
		_float		m_progress;

	} REGIST_DATA_DESC;


public:
	typedef struct Enemy_Pack_Desc
	{
		wstring						m_Name;
		vector<REGIST_DATA_DESC>    m_registDatas;

	} ENEMY_PACK_DESC;

public:
	enum  TYPE
	{
		TYPE_SPAWN_START,
		TYPE_SPAWN_AFTER_ROUND_END,
		TYPE_SPAWN_IN_THE_MIDDLE_OF_ROUND,
		TYPE_SPAWN_WITH_BOSS,
		TYPE_SPAWN_BOSS,
		TYPE_END
	};

private:
	explicit CEnemyPack(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CEnemyPack(const CEnemyPack& rhs);
	virtual ~CEnemyPack() = default;

public:
	virtual HRESULT			Initialize_Prototype() override;
	virtual HRESULT			Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg) override;
	virtual HRESULT			Initialize(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath);

	virtual _uint			Tick(_double TimeDelta) override;
	virtual _uint			LateTick(_double TimeDelta) override;
	virtual HRESULT			Render() override;

public:
	_bool					Next_Spawn(_float3 vPosition, const _float& radius);
	_bool					Is_CanNextSpawn() {
		return m_CanNextSpawn;
	}

	void					Check_CanNextSpawn();
	_int					Get_NextEnemyPack() {
		return m_NextEnemyPack;
	}

protected:
	virtual _bool			Save_By_JsonFile_Impl(Document& doc, Document::AllocatorType& allocator);
	virtual _bool			Load_By_JsonFile_Impl(Document& doc);

private:
	HRESULT					Insert_In_Stage_Manager();

	HRESULT					Create_EnemyPack();

	void					Set_WaveProgress();

private:
	HRESULT					Add_Components();
	HRESULT					Add_Components_By_Json();
	HRESULT					SetUp_ShaderResources();

private:
	ENEMY_PACK_DESC							m_Desc = { };
	vector<vector<CStageEnemyMonster*>*>	m_EnemyPack;
	vector<WAVE_PROGRESS_DESC>				m_progresses;
	_int									m_NextEnemyPack = { 0 };
	_bool									m_CanNextSpawn = { true };

public:
	static CEnemyPack* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg = nullptr) override;
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath = nullptr);
	virtual void Free() override;
};

END