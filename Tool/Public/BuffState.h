#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CVIBuffer_Rect;
class CRenderer;
class CShader;
class CTexture;
class CTransform;
class CModel;
class CHp;
class CAttack;
class CMonFSM;
END

/*
버프 타입에 맞게 플레이어의 컴포넌트를 가져와서 변경시키기
추가로 체인지 컴포넌트로 아이콘 텍스처 바꾸기
버프/디버프 끝났으면 버프 상태 NONE으로 바꾸기 NONE인 상태에서는 랜더그룹에 추가 X*/


/*
* 부모 컴포넌트별 적용되는 버프
Transform 컴포넌트 : 이동속도 증가 및 감소
HP 컴포넌트 : 방어력 및 체력 틱 데미지
Attack 컴포넌트 : 공격력 증가 및 감소
MonFSM 컴포넌트 : 몬스터 정지 시키기
*/

BEGIN(Client)

class CBuffState final : public CGameObject
{
public:
	enum BUFF_STATE {
		BUFF_STATE_NONE,
		BUFF_STATE_DAMAGE_UP,
		BUFF_STATE_DAMAGE_DOWN,
		BUFF_STATE_DEFENSE_UP,
		BUFF_STATE_DEFENSE_DOWN,
		BUFF_STATE_SPEED_UP,
		BUFF_STATE_SPEED_DOWN,
		BUFF_STATE_RESIST_UP,
		BUFF_STATE_RESIST_DOWN,
		BUFF_STATE_DOKU,
		BUFF_STATE_MAHI,
		BUFF_STATE_NEMURI,
		BUFF_STATE_KOORI,
		BUFF_STATE_YAKEDO,
		BUFF_STATE_KONRAN,
		BUFF_STATE_KANASIBARI,
		BUFF_STATE_NEMURI2,
		BUFF_STATE_END
	};

public:
	typedef struct BuffState_Desc
	{
		CTransform*			pParentTransform = { nullptr }; // 기준이 되는 부모
		CHP*				pParentHP = { nullptr };
		CAttack*			pParentAttack = { nullptr };
		CMonFSM*			pParentMonFSM = { nullptr };
		CModel*				pParentModel = { nullptr };

		_float4x4			PivotMatrix;

		_float				m_fPositionX;
		_float				m_fPositinoY;
		_float				m_fPositinoZ;

		_float				m_fSizeX;
		_float				m_fSizeY;

		_tchar				m_TextureProtoTypeName[MAX_PATH];
		_uint				m_TextureLevelIndex;

	} BUFFSTATE_DESC;
private:
	CBuffState(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBuffState(const CBuffState& rhs);
	virtual ~CBuffState() = default;

public:
	virtual HRESULT Initialize_Prototype() override; /* 원형객체의 초기화작업 */
	virtual HRESULT Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg) override; /* 사본객체의 초기화작업 */
	virtual _uint Tick(_double TimeDelta) override;
	virtual _uint LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

public:
	HRESULT Set_BuffState(_uint buffType, _uint skillType, BUFF_STATE eState, const _tchar* textureName,
		_float valueA, _float valueB, _float endTime, _float ratio);

	HRESULT Change_Texture(const _tchar* prototypeTag);

	const _bool	Get_CanBuffSet() const {
		return m_bCanBuffSet;
	}

	const _uint Get_CurSkillType() const {
		return m_CurSkillType;
	}

	const _uint Get_CurBuffType() const {
		return m_CurBuffType;
	}

private:
	void	EndTime_Check(const _double& TimeDelta);
	void	State_Tick(const _double& TimeDelta);
	void	Change_State();

	void	Change_State_Buff_On();

private:
	void	Set_ParentSpeedPercent(_float percent);
	void	Set_ParentDefensePercent(_float percent);

	void	Set_ParentState(CMonFSM::MONSTER_STATE eState);

private:
	void	Return_Original_State(BUFF_STATE preState);

private:
	HRESULT Add_Components();
	HRESULT SetUp_ShaderResources();
	HRESULT SetUp_CoolTimeMask_ShaderResources();

	//_matrix Remove_Scale(_fmatrix Matrix);

private:
	CTransform*				m_pTransformCom = { nullptr };
	CRenderer*				m_pRendererCom = { nullptr };
	CShader*				m_pShaderCom = { nullptr };
	CVIBuffer_Rect*			m_pVIBufferCom = { nullptr };
	CTexture*				m_pTextureCom = { nullptr };
	CTexture*				m_pMaskTextureCom = { nullptr };

private:
	BUFFSTATE_DESC			m_Desc = {};
	_float4x4				m_FinalWorldMatrix = {};

	_float4x4				m_ViewMatrix = {};
	_float4x4				m_ProjMatrix = {};

	_bool					m_bCanBuffSet = { true };
	_double					m_EndTime = { 10.0 };
	_double					m_EndTimeAcc = { 0.0 };

	_float					m_valueA = { 0.f };
	_float					m_valueB = { 0.f };
	_float					m_ratio = { 0.f };

	BUFF_STATE				m_eCurBuffState = { BUFF_STATE_NONE };
	BUFF_STATE				m_ePreBuffState = { BUFF_STATE_END };

	_uint					m_CurSkillType = { 0 };
	_uint					m_CurBuffType = { 0 };


public:
	static CBuffState* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg = nullptr) override;
	virtual void Free() override;
};

END