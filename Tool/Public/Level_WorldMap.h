#pragma once

#include "Client_Defines.h"
#include "Level.h"

BEGIN(Client)

class CLevel_WorldMap final : public CLevel
{
private:
	CLevel_WorldMap(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLevel_WorldMap() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void Tick(_double TimeDelta) override;

private: /* �� ������ ����ϱ����� �纻 ��ü���� �׷캰�� �����ϳ�. */
	HRESULT Ready_Layer_Manager(const _tchar* pLayerTag);

	HRESULT Ready_Layer_Terrain(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Camera(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Env(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Map(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Stage_Point(const _tchar* pLayerTag);

	HRESULT Ready_Layer_Effect(const _tchar* pLayerTag);
	HRESULT Ready_Layer_PlayerSkill(const _tchar* pLayerTag);

	HRESULT Ready_Layer_Player(const _tchar* pLayerTag);
	HRESULT Ready_Layer_UI(const _tchar* pLayerTag);


	HRESULT	Ready_LightDesc();
public:
	static CLevel_WorldMap* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;

};

END