#pragma once

#include "Base.h"

BEGIN(Engine)

class CTarget_Manager final : public CBase
{
	DECLARE_SINGLETON(CTarget_Manager)

private:
	CTarget_Manager();
	virtual ~CTarget_Manager() = default;

public:
	HRESULT Add_RenderTarget(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, 
		const _tchar* pTargetTag, _uint iSizeX, _uint iSizeY, DXGI_FORMAT eFormat, const _float4& vClearColor);
	HRESULT Add_MRT(const _tchar* pMRTTag, const _tchar* pTargetTag);
	/// <summary>
	/// 바인딩 되어있던 백버퍼뷰를 임시 저장한 후에 해당 태그의 MRT를 랜더 타겟으로 지정
	/// </summary>
	/// <param name="pContext">디바이스 컨텍스트  객체 </param>
	/// <param name="pMRTTag">찾을 MRT의 태그</param>
	/// <returns></returns>
	HRESULT Begin_MRT(ID3D11DeviceContext* pContext, const _tchar* pMRTTag);

	/// <summary>
	/// 바인딩 되어있던 MRT 대신 기존에 바인딩 되어있던 백버퍼 뷰를 다시 바인딩 함
	/// </summary>
	/// <param name="pContext"></param>
	/// <returns></returns>
	HRESULT End_MRT(ID3D11DeviceContext* pContext);

	/// <summary>
	/// 해당 타겟에게 셰이더 리소스 뷰를 셰이더에 바인딩 요청
	/// </summary>
	/// <param name="pTargetTag"> 해당 타겟의 태그 </param>
	/// <param name="pShader">바인딩 될 셰이더</param>
	/// <param name="pConstantName">셰이더 안에 있는 변수의 컨택스트 이름</param>
	/// <returns></returns>
	HRESULT Set_ShaderResourceView(const _tchar* pTargetTag, class CShader* pShader,
		const char* pConstantName);

#ifdef RENDERTARGET_DEBUG
public:
	HRESULT Ready_Debug(const _tchar* pTargetTag, _float fX, _float fY, _float fSizeX, _float fSizeY);
	HRESULT Render_MRT(const _tchar* pMRTTag, class CShader* pShader, class CVIBuffer_Rect* pVIBuffer);
#endif // _DEBUG

private:
	/* 랜더타켓을 바인딩 하기 이전에 바인딩 되어있던 백버퍼를 임시로 저장하기 위함*/
	ID3D11RenderTargetView* m_pBackBufferView = { nullptr };
	/* 뎁스 스탠실은 기존에 있던것을 사용해야함*/
	ID3D11DepthStencilView* m_pDepthStencilView = { nullptr };

	UINT					m_CurBeginRederTargetNum = { 0 };

private: /* 렌더타겟을 생성하여 보관의 역할을 한다. */
	unordered_map<const _tchar*, class CRenderTarget*>				m_RenderTargets;
	typedef unordered_map<const _tchar*, class CRenderTarget*>		RENDERTARGETS;

private:
	/* 생성된 렌더타겟들 중, 동시에 바인딩 되어야할 렌더타겟들을 미리 묶어놓는다. 
	보관과 바인딩을 따로 하는 이유는 같은 랜더타겟이 다른 랜더타겟 리스트에 복수로 들어갈 수 있음*/
	unordered_map<const _tchar*, list<class CRenderTarget*>>			m_MRTs;
	typedef unordered_map<const _tchar*, list<class CRenderTarget*>>	MRTS;

private:
	class CRenderTarget* Find_RenderTarget(const _tchar* pTargetTag);
	list<class CRenderTarget*>* Find_MRT(const _tchar* pMRTTag);

public:
	virtual void Free() override;
};

END