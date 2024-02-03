#include "Driver_FDJD.h"

void MouseClick(long x, long y)
{
    INPUT ip;
    if(x>=0 && y>=0)
    {
        ip.type = INPUT_MOUSE;
        ip.mi.dx = x*65535/1920;
        ip.mi.dy = y*65535/1080;
        ip.mi.mouseData = 0;
        ip.mi.dwFlags = MOUSEEVENTF_ABSOLUTE|MOUSEEVENTF_MOVE|MOUSEEVENTF_LEFTDOWN|MOUSEEVENTF_LEFTUP;
        ip.mi.time = 0;
    }
    else
    {
        ip.type = INPUT_MOUSE;
        ip.mi.dx = 0;
        ip.mi.dy = 0;
        ip.mi.mouseData = 0;
        ip.mi.dwFlags = MOUSEEVENTF_LEFTDOWN|MOUSEEVENTF_LEFTUP;
        ip.mi.time = 0;
    }
    ip.mi.dwExtraInfo = 0;

    SendInput(1, &ip, sizeof(ip));
}
void KeyDown(char k)
{
    INPUT ip;
    ip.type = INPUT_KEYBOARD;
    ip.ki.wVk = (WORD)(k&0XFF);
    ip.ki.dwFlags = 0;
    ip.ki.time = 0;
    ip.ki.dwExtraInfo = 0;

    SendInput(1, &ip, sizeof(ip));
}
void KeyUp(char k)
{
    INPUT ip;
    ip.type = INPUT_KEYBOARD;
    ip.ki.wVk = (WORD)(k&0XFF);
    ip.ki.dwFlags = KEYEVENTF_KEYUP;
    ip.ki.time = 0;
    ip.ki.dwExtraInfo = 0;

    SendInput(1, &ip, sizeof(ip));
}
void KeyPress(char k)
{
    KeyDown(k);
    Sleep(10);
    KeyUp(k);
}
int CheckPoint(PointList *pl)
{
    while(pl!=NULL)
    {
        if((pl->checkmethod)==METHOD_IS)
        {
            if(GetPixel(hd, pl->x,pl->y)!=(pl->c))
            {
                return CONDITION_OFF;
            }
        }
        else
        {
            if(GetPixel(hd, pl->x,pl->y)==(pl->c))
            {
                return CONDITION_OFF;
            }
        }
        pl = pl->next;
    }
    return CONDITION_ON;
}
void DriverInit(void)
{
    hd = GetDC(NULL);
    ClipCursor(NULL);
}
void DriverExit(void)
{
    DeleteObject(hd);
}
void MySleep(int n)
{
    static clock_t t;
    if(TimeFlags!=NULL)
    {
        t = clock();
        while((clock()-t)<(n) && (*TimeFlags)==TIME_NULL) {Sleep(0);}
        // if((clock()-t)>=(n)) printf("normal\n"); else printf("early\n");
    }
    else
        Sleep(n);
}
void MySleepInit(int *par){TimeFlags=par;}