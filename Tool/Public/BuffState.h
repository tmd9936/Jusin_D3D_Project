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
���� Ÿ�Կ� �°� �÷��̾��� ������Ʈ�� �����ͼ� �����Ű��
�߰��� ü���� ������Ʈ�� ������ �ؽ�ó �ٲٱ�
����/����� �������� ���� ���� NONE���� �ٲٱ� NONE�� ���¿����� �����׷쿡 �߰� X*/


/*
* �θ� ������Ʈ�� ����Ǵ� ����
Transform ������Ʈ : �̵��ӵ� ���� �� ����
HP ������Ʈ : ���� �� ü�� ƽ ������
Attack ������Ʈ : ���ݷ� ���� �� ����
MonFSM ������Ʈ : ���� ���� ��Ű��
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
		CTransform*			pParentTransform = { nullptr }; // ������ �Ǵ� �θ�
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
	virtual HRESULT Initialize_Prototype() override; /* ������ü�� �ʱ�ȭ�۾� */
	virtual HRESULT Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg) override; /* �纻��ü�� �ʱ�ȭ�۾� */
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