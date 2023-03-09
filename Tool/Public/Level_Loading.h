#pragma once

#include "Client_Defines.h"
#include "Level.h"

/* ���������� �غ��ϱ����� CLoader�� �����Ѵ�. */
/* �δ��� ��Ȱ�� ������ ���� �Ҵ��ϱ����� ���������� �ѱ��. */
/* �ε��������� ��������� ��ü�� �����ϰ� ����, ������ �����Ѵ�. */
BEGIN(Client)

class CLevel_Loading final : public CLevel
{
private:
	CLevel_Loading(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLevel_Loading() = default;

public:
	virtual HRESULT Initialize(LEVEL eNextLevelID);
	virtual void Tick(_double TimeDelta) override;

private:
	HRESULT Ready_Layer_BackGround();
	HRESULT Ready_Layer_UI(const _tchar* pLayerTag);

private:
	LEVEL			m_eNextLevelID = { LEVEL_END };
	class CLoader* m_pLoader = { nullptr };

private:
	MYFLOAT4			m_LoadindBackBufferColor = { 0.156f, 0.109f, 0.f, 1.f };

public:
	static CLevel_Loading* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVEL eNextLevelID);
	virtual void Free() override;

};

END