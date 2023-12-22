#pragma once

#include "Base.h"
#include "Layer.h"

/* 모아서 관리 : 내가 나누고 싶은 기준(CLayer)에 따라 레벨[] 별로 */
/* 1. 원형 객체들을 모아서 관리하다. */
/* 1. 사본 객체들을 모아서 관리하다. */
/* 2. 사본객체들의 Tick, LateTick무한으로 호출한다. */

BEGIN(Engine)

class CGameObject;
class CComponent;

class ENGINE_DLL CObject_Manager final : public CBase
{
	DECLARE_SINGLETON(CObject_Manager)
private:
	CObject_Manager();
	virtual ~CObject_Manager() = default;

public:
	HRESULT Reserve_Manager(_uint iNumLevels);
	void Clear(_uint iLevelIndex);

public:
	HRESULT		Add_Layer(_uint iLevelIndex, const _tchar* pLayerTag);

public:
	HRESULT Add_Prototype(const _tchar* pPrototypeTag, class CGameObject* pPrototype);
	void Tick(_double TimeDelta);
	void LateTick(_double TimeDelta);

public:
	HRESULT Add_GameObject(const _tchar* pPrototypeTag, _uint iLevelIndex,
		const _tchar* pLayerTag, const _tchar* pObjectNameTag = nullptr, void* pArg = nullptr, CLONE_TYPE eCloneType = CLONE_ARGS); /* 원형을 복제하여 사본을 추가한다. */
	
	HRESULT Add_GameObject(const _tchar* pPrototypeTag, _uint iLevelIndex,
		const _tchar* pLayerTag, CGameObject** ppOut, const _tchar* pObjectNameTag = nullptr, void* pArg = nullptr, CLONE_TYPE eCloneType = CLONE_ARGS); /* 원형을 복제하여 사본을 추가한다. */
	
	CGameObject* Get_Object(_uint iLevelIndex, const _tchar* pLayerTag, const _tchar* pObjectTag) const;
	CGameObject* Get_Object(_uint iLevelIndex, const _tchar* pLayerTag, wstring pObjectTag) const;

	template<typename T, typename = std::enable_if<is_base_of<CComponent, T>::value>>
	HRESULT Get_ComponentList(vector<T>& result, _uint iLevelIndex, const _tchar* pLayerTag)
	{
		if (nullptr == pLayerTag)
			return E_FAIL;

		CLayer* pLayer = Find_Layer(iLevelIndex, pLayerTag);

		if (nullptr == pLayer)
			return E_FAIL;

		const unordered_multimap<FamilyId, CGameObject*>* componentStore = pLayer->Get_ComponentStore();

		if (nullptr == componentStore)
			return E_FAIL;

		auto iterPair = componentStore->equal_range(T::familyId);

		for (auto iter = iterPair.first; iter != iterPair.second; ++iter)
		{
			T* com = (T*)(iter->second->Get_Component(T::familyId));
			if (nullptr != com)
				result.push_back(com);
		}

		return S_OK;
	}

	template<typename T, typename = std::enable_if<is_base_of<CComponent, T>::value>>
	HRESULT Get_ObjectList(vector<CGameObject*>& result, _uint iLevelIndex, const _tchar* pLayerTag)
	{
		if (nullptr == pLayerTag)
			return E_FAIL;

		CLayer* pLayer = Find_Layer(iLevelIndex, pLayerTag);

		if (nullptr == pLayer)
			return E_FAIL;

		const unordered_multimap<FamilyId, CGameObject*>* componentStore = pLayer->Get_ComponentStore();

		if (nullptr == componentStore)
			return E_FAIL;

		auto iterPair = componentStore->equal_range(T::familyId);

		for (auto iter = iterPair.first; iter != iterPair.second; ++iter)
		{
			if (nullptr != iter->second)
				result.push_back(iter->second);
		}

		return S_OK;
	}


public:
	HRESULT		Add_Component (const FamilyId& familyId, CGameObject* pGameObject, _uint iLevelIndex, const _tchar* pPrototypeTag, CComponent** ppOut, void* pArg);
	CComponent* Get_Component(const FamilyId& familyId, CGameObject* pObj) const;
	CComponent* Get_Component(const FamilyId& familyId, _uint iLevelIndex, const _tchar* pLayerTag, const _tchar* pObjectTag) const;
	HRESULT		Remove_Component(const FamilyId& familyId, CGameObject* pObj);
	HRESULT		Remove_All_GameObject_In_Layer(_uint iLevelIndex, const _tchar* pLayerTag);
	HRESULT		Get_All_GameObject_In_Layer(vector<CGameObject*>& result, _uint iLevelIndex, const _tchar* pLayerTag);

public:
	HRESULT		Get_Layer_Names(_uint iLevelIndex, vector<wstring>& vecNames);
	const unordered_map<const _tchar*, class CGameObject*>*		Get_GameObject_Prototypes();
	CGameObject* Clone_GameObject(const _tchar* pLayerTag, _uint iLevelIndex, const _tchar* pPrototypeTag, void* pArg);
	CGameObject* Clone_GameObject(const _tchar* pLayerTag, _uint iLevelIndex, const _tchar* pPrototypeTag, CGameObject** ppOut, void* pArg);
	HRESULT		Layer_Tick_State_Change(const _tchar* pLayerTag, _uint iLevelIndex, _bool bTick);

public:
	_bool		Is_Layer(_uint iLevelIndex, const wstring& layerTag);

	HRESULT		Change_Component(const FamilyId& familyId, CGameObject* pGameObject, _uint iLevelIndex, const _tchar* pPrototypeTag, CComponent** ppOut, void* pArg);

protected:
	HRESULT		Store_Component(const _tchar* pLayerTag, class CGameObject* pGameObject, const FamilyId& id);

private: /* 원형 객체들을 보관한다. */
	unordered_map<const _tchar*, class CGameObject*>			m_Prototypes;
	typedef unordered_map<const _tchar*, class CGameObject*>	PROTOTYPES;

private: /* 복제한 객체들을 그룹지어 레벨별로 보관한다. */
	unordered_map<wstring, class CLayer*>* m_pLayers = { nullptr };
	typedef unordered_map<wstring, class CLayer*>	LAYERS;

private:
	_uint		m_iNumLevels = { 0 };

private:
	CGameObject* Find_Prototype(const _tchar* pPrototypeTag) const;
	CLayer* Find_Layer(_uint iLevelIndex, const _tchar* pLayerTag) const;

public:
	virtual void Free() override;
};

END