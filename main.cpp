#include <Windows.h>
#include <stdio.h>
#include <vector>
#include <exception>
using namespace std;

struct MyException : public exception
{
    const char * what() const throw();
};

int loopNum = 0;
bool Skip = false;
vector<string> enumList;

string ignoreSkipList[] =
{
    "Chrome_WidgetWin_1",
    "Windows.UI.Core.CoreWindow",
    "ApplicationFrameWindow"
};

void deleteNotNeedToSkipList()
{
    for(int i=enumList.size();i>=0;i--)
    {
        for(int j=0;j<sizeof(ignoreSkipList)/sizeof(ignoreSkipList[0]);j++)
        {
            if(!strcmp(enumList[i].c_str(), ignoreSkipList[j].c_str()))
            {
                enumList.pop_back();
                break;
            }
        }
    }
};

BOOL CALLBACK EnumWindowSkipMySelfProc(HWND hWnd, LPARAM lParam)
{
	TCHAR szClass[256] = { 0 };
	TCHAR szString[1024] = { 0 };
	::GetWindowText(hWnd, szString, 255);
	::GetClassName(hWnd, szClass, 255);
	if (::IsWindow(hWnd) &&
		::IsWindowVisible(hWnd) &&
		(::GetWindowLong(hWnd, GWL_EXSTYLE) & WS_EX_TOOLWINDOW) != WS_EX_TOOLWINDOW )
	{
        //printf("%s\n",(char *)szClass);
        enumList.push_back(szClass);
	}
 
	return TRUE;
}

BOOL CALLBACK EnumWindowProc(HWND hWnd, LPARAM lParam)
{
	TCHAR szClass[256] = { 0 };
	TCHAR szString[1024] = { 0 };
	::GetWindowText(hWnd, szString, 255);
	::GetClassName(hWnd, szClass, 255);
	if (::IsWindow(hWnd) &&
		::IsWindowVisible(hWnd) &&
		(::GetWindowLong(hWnd, GWL_EXSTYLE) & WS_EX_TOOLWINDOW) != WS_EX_TOOLWINDOW )
	{
        if(strstr((char *)szClass,"MainFrameDockWindow"))
            if(strstr((char *)szString,"Manga"))
            {
                loopNum++;
                //printf("%s\n",(char *)szClass);
            }
	}
 
	return TRUE;
}

int main(int, char**)
{
    EnumWindows(EnumWindowProc, NULL);
    loopNum = loopNum + 2;

    while(1)
    {
        keybd_event(VK_MENU, 0, 0, 0);
        //Sleep(500);
        for(int i=0;i<loopNum;i++)
        {
            keybd_event(VK_TAB, 0, 0, 0);
            keybd_event(VK_TAB, 0, KEYEVENTF_KEYUP, 0);
            Sleep(10);
        }
        keybd_event(VK_MENU, 0, KEYEVENTF_KEYUP, 0);

        EnumWindows(EnumWindowSkipMySelfProc, NULL);
        //printf("-------------------------------\n");
        deleteNotNeedToSkipList();
        //printf("enumList.back().c_str() = %s\n\n",enumList.back().c_str());
        
        if(!strcmp(enumList.back().c_str(), "CASCADIA_HOSTING_WINDOW_CLASS"))
            Skip = true;
        if(!strcmp(enumList.back().c_str(),"CabinetWClass"))
            Skip = true;
        if(!strcmp(enumList.back().c_str(), "SandboxieControlWndClass"))
            Skip = true;

        if(Skip)
        {
            Skip = false;
            Sleep(10);
            continue;
        }

        Skip = false;

        Sleep(10000);
    }

    //system("PAUSE");
    return 0;
}