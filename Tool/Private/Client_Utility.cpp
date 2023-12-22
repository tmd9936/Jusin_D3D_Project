#include "stdafx.h"
#include "Client_Utility.h"

#include "GameInstance.h"

#include "StageEnv.h"

#include <codecvt>

HRESULT CClient_Utility::Load_Layer_GameObjects(const char* filePath)
{
	if (nullptr == filePath)
		return E_FAIL;

	FILE* fp = fopen(filePath, "rb"); // non-Windows use "r"

	if (NULL == fp)
	{
		MSG_BOX("Load File Open Error");
		return E_FAIL;
	}
	else
	{
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> convert;

		char* readBuffer = new char[65536];
		FileReadStream is(fp, readBuffer, sizeof(readBuffer));

		Document document;
		document.ParseStream(is);

		_uint	iLevelindex = 0;
		_float3 vScale{}, vRot{}, vPos{};
		wstring objectNameTag, layerTag, prototypeTag;

		CTransform* pTransform = nullptr;

		assert(document.HasMember("Level"));
		assert(document["Level"].IsUint());
		iLevelindex = (_uint)document["Level"].GetUint();

		assert(document.HasMember("Layer"));
		assert(document["Layer"].IsString());
		layerTag = convert.from_bytes(document["Layer"].GetString());

		const Value& objects = document["Objects"];
		assert(objects.IsArray());
		for (SizeType i = 0; i < objects.Size(); ++i)
		{
			CGameObject* pOut = nullptr;

			vScale.x = objects[i]["ScaleX"].GetFloat();
			vScale.y = objects[i]["ScaleY"].GetFloat();
			vScale.z = objects[i]["ScaleZ"].GetFloat();

			vRot.x = objects[i]["RotX"].GetFloat();
			vRot.y = objects[i]["RotY"].GetFloat();
			vRot.z = objects[i]["RotZ"].GetFloat();

			vPos.x = objects[i]["PosX"].GetFloat();
			vPos.y = objects[i]["PosY"].GetFloat();
			vPos.z = objects[i]["PosZ"].GetFloat();

			prototypeTag = convert.from_bytes(objects[i]["PrototypeTag"].GetString());
			objectNameTag = convert.from_bytes(objects[i]["ObjectNameTag"].GetString());


			if (objectNameTag.empty())
				CGameInstance::GetInstance()->Add_GameObject(prototypeTag.c_str(), iLevelindex, layerTag.c_str(), &pOut);
			else
				CGameInstance::GetInstance()->Add_GameObject(prototypeTag.c_str(), iLevelindex, layerTag.c_str(), &pOut, objectNameTag.c_str());

			pTransform = (CTransform*)CGameInstance::GetInstance()->Get_Component(CTransform::familyId, pOut);

			Safe_Release(pOut);
			if (nullptr == pTransform)
				continue;

			pTransform->Set_Scaled(vScale);
			pTransform->Set_Rotation(vRot);
			pTransform->Set_Pos(vPos.x, vPos.y, vPos.z);

		}

		fclose(fp);
		Safe_Delete_Array(readBuffer);
	}
	return S_OK;
}

_bool CClient_Utility::Mouse_Pos_In_Platform()
{
	POINT ptMouse{};
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	if (ptMouse.x >= 0 && ptMouse.x < g_iWinSizeX &&
		ptMouse.y >= 0 && ptMouse.y < g_iWinSizeY)
		return true;

	return false;
}

void CClient_Utility::Play_Monster_SignitureSound(const _uint& monsterNo)
{
	wstring sound = L"2d_pv";

	if (10 > monsterNo)
	{
		sound += L"00" + to_wstring(monsterNo);
	}
	else if (100 > monsterNo)
	{
		sound += L"0" + to_wstring(monsterNo);

	}
	sound += L".ogg";

	CGameInstance::GetInstance()->PlaySoundW(sound.c_str(), 0.65f);
}

HRESULT CClient_Utility::Load_StageEnv(const char* filePath, const _tchar* layerTag, const _uint& iLevelIndex)
{
	if (nullptr == filePath)
		return E_FAIL;

	FILE* fp = fopen(filePath, "rb"); // non-Windows use "r"

	if (NULL == fp)
	{
		MSG_BOX("Load File Open Error");
		return E_FAIL;
	}
	else
	{
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> convert;

		char* readBuffer = new char[65536];
		FileReadStream is(fp, readBuffer, sizeof(readBuffer));

		Document document;
		document.ParseStream(is);

		wstring objectNameTag;

		CTransform* pTransform = nullptr;

		const Value& StageEnvs = document["StageEnvs"];
		assert(StageEnvs.IsArray());
		for (SizeType i = 0; i < StageEnvs.Size(); ++i)
		{
			CGameObject* pOut = nullptr;

			CStageEnv::STAGE_ENV_DESC desc{};

			desc.vScale.x = StageEnvs[i]["ScaleX"].GetFloat();
			desc.vScale.y = StageEnvs[i]["ScaleY"].GetFloat();
			desc.vScale.z = StageEnvs[i]["ScaleZ"].GetFloat();

			desc.vRotate.x = StageEnvs[i]["RotX"].GetFloat();
			desc.vRotate.y = StageEnvs[i]["RotY"].GetFloat();
			desc.vRotate.z = StageEnvs[i]["RotZ"].GetFloat();

			desc.vPos.x = StageEnvs[i]["PosX"].GetFloat();
			desc.vPos.y = StageEnvs[i]["PosY"].GetFloat();
			desc.vPos.z = StageEnvs[i]["PosZ"].GetFloat();

			desc.ModelPrototypeTag = convert.from_bytes(StageEnvs[i]["ModelPrototypeTag"].GetString());

			objectNameTag = convert.from_bytes(StageEnvs[i]["ObjectNameTag"].GetString());

			if (objectNameTag.empty())
				CGameInstance::GetInstance()->Add_GameObject(TEXT("Prototype_GameObject_StageEnv"), iLevelIndex, layerTag, nullptr, &desc);
			else
				CGameInstance::GetInstance()->Add_GameObject(TEXT("Prototype_GameObject_StageEnv"), iLevelIndex, layerTag, objectNameTag.c_str(), &desc);
		}

		fclose(fp);
		Safe_Delete_Array(readBuffer);
	}
	return S_OK;
}

void CClient_Utility::Free()
{
}
