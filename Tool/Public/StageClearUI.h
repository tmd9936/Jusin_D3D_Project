#pragma once
#include "UI.h"

BEGIN(Client)

class CStage_Manager;

class CStageClearUI :
	public CUI
{
public:
	enum STATE
	{
		STATE_OPENING,
		STATE_STAY,
		STATE_CLOSING,
		STATE_NO_RENDER,
		STATE_END
	};

private:
	explicit CStageClearUI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CStageClearUI(const CStageClearUI& rhs);
	virtual ~CStageClearUI() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg) override;
	virtual HRESULT Initialize(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath);

public:
	virtual _uint Tick(_double TimeDelta) override;
	virtual _uint LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

public:
	_bool	Open_Message(const wstring& message) {
		if (m_eCurState != STATE_NO_RENDER)
			return false;

		m_message = message;

		m_eCurState = STATE_OPENING;

		return true;
	}

	_bool	Open_Message(const wstring& message, const _float& textPositionX) {
		if (m_eCurState != STATE_NO_RENDER)
			return false;

		m_message = message;

		m_eCurState = STATE_OPENING;

		m_textPositionX = textPositionX;

		return true;
	}

	const _bool	Is_NoMessageState() const {
		if (STATE_NO_RENDER == m_eCurState)
			return true;
		else
			return false;
	}

private:
	void			Change_State();
	void			State_Tick(const _double& TimeDelta);
	void			Stay_Time_Check(const _double& TimeDelta);

private:
	void			State_Tick_Opening(const _double& TimeDelta);
	void			State_Tick_Closing(const _double& TimeDelta);

private:
	void			Set_TextOnParts(const wstring& message);

private:
	HRESULT			Insert_In_Stage_Manager();

private:
	STATE			m_eCurState = { STATE_OPENING };
	STATE			m_ePreState = { STATE_END };

	_double			m_StayTime = { 1.0 };
	_double			m_StayTimeAcc = { 0.0 };

	_float			m_CurSizeY = { 0.f };
	_float			m_MaxSizeY = { 0.f };
	wstring			m_message = {};

	_float			m_textPositionX = { 0.f };

public:
	static CStageClearUI* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	/* 사본 객체를 생성한다. */
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg = nullptr) override;
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath = nullptr) override;

	virtual void Free() override;
};

END