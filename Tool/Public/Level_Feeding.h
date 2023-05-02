#pragma once

#include "Client_Defines.h"
#include "Level.h"

BEGIN(Client)

class CLevel_Feeding final : public CLevel
{
private:
	CLevel_Feeding(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLevel_Feeding() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void Tick(_double TimeDelta) override;

private:
	HRESULT Ready_Layer_BackGround(const _tchar* pLayerTag);
	HRESULT Ready_Layer_UI(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Player(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Manager(const _tchar* pLayerTag);

private:
	MYFLOAT4			m_FeedingBackBufferColor = { 0.9221f, 0.85607f, 0.67843f, 1.f };

public:
	static CLevel_Feeding* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;

};

END