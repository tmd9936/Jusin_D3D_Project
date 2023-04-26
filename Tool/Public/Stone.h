#pragma once
#include "UI.h"

BEGIN(Client)

class CStone :
	public CUI
{
public:
	enum STATE
	{
		STATE_IN_INVENTORY_NO_EQUIP,
		STATE_IN_INVENTORY_EQUIP,
		STATE_PICKING,
		STATE_IN_EQUIPINFO,
		STATE_END
	};

private:
	explicit CStone(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CStone(const CStone& rhs);
	virtual ~CStone() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg) override;
	virtual HRESULT Initialize(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath);

public:
	virtual _uint Tick(_double TimeDelta) override;
	virtual _uint LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

private:
	void			Change_State();
	void			State_Tick(const _double& TimeDelta);

private:
	STATE			m_eCurState = { STATE_END };
	STATE			m_ePreState = { STATE_END };

	STONE_EQUIP_TYPE m_stoneEquipType = { STONE_EQUIP_TYPE::TYPE_ATK };

public:
	static CStone* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg = nullptr) override;
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath = nullptr) override;

	virtual void Free() override;
};

END