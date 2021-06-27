#include <tchar.h>
#include <windows.h>
#include <commctrl.h>
#include "linkedandstruct.h"
#include <string>
using namespace std;

linkedlist::linkedlist():oTable(0){}
linkedlist::~linkedlist()
{
    table* temp;
    while(oTable!=NULL)
    {
        temp=oTable;
        oTable=oTable->next;
        delete temp;
    }
}
void linkedlist::insertNew(table Table)
{
    table* newnode= new table();
    newnode->lastUpDate=Table.lastUpDate;
    newnode->type=Table.type;
    newnode->name=Table.name;
    newnode->unit=Table.unit;
    newnode->price=Table.price;
    newnode->leftover=Table.leftover;
    newnode->key=numberofNode;
    newnode->next=oTable;
    oTable=newnode;
    numberofNode++;
}
void linkedlist::deleteAnode(int target)
{
        table *temp=oTable,*current;
        if(temp&&temp->key==target)
        {
            current=temp;
            oTable=oTable->next;
            numberofNode--;
            delete current;

            return;
        }
        while(temp->next!=NULL)
        {
            if(temp->next->key==target)
            {
                current=temp->next;
                temp->next=temp->next->next;
                delete current;
                numberofNode--;
                return;
            }
            temp=temp->next;
        }
}
void linkedlist::print()
{
table* temp=oTable;
while(temp!=NULL)
{
    cout<<numberofNode<<"  "<<temp->type<<endl;
    temp=temp->next;
}
}
void linkedlist::setdata(HWND hwndListView)
{
    table *temp=oTable;
    LVITEM item;
    for(int i=0;i<numberofNode;++i)
    {
        item.lParam=(LPARAM)temp;
        item.mask=LVIF_TEXT|LVIF_PARAM;
        item.pszText=(char*)temp->lastUpDate.c_str();
        item.iItem=i;
        item.iSubItem=0;
        ListView_InsertItem(hwndListView, &item);
        item.mask=LVIF_TEXT;
        item.pszText=(char*)temp->type.c_str();
        item.iItem=i;
        item.iSubItem=1;
        ListView_SetItem(hwndListView, &item);
        item.mask=LVIF_TEXT;
        item.pszText=(char*)temp->name.c_str();
        item.iItem=i;
        item.iSubItem=2;
        ListView_SetItem(hwndListView, &item);
        item.mask=LVIF_TEXT;
        item.pszText=(char*)temp->unit.c_str();
        item.iItem=i;
        item.iSubItem=3;
        ListView_SetItem(hwndListView, &item);
        item.mask=LVIF_TEXT;
        item.pszText=(char*)temp->price.c_str();
        item.iItem=i;
        item.iSubItem=4;
        ListView_SetItem(hwndListView, &item);
        item.mask=LVIF_TEXT;
        item.pszText=(char*)temp->leftover.c_str();
        item.iItem=i;
        item.iSubItem=5;
        ListView_SetItem(hwndListView, &item);
        item.mask=LVIF_TEXT;
        item.pszText=(char*)to_string(temp->key).c_str();
        item.iItem=i;
        item.iSubItem=6;
        ListView_SetItem(hwndListView, &item);
        temp=temp->next;
    }
}
void linkedlist::addnew(HWND hwndListView)
{
    table *temp=oTable;
    LVITEM item;
    item.lParam=(LPARAM)temp;
    item.mask=LVIF_TEXT|LVIF_PARAM;
    item.pszText=(char*)temp->lastUpDate.c_str();
    item.iItem=0;
    item.iSubItem=0;
    ListView_InsertItem(hwndListView, &item);
    item.mask=LVIF_TEXT;
    item.pszText=(char*)temp->type.c_str();
    item.iItem=0;
    item.iSubItem=1;
    ListView_SetItem(hwndListView, &item);
    item.mask=LVIF_TEXT;
    item.pszText=(char*)temp->name.c_str();
    item.iItem=0;
    item.iSubItem=2;
    ListView_SetItem(hwndListView, &item);
    item.mask=LVIF_TEXT;
    item.pszText=(char*)temp->unit.c_str();
    item.iItem=0;
    item.iSubItem=3;
    ListView_SetItem(hwndListView, &item);
    item.mask=LVIF_TEXT;
    item.pszText=(char*)temp->price.c_str();
    item.iItem=0;
    item.iSubItem=4;
    ListView_SetItem(hwndListView, &item);
    item.mask=LVIF_TEXT;
    item.pszText=(char*)temp->leftover.c_str();
    item.iItem=0;
    item.iSubItem=5;
    ListView_SetItem(hwndListView, &item);
    item.mask=LVIF_TEXT;
    item.pszText=(char*)to_string(temp->key).c_str();
    item.iItem=0;
    item.iSubItem=6;
    ListView_SetItem(hwndListView, &item);
}
table* linkedlist::SearchItem(int target)
{
    table* temp=oTable;
    while(temp!=NULL)
        if(temp->key==target)
            return temp;
    return 0;
}
bool linkedlist::ChangeItem(int target,table T)
{
    table* temp=oTable;
    while(temp!=NULL)
    {
        if(temp->key==target)
        {
            temp->type=T.type;
            temp->name=T.name;
            temp->unit=T.unit;
            temp->price=T.price;
            temp->leftover=T.leftover;
            return true;
        }
        temp=temp->next;
    }

    return false;
}
