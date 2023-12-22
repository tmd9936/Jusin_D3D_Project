#pragma once

#include "Base.h"

/* ���� Ŭ���̾�Ʈ���� ȭ�鿡 ��������� ������ �����ϰ� �����Ѵ�.(Tick�� ȣ���Ѵ�) */

BEGIN(Engine)

class CLevel;

class CLevel_Manager final : public CBase
{
	DECLARE_SINGLETON(CLevel_Manager)
private:
	CLevel_Manager();
	virtual ~CLevel_Manager() = default;

public:
	HRESULT Reserve_Manager(_uint iNumLevels);

public:
	/* SetUp_Level */
	HRESULT Open_Level(_uint iLevelIndex, class CLevel* pNewLevel);
	void Tick_Level(_double TimeDelta);

public:
	const _uint			Get_LevelIndex() const {
		return m_iLevelIndex;
	}

	const _bool		Get_LevelFirstInit(_uint m_iLevelIndex) const {

		if (m_iLevelIndex >= m_LevelFirstInit.size())
			return true;

		return m_LevelFirstInit[m_iLevelIndex];
	}

private:
	CLevel*				m_pCurrentLevel = { nullptr };
	_uint				m_iLevelIndex = { 0 };

	vector<_bool>		m_LevelFirstInit;

public:
	virtual void Free() override;
};

END