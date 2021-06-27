#include <windows.h>
#include <commdlg.h>
#include <iostream>
#include <commctrl.h>
#include <tchar.h>
#include <iostream>


#define MAX_STRING_LEN 256
#define NUMOFITEMS 6


using namespace std;
static TCHAR szFindText [MAX_STRING_LEN] ;
static TCHAR szReplText [MAX_STRING_LEN] ;

HWND FindDialog(HWND hwnd)
{
    static FINDREPLACE fr;
    fr.lStructSize=sizeof(FINDREPLACE);
    fr.hwndOwner=hwnd;
    fr.hInstance=NULL;
    fr.Flags = FR_HIDEUPDOWN | FR_HIDEMATCHCASE | FR_HIDEWHOLEWORD ;
    fr.lpstrFindWhat=szFindText;
    fr.lpstrReplaceWith=NULL;
    fr.wFindWhatLen = MAX_STRING_LEN ;
    fr.wReplaceWithLen = 0 ;
    fr.lCustData = 0 ;
    fr.lpfnHook = NULL ;
    fr.lpTemplateName = NULL ;
    return FindText(&fr);
}
int findnext(HWND hwndListView,LPFINDREPLACE pfr,int pos,int number)
{
    TCHAR text[80];
    for(int i=pos;i<number;i++)
        for(int j=0;j<NUMOFITEMS;j++)
        {
            ListView_GetItemText(hwndListView,i,j,text,80);
            if(strstr(text,pfr->lpstrFindWhat)!=NULL)
                return i;
        }
    return -1;
}
int filter(HWND hwndListView,LPTSTR text,int i)
{
    TCHAR oText[80];
    for(int j=0;j<NUMOFITEMS;j++)
    {
        ListView_GetItemText(hwndListView,i,j,oText,80);
        if(strstr(oText,text)!=NULL)
            return i;
    }
    return -1;
}
