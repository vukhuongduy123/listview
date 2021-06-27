#include <iostream>
#include <windows.h>
#include <commctrl.h>
using namespace std;
struct table
{
    string lastUpDate;
    string type;
    string name;
    string unit;
    string price;
    string leftover;
    table* next;
    int key;
};
class linkedlist
{
public:
    int numberofNode=0;
    linkedlist();
    ~linkedlist();
    void insertNew(table Table);
    void deleteAnode(int);
    void print();
    void setdata(HWND hwndListView);
    void addnew(HWND hwndListView);
    table* SearchItem(int target);
    table* getData();
    bool ChangeItem(int target,table T);
private:
    table* oTable;
};

