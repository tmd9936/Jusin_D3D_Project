#pragma once

#include "Client_Defines.h"
#include "Base.h"


BEGIN(Engine)

class CTransform;

END

BEGIN(Client)

class CClient_Utility : public CBase
{
private:
    CClient_Utility() {}
    ~CClient_Utility() {}

public:
    static HRESULT Load_Layer_GameObjects(const char* filePath);
    static _bool Mouse_Pos_In_Platform();

public:
    // CBase을(를) 통해 상속됨
    virtual void Free(void) override;

};

END