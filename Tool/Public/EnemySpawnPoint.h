#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTransform;
class CRenderer;
class CPickingCube;
class CSphere;
END

/*
스폰 리스트를 벡터로 담은 다음에
조건에 맞춰서 스폰을 순차적으로 진행을 함.
스폰시에 EnemyPack에 있는 스폰 인덱스를 확인해서
현재 스폰 인덱스에 맞는 몬스터를 EnemyPack에서 찾아서 스폰을 함
스폰시의 효과는 스폰 포인트에 있는 스폰 타입에 맞춰서 카메라를 움직이거나
텍스트를 출력

*/

BEGIN(Client)

class CStage_Manager;

class CEnemySpawnPoint final : public CGameObject
{
public:
	typedef struct Enemy_Spawn_Point_Desc
	{
		_float3		m_position;
		_uint		m_type;
		_float		m_spawnRadius;

	} ENEMY_SPAWN_POINT_DESC;

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
	CEnemySpawnPoint(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEnemySpawnPoint(const CEnemySpawnPoint& rhs);
	virtual ~CEnemySpawnPoint() = default;

public:
	virtual HRESULT				Initialize_Prototype() override;
	virtual HRESULT				Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg) override;
	virtual HRESULT				Initialize(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath);


	virtual _uint				Tick(_double TimeDelta) override;
	virtual _uint				LateTick(_double TimeDelta) override;
	virtual HRESULT				Render() override;

protected:
	virtual _bool				Save_By_JsonFile_Impl(Document& doc, Document::AllocatorType& allocator);
	virtual _bool				Load_By_JsonFile_Impl(Document& doc);

private:
	HRESULT						Add_Components();
	HRESULT						Add_Components_By_Json();
	HRESULT						SetUp_ShaderResources(); 

private:
	HRESULT						Insert_In_Stage_Manager();

private:
	CTransform*					m_pTransformCom = { nullptr };
	CRenderer*					m_pRendererCom = { nullptr };
	CPickingCube*				m_pPickingCube = { nullptr };
	CSphere*					m_pSphere = { nullptr };

private:
	ENEMY_SPAWN_POINT_DESC		m_Desc = { };
	_bool						m_bSpawnComplite = { false };

public:
	static CEnemySpawnPoint* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg = nullptr) override;
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath = nullptr);
	virtual void Free() override;
};

END