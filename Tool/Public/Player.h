#pragma once

#include "Client_Defines.h"
#include "Monster.h"

BEGIN(Engine)
class CTransform;
class CRenderer;
class CTexture;
class CShader;
class CModel;
END

BEGIN(Client)

class CPlayer final : public CMonster
{

private:
	CPlayer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPlayer(const CPlayer& rhs);
	virtual ~CPlayer() = default;

public:
	virtual HRESULT Initialize_Prototype() override; /* ������ü�� �ʱ�ȭ�۾� */
	virtual HRESULT Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg) override; /* �纻��ü�� �ʱ�ȭ�۾� */
	virtual _uint Tick(_double TimeDelta) override;
	virtual _uint LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

public:
	virtual void On_Collision(CCollider* pOther, const _float& fX, const _float& fY, const _float& fZ);
	virtual void On_CollisionEnter(CCollider* pOther, const _float& fX, const _float& fY, const _float& fZ);
	virtual void On_CollisionExit(CCollider* pOther, const _float& fX, const _float& fY, const _float& fZ);

public:
	void	Set_TestSkillindex(_uint testSkillindex) {
		m_TestSkillindex = testSkillindex;
	}

public:
	virtual HRESULT Add_TransitionRandomState() override;
	virtual _uint State_Tick(const _double& TimeDelta) override;

private:
	void	Do_Skill();

private:
	_uint			m_TestSkillindex = { 0 };

//private:
//	HRESULT SetUp_ShaderResources(); /* ���̴� ���������� ���� ������. */

public:
	static CPlayer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg = nullptr) override;
	virtual void Free() override;
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath) override;
};

END