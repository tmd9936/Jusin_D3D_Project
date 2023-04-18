#pragma once
#include "UI.h"

BEGIN(Client)

class CStage_Manager;

class CStageMessageInfo :
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
	explicit CStageMessageInfo(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CStageMessageInfo(const CStageMessageInfo& rhs);
	virtual ~CStageMessageInfo() = default;

public:
	virtual HRESULT Initialize_Prototype() override; 
	virtual HRESULT Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg) override; 
	virtual HRESULT Initialize(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath);

public:
	virtual _uint Tick(_double TimeDelta) override;
	virtual _uint LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

public:
	void	Open_Message(const wstring& message) {
		if (m_eCurState != STATE_NO_RENDER)
			return;

		m_message = message;

		m_eCurState = STATE_OPENING;
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

public:
	static CStageMessageInfo* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	/* 사본 객체를 생성한다. */
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg = nullptr) override;
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath = nullptr) override;

	virtual void Free() override;
};

END