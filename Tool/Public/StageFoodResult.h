#pragma once
#include "UI.h"

BEGIN(Client)

class CStage_Manager;
class CFoodInfoUI;

class CStageFoodResult :
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
	explicit CStageFoodResult(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CStageFoodResult(const CStageFoodResult& rhs);
	virtual ~CStageFoodResult() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg) override;
	virtual HRESULT Initialize(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath);

public:
	virtual _uint Tick(_double TimeDelta) override;
	virtual _uint LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

public:
	void					OpenUI();

public:
	HRESULT					Init_FoodInfoUIs();

public:
	STATE					Get_CurrentState() const {
		return m_eCurState;
	}

private:
	void					Change_State();
	void					State_Tick(const _double& TimeDelta);

private:
	void					Open_Tick(const _double& TimeDelta);
	void					Stay_Tick(const _double& TimeDelta);

private:
	STATE					m_eCurState = { STATE_OPEN };
	STATE					m_ePreState = { STATE_END };

	const _double			m_showTime = { 0.5 };
	_double					m_showTimeAcc = { 0.0 };
	_uint					m_curShowFoodInfoUI = { 0 };
	const _uint				m_maxShowFoodInfoUI = { 4 };

	const _double			m_stayTime = { 1.5 };
	_double					m_stayTimeAcc = { 0.0 };

private:
	vector<CFoodInfoUI*>	m_FoodInfoUIs;

public:
	static CStageFoodResult* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg = nullptr) override;
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath = nullptr) override;
	virtual void Free() override;
};

END