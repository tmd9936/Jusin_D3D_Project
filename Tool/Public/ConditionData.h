#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
END

BEGIN(Client)

class CConditionData final : public CGameObject
{
public:
	typedef struct ConditionData_Desc
	{
		_uint m_id;
		_uint m_type;
		_float m_Value_A;
		_float m_Value_B;
		_float m_time;
		_float m_ratio;

	} CONDITIONDATA_DESC;

	typedef struct ConditionTypeData_Desc
	{
		_uint m_id;
		_uint m_category;
		wstring m_iconPath;
		wstring m_iconLSizePath;
		_uint m_effectType;
		_uint m_resistType_1;
		_uint m_resistType_2;
		_uint m_groupID;
		_uint m_force;

	} CONDITIONTYPEDATA_DESC;

	typedef struct ConditionParameter_Desc
	{
		_float m_attackRatioClampMin;
		_float m_attackRatioClampMax;

		_float m_damageRatioClampMin;
		_float m_damageRatioClampMax;

		_float m_moveSpeedClampMin;
		_float m_moveSpeedClampMax;

		_float m_resistBadConditionClampMin;
		_float m_resistBadConditionClampMax;

	} CONDITIONPARAMETER_DESC;


private:
	explicit CConditionData(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CConditionData(const CConditionData& rhs);
	virtual ~CConditionData() = default;

public:
	virtual HRESULT Initialize_Prototype(const char* conditionDataFilePath, const char* conditionTypeDataFilePath,
		const char* conditionParameterFilePath); /* 원형객체의 초기화작업 */
	virtual HRESULT Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg) override; /* 사본객체의 초기화작업 */
	virtual HRESULT Initialize(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath);

	virtual _uint Tick(_double TimeDelta) override;
	virtual _uint LateTick(_double TimeDelta) override;

public:
	CONDITIONDATA_DESC	Get_ConditonData(_uint index) const {
		return m_ConditionData_Desc[index];
	}

	CONDITIONTYPEDATA_DESC	Get_ConditonTypeData(_uint index) const {
		return m_ConditionTypeData_Desc[index];
	}

	CONDITIONPARAMETER_DESC	Get_ConditionParameter() const {
		return m_ConditionParameter;
	}

protected:
	virtual _bool			Save_By_JsonFile_Impl(Document& doc, Document::AllocatorType& allocator);
	virtual _bool			Load_By_JsonFile_Impl(Document& doc);

private:
	_bool	Load_ConditionTypeData_JsonFile(const char* conditionTypeDataFilePath);
	_bool	Load_ConditionParameter_JsonFile(const char* conditionParameterFilePath);

private:
	HRESULT Add_Components();
	HRESULT Add_Components_By_File();

private:
	vector<CONDITIONDATA_DESC>			m_ConditionData_Desc;
	vector<CONDITIONTYPEDATA_DESC>		m_ConditionTypeData_Desc;
	CONDITIONPARAMETER_DESC				m_ConditionParameter = {};

public:
	static CConditionData* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, 
		const char* conditionDataFilePath, const char* conditionTypeDataFilePath, 
		const char* conditionParameterFilePath);
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg = nullptr) override;
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath = nullptr);
	virtual void Free() override;
};

END