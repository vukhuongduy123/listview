#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif

#include <tchar.h>
#include <windows.h>
#include <iostream>
#include <commctrl.h>
#include <fstream>
#include "linkedandstruct.h"
#define NUMOFITEMS 6
using namespace std;
/*  Declare Windows procedure  */

HINSTANCE hInst;
HWND hGListView;
HWND hwndGsearchDialog;


void filetoStruct(linkedlist& linked);
void AddToFile(table T);
void deleteTextFile(string text);
void ChangeText(string text,string newtext);


HWND FindDialog(HWND);
int findnext(HWND hwndListView,LPFINDREPLACE pfr,int pos,int number);
int filter(HWND hwndListView,LPTSTR text,int i);


int CALLBACK ListViewCompareProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
void dateStringToNumber(string s,int& day,int &month,int &year);


LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK DialogPro(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK DialogInsert(HWND,UINT,WPARAM,LPARAM);
INT_PTR CALLBACK DialogFilter(HWND,UINT,WPARAM,LPARAM);
INT_PTR CALLBACK DialogChange(HWND,UINT,WPARAM,LPARAM);

TCHAR szClassName[ ] = _T("DUYDZ");

LPTSTR loginName="admin";
LPTSTR loginPass="admin123";
int WINAPI WinMain (HINSTANCE hThisInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpszArgument,
                     int nCmdShow)
{
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASS wincl;        /* Data structure for the windowclass */

    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClass (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
           0,                   /* Extended possibilites for variation */
           szClassName,         /* Classname */
           _T("VKD-3005"),       /* Title Text */
           WS_OVERLAPPEDWINDOW, /* default window */
           CW_USEDEFAULT,       /* Windows decides the position */
           CW_USEDEFAULT,       /* where the window ends up on the screen */
           CW_USEDEFAULT,                 /* The programs width */
           CW_USEDEFAULT,                 /* and height in pixels */
           NULL,        /* The window is a child-window to desktop */
           NULL,                /* No menu */
           hThisInstance,       /* Program Instance handler */
           NULL                 /* No Window Creation data */
           );
    /* Make the window visible on the screen */
    ShowWindow (hwnd, nCmdShow);
    /* Run the message loop. It will run until GetMessage() returns 0 */
        while (GetMessage (&messages, NULL, 0, 0))
        {
            if (hwndGsearchDialog == NULL || !IsDialogMessage (hwndGsearchDialog, &messages))
            {
                    TranslateMessage (&messages) ;
                    DispatchMessage (&messages) ;
            }
        }
    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}
bool check(LPTSTR name,int textname,LPTSTR pass,int textpass)
{
    bool flagName=false,flagPass=false;
    if(5==textname)
        {
        if(8==textpass)
        {
        for(int i=0;i<5;++i)
            if(name[i]!=loginName[i])
                return false;
        flagName=true;
        for(int i=0;i<8;++i)
            if(pass[i]!=loginPass[i])
                return false;
        flagPass=true;
        if(flagName&&flagPass)
            return true;
        return false;
        }
        else
            return false;
        }
    else
        return false;
}

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    NMLISTVIEW *pNm;
    string Dstr;
    static LPFINDREPLACE pfr;
    static TCHAR prev[80],CText[80];
    static int iSelect=0,pos=0;
    static table T;
    static table Ctable;
    static linkedlist linked;
    static int cxClient,cyClient;
    static HWND hwndListView,hwndAddButton,hwndDelButton,hwndSearchButton,hwndFilterButton,hwndResetButton,hwndChangeButton;
    static UINT messageFind;
    switch (message)                  /* handle the messages */
    {
        case WM_CREATE:
            if(DialogBox(hInst,TEXT("DIALOGMENU"),hwnd,DialogPro)==-1)
                SendMessage(hwnd,WM_CLOSE,0,0);
            hwndListView=CreateWindow(TEXT("SysListView32"), NULL, LVS_REPORT | WS_CHILD | WS_VISIBLE|LVS_SHOWSELALWAYS , 0, 0,0,0, hwnd, (HMENU) 0, hInst, NULL);
            hwndAddButton=CreateWindow (TEXT("button"),TEXT("Insert"),WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,0,0,0,0,hwnd, (HMENU) 1,hInst, NULL) ;
            hwndDelButton=CreateWindow (TEXT("button"),TEXT("Delete"),WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,0,0,0,0,hwnd, (HMENU) 2,hInst, NULL) ;
            hwndSearchButton=CreateWindow (TEXT("button"),TEXT("Search"),WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,0,0,0,0,hwnd, (HMENU) 3,hInst, NULL) ;
            hwndFilterButton=CreateWindow (TEXT("button"),TEXT("Filter"),WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,0,0,0,0,hwnd, (HMENU) 4,hInst, NULL) ;
            hwndResetButton=CreateWindow (TEXT("button"),TEXT("X"),WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,0,0,0,0,hwnd, (HMENU) 5,hInst, NULL) ;
            hwndChangeButton=CreateWindow (TEXT("button"),TEXT("Change"),WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,0,0,0,0,hwnd, (HMENU) 6,hInst, NULL) ;
            messageFind=RegisterWindowMessage(FINDMSGSTRING);
            hGListView=hwndListView;
            LV_COLUMN lvc;
            lvc.mask = LVCF_TEXT | LVCF_WIDTH;
            lvc.pszText = "LastUpadate";
			SendMessage(hwndListView, LVM_INSERTCOLUMN, 0,(LONG_PTR) &lvc);
			lvc.pszText = TEXT("Type");
			SendMessage(hwndListView, LVM_INSERTCOLUMN, 1,(LONG_PTR) &lvc);
			lvc.pszText = TEXT("Name");
			SendMessage(hwndListView, LVM_INSERTCOLUMN, 2,(LONG_PTR) &lvc);
			lvc.pszText = TEXT("Unit");
			SendMessage(hwndListView, LVM_INSERTCOLUMN, 3,(LONG_PTR) &lvc);
			lvc.pszText = TEXT("Price");
			SendMessage(hwndListView, LVM_INSERTCOLUMN, 4,(LONG_PTR) &lvc);
			lvc.pszText = TEXT("LeftOver");
			SendMessage(hwndListView, LVM_INSERTCOLUMN, 5,(LONG_PTR) &lvc);
			lvc.pszText = TEXT("Key");
			SendMessage(hwndListView, LVM_INSERTCOLUMN, 6,(LONG_PTR) &lvc);
			filetoStruct(linked);
			linked.setdata(hwndListView);
            return 0;
        case WM_SIZE:
            cxClient=LOWORD(lParam);
            cyClient=HIWORD(lParam);
            MoveWindow(hwndListView,0,30,cxClient,cyClient,1);
            MoveWindow(hwndDelButton,0,0,50,20,1);
            MoveWindow(hwndSearchButton,50,0,50,20,1);
            MoveWindow(hwndFilterButton,100,0,50,20,1);
            MoveWindow(hwndAddButton,150,0,50,20,1);
            MoveWindow(hwndChangeButton,200,0,50,20,1);
            MoveWindow(hwndResetButton,6*cxClient/7,0,20,20,1);
            for(int i=0;i<=NUMOFITEMS;++i)
                ListView_SetColumnWidth(hwndListView,i,cxClient/8);
            EnableWindow (hwndDelButton, FALSE) ;
            EnableWindow (hwndChangeButton, FALSE) ;
            return 0;
        case WM_COMMAND:
            switch(LOWORD(wParam))
            {
            case 1:
                DialogBoxParam(hInst,TEXT("DIALOGADD"),hwnd,DialogInsert,(LPARAM)&linked);
                return 0;
            case 2:
                TCHAR text[100];
                for(int i=0;i<NUMOFITEMS;++i)
                {
                    ListView_GetItemText(hwndListView,iSelect,i,text,100);
                    Dstr+=text;
                    Dstr+="|";
                }
                if(Dstr[0]=='\n')
                    Dstr.erase(0,1);
                ListView_GetItemText(hwndListView,iSelect,NUMOFITEMS,text,100);
                linked.deleteAnode(atoi(text));
                deleteTextFile(Dstr);
                ListView_DeleteItem(hwndListView,iSelect);
                EnableWindow(hwndDelButton,FALSE);
                return 0;
            case 3:
                hwndGsearchDialog=FindDialog(hwnd);
                return 0;
            case 4:
                DialogBox(hInst,TEXT("DIALOGFILTER"),hwnd,DialogFilter);
                return 0;
            case 5:
                ListView_DeleteAllItems(hwndListView);
                linked.setdata(hwndListView);
                return 0;
            case 6:
                ListView_GetItemText(hwndListView,iSelect,0,CText,80);
                Ctable.lastUpDate=CText;
                ListView_GetItemText(hwndListView,iSelect,1,CText,80);
                Ctable.type=CText;
                ListView_GetItemText(hwndListView,iSelect,2,CText,80);
                Ctable.name=CText;
                ListView_GetItemText(hwndListView,iSelect,3,CText,80);
                Ctable.unit=CText;
                ListView_GetItemText(hwndListView,iSelect,4,CText,80);
                Ctable.price=CText;
                ListView_GetItemText(hwndListView,iSelect,5,CText,80);
                Ctable.leftover=CText;
                ListView_GetItemText(hwndListView,iSelect,6,CText,80);
                Ctable.key=atoi(CText);
                if(DialogBoxParam(hInst,TEXT("DIALOGCHANGE"),hwnd,DialogChange,(LPARAM)&Ctable)==0)
                {
                    ListView_SetItemText(hwndListView,iSelect,0,(char*)Ctable.lastUpDate.c_str());
                    ListView_SetItemText(hwndListView,iSelect,1,(char*)Ctable.type.c_str());
                    ListView_SetItemText(hwndListView,iSelect,2,(char*)Ctable.name.c_str());
                    ListView_SetItemText(hwndListView,iSelect,3,(char*)Ctable.unit.c_str());
                    ListView_SetItemText(hwndListView,iSelect,4,(char*)Ctable.price.c_str());
                    ListView_SetItemText(hwndListView,iSelect,5,(char*)Ctable.leftover.c_str());
                    linked.ChangeItem(Ctable.key,Ctable);
                }

                EnableWindow(hwndChangeButton,FALSE);
                return 0;
            }
            return 0;
        case WM_NOTIFY:
            switch(LOWORD(wParam))
            {
            case 0:
                pNm=(NMLISTVIEW  *)lParam;
                if(((LPNMHDR)lParam)->code==NM_CLICK)
                {
                    iSelect=SendMessage(hwndListView,LVM_GETNEXTITEM,-1,LVNI_FOCUSED|LVNI_SELECTED);
                    if(iSelect!=-1)
                    {
                        EnableWindow(hwndDelButton,TRUE);
                        EnableWindow(hwndChangeButton,TRUE);
                    }
                    else
                    {
                        EnableWindow(hwndDelButton,FALSE);
                        EnableWindow(hwndChangeButton,FALSE);
                    }
                }
                if(((LPNMHDR)lParam)->code==LVN_COLUMNCLICK)
                {
                    ListView_SortItems(hwndListView,ListViewCompareProc,(LPARAM)(pNm->iSubItem));
                }
            }
            return 0;
        case WM_DESTROY:
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;
        default:
            if(message==messageFind)
            {
                pfr=(LPFINDREPLACE) lParam;
                if(pfr->Flags&FR_DIALOGTERM)
                {
                    pos=0;
                    hwndGsearchDialog=NULL;
                    for(int i=0;i<ListView_GetItemCount(hwndListView);++i)
                            ListView_SetItemState(hGListView,i,0, LVIS_DROPHILITED);
                }
                if(pfr->Flags&FR_FINDNEXT)
                {
                    if( prev!=NULL && strcmp(prev,pfr->lpstrFindWhat)!=0)
                    {
                        pos=0;
                        for(int i=0;i<ListView_GetItemCount(hwndListView);++i)
                            ListView_SetItemState(hGListView,i,0, LVIS_DROPHILITED);
                    }
                    strcpy(prev, pfr->lpstrFindWhat);
                    if(findnext(hwndListView,pfr,pos,ListView_GetItemCount(hwndListView))!=-1)
                    {
                        pos=findnext(hwndListView,pfr,pos,ListView_GetItemCount(hwndListView));
                        ListView_SetItemState(hGListView,pos,LVIS_DROPHILITED, LVIS_DROPHILITED|LVNI_FOCUSED|LVNI_SELECTED);
                        pos++;
                    }
                    else
                        MessageBox(hwnd,TEXT("Not Found"),TEXT("End of Search"),MB_OK);
                }
                return 0;
            }
            break;
    }
    return DefWindowProc (hwnd, message, wParam, lParam);
}
int CALLBACK ListViewCompareProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
    table* T1=(table*)lParam1;
    table* T2=(table*)lParam2;
    int day1,month1,year1,day2,month2,year2,result;
    dateStringToNumber(T1->lastUpDate,day1,month1,year1);
    dateStringToNumber(T2->lastUpDate,day2,month2,year2);
    if(T1&&T2)
    {
        switch(lParamSort)
        {
        case 0:
            if(year1==year2)
                if(month1==month2)
                    if(day1==day2)
                        result=0;
                    else
                        result=day1-day2;
                else
                    result=month1-month2;
            else
                result=year1-year2;
            break;
        case 1:
            result=(T1->type).compare(T2->type);
            break;
        case 2:
            result=(T1->name).compare(T2->name);
            break;
        case 3:
            result=(T1->unit).compare(T2->unit);
            break;
        case 4:
            result=(T1->price).compare(T2->price);
            break;
        case 5:
            result=(T1->leftover).compare(T2->leftover);
            break;
        }
    }
    return result;

}
INT_PTR CALLBACK DialogInsert(HWND hwnd, UINT message,WPARAM wParam, LPARAM lParam)
{
    static linkedlist* L;
    static table T;
    string s;
    SYSTEMTIME time;
    static HWND hwndEdit[6];
    LPTSTR content[6];
    int length[6];
    switch(message)
    {
    case WM_INITDIALOG:
        L=(linkedlist*)lParam;
        for(int i=1;i<6;i++)
            hwndEdit[i]=GetDlgItem(hwnd,20000+i);
        return true;
    case WM_COMMAND:
        switch(LOWORD(wParam))
        {
        case 20011:
            for(int i=1;i<6;i++)
            {
                length[i]=GetWindowTextLength(hwndEdit[i]);
                content[i]=new TCHAR[length[i]+1];
                GetWindowText(hwndEdit[i],content[i],length[i]+1);
            }
            GetLocalTime(&time);
            s=to_string(time.wDay)+"/"+to_string(time.wMonth)+"/"+to_string(time.wYear);
            T.lastUpDate=s;
            T.type=content[1];
            T.name=content[2];
            T.unit=content[3];
            T.price=content[4];
            T.leftover=content[5];
            AddToFile(T);
            L->insertNew(T);
            L->addnew(hGListView);
            ListView_SetItemState(hGListView,0,LVIS_FOCUSED|LVIS_SELECTED , LVIS_FOCUSED|LVIS_SELECTED);
            for(int i=1;i<6;i++)
                delete [] content[i];
            EndDialog(hwnd,0);
            return true;
        case 20010:
            EndDialog(hwnd,-1);
            return true;
        }
        break;
    }
    return false;
}
INT_PTR CALLBACK DialogPro(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static HWND hwndEdit[2],hwndParent;
    LPTSTR name,pass;
    int textname,textpass;
    switch(message)
    {
    case WM_INITDIALOG:
        hwndEdit[0]=GetDlgItem(hwnd,10000);
        hwndEdit[1]=GetDlgItem(hwnd,10001);
        hwndParent=GetParent(hwnd);
        return true;
    case WM_COMMAND:
        switch(LOWORD(wParam))
        {
        case 10003:
            textname=GetWindowTextLength(hwndEdit[0]);
            name=new TCHAR[textname+1];
            GetWindowText(hwndEdit[0],name,textname+1);
            textpass=GetWindowTextLength(hwndEdit[1]);
            pass=new TCHAR[textname+1];
            GetWindowText(hwndEdit[1],pass,textpass+1);
            if(check(name,textname,pass,textpass))
            {
                EndDialog(hwnd,0);
                delete[] name;
                delete[] pass;
                return true;
            }
            delete[] name;
            delete[] pass;
            return true;
        case 10002:
            EndDialog(hwnd,-1);
            return true;
        }
        break;
    }
    return false;
}
INT_PTR CALLBACK DialogFilter(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static LPTSTR text;
    static int length,number;
    static HWND hwndEditF;
    switch(message)
    {
    case WM_INITDIALOG:
        number=ListView_GetItemCount(hGListView);
        hwndEditF=GetDlgItem(hwnd,30001);
        return false;
    case WM_COMMAND:
        switch(LOWORD(wParam))
        {
        case 30011:
            length=GetWindowTextLength(hwndEditF);
            text=new TCHAR[length+1];
            GetWindowText(hwndEditF,text,length+1);
            for(int i=0,j=0;i<number;j++,i++)
                    if(filter(hGListView,text,j)==-1)
                        ListView_DeleteItem(hGListView,j--);
            delete[] text;
            EndDialog(hwnd,0);
            return true;
        case 30010:
            EndDialog(hwnd,-1);
            return true;
        }
    }
    return false;
}
INT_PTR CALLBACK DialogChange(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    SYSTEMTIME time;
    static string Nstr,Ostr;
    LPTSTR text;
    int length;
    static table* T;
    static HWND hwndCEdit[NUMOFITEMS+1];
    switch(message)
    {
    case WM_INITDIALOG:
        T=(table*) lParam;
        for(int i=0;i<NUMOFITEMS;i++)
            hwndCEdit[i]=GetDlgItem(hwnd,40000+i);
        hwndCEdit[6]=GetDlgItem(hwnd,1);
        SetWindowText(hwndCEdit[1],T->type.c_str());
        SetWindowText(hwndCEdit[2],T->name.c_str());
        SetWindowText(hwndCEdit[3],T->unit.c_str());
        SetWindowText(hwndCEdit[4],T->price.c_str());
        SetWindowText(hwndCEdit[5],T->leftover.c_str());
        SetWindowText(hwndCEdit[6],to_string(T->key).c_str());
        Ostr+=T->lastUpDate+"|"+T->type+"|"+T->name+"|"+T->unit+"|"+T->price+"|"+T->leftover+"|";
        if(Ostr[0]==10)
            Ostr.erase(0,1);
        return false;
    case WM_COMMAND:
        switch(LOWORD(wParam))
        {
        case 40010:
            EndDialog(hwnd,-1);
            return true;
        case 40011:
            length=GetWindowTextLength(hwndCEdit[1]);
            text=new TCHAR[length+1];
            GetWindowText(hwndCEdit[1],text,length+1);
            T->type=text;
            delete[] text;
            length=GetWindowTextLength(hwndCEdit[2]);
            text=new TCHAR[length+1];
            GetWindowText(hwndCEdit[2],text,length+1);
            T->name=text;
            delete[] text;
            length=GetWindowTextLength(hwndCEdit[3]);
            text=new TCHAR[length+1];
            GetWindowText(hwndCEdit[3],text,length+1);
            T->unit=text;
            delete[] text;
            length=GetWindowTextLength(hwndCEdit[4]);
            text=new TCHAR[length+1];
            GetWindowText(hwndCEdit[4],text,length+1);
            T->price=text;
            delete[] text;
            length=GetWindowTextLength(hwndCEdit[5]);
            text=new TCHAR[length+1];
            GetWindowText(hwndCEdit[5],text,length+1);
            T->leftover=text;
            delete[] text;
            GetLocalTime(&time);
            Nstr=to_string(time.wDay)+"/"+to_string(time.wMonth)+"/"+to_string(time.wYear);
            Nstr+="|"+T->type+"|"+T->name+"|"+T->unit+"|"+T->price+"|"+T->leftover+"|";
            ChangeText(Ostr,Nstr);
            EndDialog(hwnd,0);
            return true;
        }
    }
    return false;
}
void dateStringToNumber(string s,int& day,int &month,int &year)
{
    int nday,nmonth,nyear;
    nday=s.find('/',0);
    nmonth=s.find('/',nday+1);
    nyear=s.find('/',nmonth+1);
    day=stoi(s.substr(0,nday));
    month=stoi(s.substr(nday+1,nmonth));
    year=stoi(s.substr(nmonth+1,nyear));
}
