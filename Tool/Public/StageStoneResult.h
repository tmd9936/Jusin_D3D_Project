#pragma once
#include "UI.h"

BEGIN(Client)

class CStage_Manager;
class CStoneInfoUI;

class CStageStoneResult :
	public CUI
{
public:
	enum STATE
	{
		STATE_OPEN,
		STATE_STAY,
		STATE_CLOSE,
		STATE_END
	};

private:
	explicit CStageStoneResult(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CStageStoneResult(const CStageStoneResult& rhs);
	virtual ~CStageStoneResult() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg) override;
	virtual HRESULT Initialize(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath);

public:
	virtual _uint Tick(_double TimeDelta) override;
	virtual _uint LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

public:
	void			OpenUI();

public:
	HRESULT			Init_StoneInfoUIs();

private:
	void			Change_State();
	void			State_Tick(const _double& TimeDelta);

private:
	STATE			m_eCurState = { STATE_END };
	STATE			m_ePreState = { STATE_END };

	CStoneInfoUI*	m_pStoneInfoUI01 = { nullptr };
	CStoneInfoUI*	m_pStoneInfoUI02 = { nullptr };
	CStoneInfoUI*	m_pStoneInfoUI03 = { nullptr };

public:
	static CStageStoneResult* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg = nullptr) override;
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath = nullptr) override;
	virtual void Free() override;
};

END