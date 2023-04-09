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
public:
	typedef struct skillLoopDesc
	{
		_uint m_CurskillIndex; 
		CMonFSM::MONSTER_STATE m_eLoopState;
	} SKILL_LOOP_DESC;

private:
	CPlayer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPlayer(const CPlayer& rhs);
	virtual ~CPlayer() = default;

public:
	virtual HRESULT Initialize_Prototype() override; 
	virtual HRESULT Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg) override;
	virtual HRESULT Initialize(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath) override;

	virtual _uint Tick(_double TimeDelta) override;
	//virtual _uint LateTick(_double TimeDelta) override;
	//virtual HRESULT Render() override;

public:
	void On_CollisionEnter(CCollider* pOther, const _float& fX, const _float& fY, const _float& fZ);
	void On_Collision(CCollider* pOther, const _float& fX, const _float& fY, const _float& fZ);
	void On_CollisionExit(CCollider* pOther, const _float& fX, const _float& fY, const _float& fZ);

public:
	void	Set_TestSkillindex(_uint testSkillindex) {
		m_TestSkillindex = testSkillindex;
	}

private:
	virtual HRESULT Add_TransitionRandomState() override;
	virtual _uint State_Tick(const _double& TimeDelta) override;

protected:
	virtual _bool			Save_By_JsonFile_Impl(Document& doc, Document::AllocatorType& allocator);
	virtual _bool			Load_By_JsonFile_Impl(Document& doc);

private:
	void	Do_TestSkill();
	void	Jump_Rotate();

private:
	_uint				m_TestSkillindex = { 58 };
	_double				m_fAccel = { 1.5f };
	_int				m_SkillLoopCount = { 0 };
	_float				m_SkillLoopDelay = { 1.f };

	SKILL_LOOP_DESC		m_SkillLoopDesc = {};

	_uint				m_normalSkillType2 = { 0 };

public:
	static CPlayer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg = nullptr) override;
	virtual void Free() override;
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath) override;
};

END