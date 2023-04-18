#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)

END

BEGIN(Client)

class CEnemyPack final : public CGameObject
{
public:
	typedef struct Regist_Data_Desc
	{
		string		m_enemyFilePath;
		_uint		m_enemyNumber;
		_bool		m_isBoss;
		_uint		m_setType;  // 적의 데이터 타입(아마 포켓몬마다 던전에 맞춘 데이터파일이 있는데 그거 말하는 듯)
		_uint		m_setIndex; // 몇 번째에 스폰될지
		_uint		m_spawnMode; // 스폰모드 (처음에 스폰, 1라운드 끝나면 스폰, 기습 스폰, 보스 스폰..)

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
		TYPE_PRE_CREATE,
		TYPE_NORMAL_APPEARANCE,
		TYPE_SURPIRSE_APPEARANCE,
		TYPE_BOSS_APPEARANCE,
		TYPE_END
	};

private:
	CEnemyPack(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEnemyPack(const CEnemyPack& rhs);
	virtual ~CEnemyPack() = default;

public:
	virtual HRESULT			Initialize_Prototype() override;
	virtual HRESULT			Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg) override;
	virtual HRESULT			Initialize(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath);


	virtual _uint			Tick(_double TimeDelta) override;
	virtual _uint			LateTick(_double TimeDelta) override;
	virtual HRESULT			Render() override;

protected:
	virtual _bool			Save_By_JsonFile_Impl(Document& doc, Document::AllocatorType& allocator);
	virtual _bool			Load_By_JsonFile_Impl(Document& doc);

private:
	HRESULT					Add_Components();
	HRESULT					Add_Components_By_Json();
	HRESULT					SetUp_ShaderResources();

private:
	ENEMY_PACK_DESC		m_Desc = { };

public:
	static CEnemyPack* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg = nullptr) override;
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath = nullptr);
	virtual void Free() override;
};

END