#include "..\Public\Renderer.h"
#include "GameObject.h"

CRenderer::CRenderer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CComponent(pDevice, pContext)
{

}

HRESULT CRenderer::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CRenderer::Initialize(void* pArg)
{
	return S_OK;
}

HRESULT CRenderer::Add_RenderGroup(RENDERGROUP eRenderGroup, CGameObject* pGameObject)
{
	if (eRenderGroup >= RENDER_END ||
		nullptr == pGameObject)
		return E_FAIL;

	m_RenderGroups[eRenderGroup].push_back(pGameObject);

	Safe_AddRef(pGameObject);

	return S_OK;
}

HRESULT CRenderer::Draw_RenderGroup()
{
	if (FAILED(Draw_Priority()))
		return E_FAIL;
	if (FAILED(Draw_NonBlend()))
		return E_FAIL;
	if (FAILED(Draw_NonLight()))
		return E_FAIL;
	if (FAILED(Draw_Blend()))
		return E_FAIL;
	if (FAILED(Draw_UI()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Draw_Priority()
{
	for (auto& pGameObject : m_RenderGroups[RENDER_PRIORITY])
	{
		if (nullptr != pGameObject)
			pGameObject->Render();

		Safe_Release(pGameObject);
	}

	m_RenderGroups[RENDER_PRIORITY].clear();

	return S_OK;
}

HRESULT CRenderer::Draw_NonBlend()
{
	for (auto& pGameObject : m_RenderGroups[RENDER_NONBLEND])
	{
		if (nullptr != pGameObject)
			pGameObject->Render();

		Safe_Release(pGameObject);
	}

	m_RenderGroups[RENDER_NONBLEND].clear();

	return S_OK;
}

HRESULT CRenderer::Draw_NonLight()
{
	for (auto& pGameObject : m_RenderGroups[RENDER_NONLIGHT])
	{
		if (nullptr != pGameObject)
			pGameObject->Render();

		Safe_Release(pGameObject);
	}

	m_RenderGroups[RENDER_NONLIGHT].clear();

	return S_OK;
}

HRESULT CRenderer::Draw_Blend()
{
	for (auto& pGameObject : m_RenderGroups[RENDER_BLEND])
	{
		if (nullptr != pGameObject)
			pGameObject->Render();

		Safe_Release(pGameObject);
	}

	m_RenderGroups[RENDER_BLEND].clear();

	return S_OK;
}

HRESULT CRenderer::Draw_UI()
{
	for (auto& pGameObject : m_RenderGroups[RENDER_UI])
	{
		if (nullptr != pGameObject)
			pGameObject->Render();

		Safe_Release(pGameObject);
	}

	m_RenderGroups[RENDER_UI].clear();

	return S_OK;
}

CRenderer* CRenderer::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CRenderer* pInstance = new CRenderer(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CRenderer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CRenderer::Clone(void* pArg)
{
	AddRef();

	return this;
}

void CRenderer::Free()
{
	__super::Free();

	for (_uint i = 0; i < RENDER_END; ++i)
	{
		for (auto& pGameObject : m_RenderGroups[i])
			Safe_Release(pGameObject);

		m_RenderGroups[i].clear();
	}

}
