#pragma once

#include "Base.h"

/* ���� Ŭ���̾�Ʈ���� ȭ�鿡 ��������� ������ �����ϰ� �����Ѵ�.(Tick�� ȣ���Ѵ�) */

BEGIN(Engine)

class CLevel_Manager final : public CBase
{
	DECLARE_SINGLETON(CLevel_Manager)
private:
	CLevel_Manager();
	virtual ~CLevel_Manager() = default;

public:
	/* SetUp_Level */
	HRESULT Open_Level(_uint iLevelIndex, class CLevel* pNewLevel);
	void Tick_Level(_double TimeDelta);

public:
	const _uint			Get_LevelIndex() const {
		return m_iLevelIndex;
	}

private:
	class CLevel* m_pCurrentLevel = { nullptr };
	_uint						m_iLevelIndex = { 0 };

public:
	virtual void Free() override;
};

END