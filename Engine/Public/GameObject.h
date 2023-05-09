#pragma once

#include "Base.h"
#include "Component_Manager.h"
#include "Object_Manager.h"
#include "Engine_Defines.h"

BEGIN(Engine)

class CComponent;

class ENGINE_DLL CGameObject abstract : public CBase
{
protected:
	explicit CGameObject(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	explicit CGameObject(const CGameObject& rhs);
	virtual ~CGameObject() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg);
	virtual HRESULT Initialize(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath);
	virtual _uint Tick(_double TimeDelta);
	virtual _uint LateTick(_double TimeDelta);
	virtual HRESULT Render();
	virtual	HRESULT Render_ShadowDepth() { return S_OK; }
	virtual	HRESULT Render_Laplacian() { return S_OK; }

public:
	virtual void On_Collision(CCollider* pOther, const _float& fX, const _float& fY, const _float& fZ) {}
	virtual void On_CollisionEnter(CCollider* pOther, const _float& fX, const _float& fY, const _float& fZ) {}
	virtual void On_CollisionExit(CCollider* pOther, const _float& fX, const _float& fY, const _float& fZ) {}

public:
	const wstring			Get_NameTag() const {
		return m_strNameTag; }

	void					Set_NameTag(wstring strName) {
		m_strNameTag = strName; 
	}
	void					Set_NameTag(const _tchar* strName) {
		m_strNameTag = strName; 
	}

	const RENDERGROUP		Get_RenderId() const {
		return m_eRenderId; 
	}
	void					Set_RenderId(RENDERGROUP eRenderId) {
		m_eRenderId = eRenderId; 
	}

	wstring			Get_LayerTag() const {
		return m_strLayerTag; 
	}

	void					Set_LayerTag(const wstring strLayerTag) {
		m_strLayerTag = strLayerTag; 
	}

	void					Set_LayerTag(const _tchar* pLayerTag) {
		m_strLayerTag = pLayerTag;
	}

	const _uint				Get_Levelindex() const {
		return m_iLevelindex; 
	}
	void					Set_Levelindex(const _uint& iLevelindex) {
		m_iLevelindex = iLevelindex; 
	}

	const wstring			Get_ProtoTypeTag() const {
		return m_strProtoTypeTag;
	}
	void					Set_ProtoTypeTag(const wstring prototypeTag) {
		m_strProtoTypeTag = prototypeTag;
	}

	void					Get_Components(map<FamilyId, const CComponent*>& components) const {
		for (auto iter = m_Components.begin(); iter != m_Components.end(); ++iter)
		{
			components.insert({ iter->first, iter->second });
		}
	}

	const string			Get_JsonPath() const {
		return m_strSaveJsonPath;
	}

public:
	const _bool				Is_Dead() const { 
		return m_bDead; 
	}

	const _bool				Is_BeCulling() const {
		return m_bBeCulling;
	}

	void					Set_Alive() { m_bDead = false; }
	void					Set_Dead() { m_bDead = true; }

public:
	void					Get_Components_FamilyId(vector<FamilyId>& vecFamilyIds);
	HRESULT					Add_Component(const FamilyId& familyId, CComponent* pComponent);
	CComponent*				Get_Component(const FamilyId& familyId) const;
	HRESULT					Remove_Component(const FamilyId& familyId);

public:
	virtual void			Change_State_FSM(_uint eState);

public:
	_bool					Save_Args(const _tchar* filePath);
protected:
	virtual _bool			Save_Args_Impl(HANDLE hFile);
private:
	HANDLE					Save_Args_Begin(const _tchar* filePath);
	_bool					Save_Args_End(HANDLE hFile);

public:
	_bool					Save_By_JsonFile(const char* filePath);
protected:
	virtual _bool			Save_By_JsonFile_Impl(Document& doc, Document::AllocatorType& allocator);
public:
	_bool					Load_By_JsonFile(const char* filePath);
protected:
	virtual _bool			Load_By_JsonFile_Impl(Document& doc);

public:
	template<typename T, typename = std::enable_if<is_base_of<CComponent, T>::value>>
	T* Get_As() {
		auto iter = m_Components.find(T::familyId);
		if (m_Components.end() == iter)
			return nullptr;

		return (T*)iter->second;
	}
	

private:
	HRESULT					Find_Component(const FamilyId& familyId) const;

protected:
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };

protected:
	/* 해시테이블 */
	friend CLayer;
	unordered_map<FamilyId, class CComponent*>		m_Components = {};
	wstring											m_strNameTag = {};
	wstring											m_strLayerTag = {};
	wstring											m_strProtoTypeTag = {};
	RENDERGROUP										m_eRenderId = { RENDER_END };

	_bool											m_bDead = { false };
	_uint											m_iLevelindex = { 0 };

	string											m_strSaveJsonPath = {};

	_bool											m_bBeCulling = { false };



public:
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath = nullptr);
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg = nullptr) = 0;
	virtual void Free() override;
};


END