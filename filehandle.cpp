#include <iostream>
#include <fstream>
#include <windows.h>
#include "linkedandstruct.h"
using namespace std;
void filetoStruct(linkedlist &linked)
{
    string str;
    table T;
    fstream file;
    file.open("data.txt",ios::app|ios::in|ios::out);
    if(!file.is_open())
    {
        cout<<"cant open file";
        return;
    }
    int getcount=0;
    while(getline(file,str,'|'))
    {
        if(getcount%6==0)
            T.lastUpDate=str;
        else if(getcount%6==1)
            T.type=str;
        else if(getcount%6==2)
            T.name=str;
        else if(getcount%6==3)
            T.unit=str;
        else if(getcount%6==4)
            T.price=str;
        else if(getcount%6==5)
        {
            T.leftover=str;
            linked.insertNew(T);
        }
        getcount++;
    }
    file.close();
}
void AddToFile(table T)
{
    fstream file;
    file.open("data.txt",ios::in|ios::out|ios::app);
    if(!file.is_open())
    {
        cout<<"cant open file";
        return;
    }
    string datatoAdd = T.lastUpDate+"|"+T.type+"|"+T.name+"|"+T.unit+"|"+T.price+"|"+T.leftover+"|";
    file<<datatoAdd<<endl;
    file.close();
}
void deleteTextFile(string text)
{
    fstream Ofile;
    fstream Nfile;
    string str;
    Ofile.open("data.txt",ios::in|ios::out|ios::app);
    Nfile.open("new.txt",ios::in|ios::out|ios::app);
    if(!Nfile.is_open())
    {
        cout<<"cant open file";
        return;
    }
    while(getline(Ofile,str,'\n'))
    {
        if(text!=str)
            Nfile<<str<<endl;
        else
            break;
    }
    while(getline(Ofile,str,'\n'))
        Nfile<<str<<endl;

    Ofile.close();
    Nfile.close();
    remove("data.txt");
    rename("new.txt","data.txt");
}
void ChangeText(string text,string newtext)
{
    fstream Ofile;
    fstream Nfile;
    string str;
    Ofile.open("data.txt",ios::in|ios::out|ios::app);
    Nfile.open("new.txt",ios::in|ios::out|ios::app);
    if(!Nfile.is_open())
    {
        cout<<"cant open file";
        return;
    }
    while(getline(Ofile,str,'\n'))
    {
        if(text!=str)
            Nfile<<str<<endl;
        else
        {
            Nfile<<newtext<<endl;
            break;
        }

    }
    while(getline(Ofile,str,'\n'))
        Nfile<<str<<endl;
    Ofile.close();
    Nfile.close();
    remove("data.txt");
    rename("new.txt","data.txt");
}
