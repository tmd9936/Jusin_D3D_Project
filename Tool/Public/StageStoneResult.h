#pragma once
#include "UI.h"
#include "Stone.h"

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
	virtual HRESULT			Initialize_Prototype() override;
	virtual HRESULT			Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg) override;
	virtual HRESULT			Initialize(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath);

public:
	virtual _uint			Tick(_double TimeDelta) override;
	virtual _uint			LateTick(_double TimeDelta) override;
	virtual HRESULT			Render() override;

public:
	void					OpenUI(vector<CStone::STONE_DESC>& stoneDesces);

public:
	STATE					Get_CurrentState() const {
		return m_eCurState;
	}

public:
	HRESULT					Init_StoneInfoUIs();

private:
	void					Change_State();
	void					State_Tick(const _double& TimeDelta);

private:
	void					Open_Tick(const _double& TimeDelta);
	void					Stay_Tick(const _double& TimeDelta);

private:
	STATE					m_eCurState = { STATE_END };
	STATE					m_ePreState = { STATE_END };

	const _double			m_showTime = { 0.5 };
	_double					m_showTimeAcc = { 0.0 };
	_uint					m_curShowStoneInfoUI = { 0 };
	const _uint				m_maxShowStoneInfoUI = { 3 };

	const _double			m_stayTime = { 1.5 };
	_double					m_stayTimeAcc = { 0.0 };

private:
	vector<CStoneInfoUI*>	m_StoneInfoUIs;

public:
	static CStageStoneResult* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg = nullptr) override;
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath = nullptr) override;
	virtual void Free() override;
};

END