#pragma once

#include "Client_Defines.h"
#include "Level.h"

BEGIN(Client)

class CLevel_Stage final : public CLevel
{
private:
	CLevel_Stage(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLevel_Stage() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void Tick(_double TimeDelta) override;

private: /* 이 레벨이 사용하기위한 사본 객체들을 그룹별로 생성하낟. */
	HRESULT Ready_Layer_Manager(const _tchar* pLayerTag);

	HRESULT Ready_Layer_Terrain(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Camera(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Env(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Map(const _tchar* pLayerTag);

	HRESULT Ready_Layer_Effect(const _tchar* pLayerTag);
	HRESULT Ready_Layer_PlayerSkill(const _tchar* pLayerTag);
	HRESULT Ready_Layer_MonsterSkill(const _tchar* pLayerTag);

	HRESULT Ready_Layer_PlayerSearcher(const _tchar* pLayerTag);
	HRESULT Ready_Layer_MonsterSearcher(const _tchar* pLayerTag);

	HRESULT Ready_Layer_Monster(const _tchar* pLayerTag);
		
	HRESULT Ready_Layer_Player(const _tchar* pLayerTag);
	HRESULT Ready_Layer_UI(const _tchar* pLayerTag);

	HRESULT Ready_Layer_CameraTarget(const _tchar* pLayerTag);

	HRESULT	Ready_LightDesc();
public:
	static CLevel_Stage* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;

};

END