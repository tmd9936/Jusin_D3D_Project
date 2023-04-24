#include"stdafx.h"

#include "PokemonPowerInfoUI.h"

CPokemonPowerInfoUI::CPokemonPowerInfoUI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext)
{
}

CPokemonPowerInfoUI::CPokemonPowerInfoUI(const CPokemonPowerInfoUI& rhs)
	: CUI(rhs)
{
}

HRESULT CPokemonPowerInfoUI::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CPokemonPowerInfoUI::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, pArg)))
		return E_FAIL;

	if (FAILED(Init_PokemonPower()))
		return E_FAIL;

	return S_OK;
}

HRESULT CPokemonPowerInfoUI::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath)
{
	if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, filePath)))
		return E_FAIL;

	if (FAILED(Init_PokemonPower()))
		return E_FAIL;

	return S_OK;
}


HRESULT CPokemonPowerInfoUI::Init_PokemonPower()
{
	if (m_TextParts.empty())
		return E_FAIL;
	
	_uint power = 0;
	if (FAILED(Get_PokemonPower(power, 1)))
		return E_FAIL;

	if (FAILED(Get_PokemonPower(power, 2)))
		return E_FAIL;

	if (FAILED(Get_PokemonPower(power, 3)))
		return E_FAIL;

	m_TextParts.at(0)->Set_Text(to_wstring(power));

	return S_OK;
}

HRESULT CPokemonPowerInfoUI::Get_PokemonPower(_uint& outPower, const _uint& pokemonIndex)
{
	string path = "../../Reference/Resources/Data/Database/NowMonster/NowPartyMonster";

	path.append(to_string(pokemonIndex));
	path.append(".json");

	FILE* fp = fopen(path.c_str(), "rb"); // non-Windows use "r"

	if (NULL == fp)
	{
		MSG_BOX("Load File Open Error");
		return E_FAIL;
	}
	else
	{
		char* readBuffer = new char[65536];
		FileReadStream is(fp, readBuffer, sizeof(readBuffer));

		Document doc;
		doc.ParseStream(is);

		if (doc.MemberCount() <= 0)
		{
			fclose(fp);
			Safe_Delete_Array(readBuffer);
			return E_FAIL;
		}

		/* 备泅何 矫累 */
		const Value& PokemonDesc = doc["PokemonDesc"].GetObj();
		outPower += PokemonDesc["m_hpBasis"].GetUint();
		outPower += PokemonDesc["m_attackBasis"].GetUint();

		/* 备泅何 场 */
		fclose(fp);
		Safe_Delete_Array(readBuffer);

		return S_OK;
	}

	return E_FAIL;
}


CPokemonPowerInfoUI* CPokemonPowerInfoUI::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPokemonPowerInfoUI* pInstance = new CPokemonPowerInfoUI(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created CPokemonPowerInfoUI");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CPokemonPowerInfoUI::Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	CPokemonPowerInfoUI* pInstance = new CPokemonPowerInfoUI(*this);

	if (FAILED(pInstance->Initialize(pLayerTag, iLevelIndex, pArg)))
	{
		MSG_BOX("Failed to Cloned CPokemonPowerInfoUI");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CPokemonPowerInfoUI::Clone(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath)
{
	CPokemonPowerInfoUI* pInstance = new CPokemonPowerInfoUI(*this);

	if (FAILED(pInstance->Initialize(pLayerTag, iLevelIndex, filePath)))
	{
		MSG_BOX("Failed to Cloned CPokemonPowerInfoUI");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPokemonPowerInfoUI::Free()
{
	__super::Free();
}
