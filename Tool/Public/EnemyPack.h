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
		_uint		m_setType;  // ���� ������ Ÿ��(�Ƹ� ���ϸ󸶴� ������ ���� ������������ �ִµ� �װ� ���ϴ� ��)
		_uint		m_setIndex; // �� ��°�� ��������
		_uint		m_spawnMode; // ������� (ó���� ����, 1���� ������ ����, ��� ����, ���� ����..)

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