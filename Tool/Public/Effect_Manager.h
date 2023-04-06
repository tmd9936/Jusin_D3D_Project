#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

#include "Effect.h"

#include "ChargeEffect.h"
#include "HommingAttackEffect.h"
#include "RushAttackEffect.h"

/*
이펙트 -> 글로우 셰이딩 해야함
*/

BEGIN(Engine)
END

BEGIN(Client)

class CEffect_Manager final : public CGameObject
{

private:
	CEffect_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEffect_Manager(const CEffect_Manager& rhs);
	virtual ~CEffect_Manager() = default;

public:
	HRESULT Initialize_Prototype(const char* filePath); /* 원형객체의 초기화작업 */
	virtual HRESULT Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg) override; /* 사본객체의 초기화작업 */
	virtual HRESULT Initialize(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath);

	virtual _uint Tick(_double TimeDelta) override;
	virtual _uint LateTick(_double TimeDelta) override;

	CEffect* Create_Effect(_uint effectType, const _tchar* pLayerTag, _uint iLevelIndex, _bool hasCollider = false, _float3 vPos = { 0.f, 0.f, 0.f });

	HRESULT	Create_Charge_Effect(_uint effectType, const _tchar* pLayerTag, _uint iLevelIndex, CChargeEffect::CHARGE_EFFECT_DESC& chargeEffectDesc);

	//HRESULT	Create_Attack_Effect(_uint effectType, const _tchar* pLayerTag, _uint iLevelIndex, ATTACK_TYPE attackType);

public:
	void	Get_Effect_Desces(vector<CEffect::EFFECT_DESC>& Effect_Descs);

public:
	HRESULT	Reload_Datas();

protected:
	virtual _bool			Save_By_JsonFile_Impl(Document& doc, Document::AllocatorType& allocator);
	virtual _bool			Load_By_JsonFile_Impl(Document& doc);

private:
	HRESULT Add_Components();
	HRESULT Add_Components_By_File();

	HRESULT SetUp_ShaderResources(); /* 셰이더 전역변수에 값을 던진다. */


private:
	vector<CEffect::EFFECT_DESC> m_Effect_Descs;

	const wstring m_EffectFilePath = { L"../../Reference/Resources/Mesh/Animation/Effect/" };

	string m_filePath = {};

public:
	static CEffect_Manager* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const char* filePath);
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg = nullptr) override;
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath = nullptr);
	virtual void Free() override;
};

END