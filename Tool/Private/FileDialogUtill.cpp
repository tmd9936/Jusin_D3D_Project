#include "FileDialogUtill.h"

//HRESULT CFileDialogUtill::File_Open_Dialog(string output)
//{
//    OPENFILENAME OFN;
//    TCHAR filePathName[100] = L"";
//    TCHAR lpstrFile[100] = L"";
//    static TCHAR filter[] = L"��� ����\0*.*\0�ؽ�Ʈ ����\0*.txt\0fbx ����\0*.fbx";
//
//    memset(&OFN, 0, sizeof(OPENFILENAME));
//    OFN.lStructSize = sizeof(OPENFILENAME);
//    OFN.hwndOwner = g_hWnd;
//    OFN.lpstrFilter = filter;
//    OFN.lpstrFile = lpstrFile;
//    OFN.nMaxFile = 100;
//    OFN.lpstrInitialDir = L".";
//
//    if (GetOpenFileName(&OFN) != 0) {
//        wsprintf(filePathName, L"%s ������ ���ڽ��ϱ�?", OFN.lpstrFile);
//        MessageBox(g_hWnd, filePathName, L"���� ����", MB_OK);
//
//        string t = WstringToString(OFN.lpstrFile);
//
//        int a = 0;
//    }
//
//}

string CFileDialogUtill::WstringToString(wstring value)
{
    string temp;
    //temp.assign(value.begin(), value.end());
    return temp;
}
