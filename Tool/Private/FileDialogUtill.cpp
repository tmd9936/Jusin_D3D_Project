#include "FileDialogUtill.h"

//HRESULT CFileDialogUtill::File_Open_Dialog(string output)
//{
//    OPENFILENAME OFN;
//    TCHAR filePathName[100] = L"";
//    TCHAR lpstrFile[100] = L"";
//    static TCHAR filter[] = L"모든 파일\0*.*\0텍스트 파일\0*.txt\0fbx 파일\0*.fbx";
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
//        wsprintf(filePathName, L"%s 파일을 열겠습니까?", OFN.lpstrFile);
//        MessageBox(g_hWnd, filePathName, L"열기 선택", MB_OK);
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
