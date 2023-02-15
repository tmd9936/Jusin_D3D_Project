#pragma once

#include "Base.h"

/* ��Ƽ� ���� : ���� ������ ���� ����(CLayer)�� ���� ����[] ���� */
/* 1. ���� ��ü���� ��Ƽ� �����ϴ�. */
/* 1. �纻 ��ü���� ��Ƽ� �����ϴ�. */
/* 2. �纻��ü���� Tick, LateTick�������� ȣ���Ѵ�. */

BEGIN(Engine)

class CObject_Manager final : public CBase
{
	DECLARE_SINGLETON(CObject_Manager)
private:
	CObject_Manager();
	virtual ~CObject_Manager() = default;

public:
	HRESULT Reserve_Manager(_uint iNumLevels);
	void Clear(_uint iLevelIndex);

public:
	HRESULT Add_Prototype(const _tchar* pPrototypeTag, class CGameObject* pPrototype);
	HRESULT Add_GameObject(const _tchar* pPrototypeTag, _uint iLevelIndex, const _tchar* pLayerTag, void* pArg = nullptr); /* ������ �����Ͽ� �纻�� �߰��Ѵ�. */
	void Tick(_double TimeDelta);
	void LateTick(_double TimeDelta);

private: /* ���� ��ü���� �����Ѵ�. */
	unordered_map<const _tchar*, class CGameObject*>			m_Prototypes;
	typedef unordered_map<const _tchar*, class CGameObject*>	PROTOTYPES;

private: /* ������ ��ü���� �׷����� �������� �����Ѵ�. */
	unordered_map<const _tchar*, class CLayer*>* m_pLayers = { nullptr };
	typedef unordered_map<const _tchar*, class CLayer*>	LAYERS;

private:
	_uint		m_iNumLevels = { 0 };

private:
	class CGameObject* Find_Prototype(const _tchar* pPrototypeTag);
	class CLayer* Find_Layer(_uint iLevelIndex, const _tchar* pLayerTag);

public:
	virtual void Free() override;
};

END