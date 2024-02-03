#include "Find_CheckPiont.h"

LRESULT CALLBACK KeyPrroc(int nCode,WPARAM wParam, LPARAM lParam)
{
    static KBDLLHOOKSTRUCT LastlParam;
    KBDLLHOOKSTRUCT *p = (KBDLLHOOKSTRUCT *)lParam;
    // static char lastkey=0;
    static int laststate=1;

    if(nCode==HC_ACTION)
    {
        if(p->vkCode=='\e') 
        {
            printf("keyexit\n",PostThreadMessage(GetThreadId(tid_keylisten),WM_QUIT,0,0));
            KeySignal = -1;
            return CallNextHookEx(NULL,nCode,wParam,lParam);
        }
        if(laststate!=(p->flags) && (p->flags)==0) KeySignal=1;
        laststate = p->flags;
    }
    return CallNextHookEx(NULL,nCode,wParam,lParam);
}
void KeyListen(void)
{
    MSG msg;

    keyHook = SetWindowsHookEx(WH_KEYBOARD_LL,KeyPrroc,GetModuleHandle(NULL),0);
    while(KeySignal!=-1)
    {
        if(PeekMessageA(&msg,NULL,0,0,PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    UnhookWindowsHookEx(keyHook);
    return;
}
PointList FindPiont(void)
{
    POINT pos;
    PointList pl;

    GetCursorPos(&pos);
    pl.x = (int)(1.251*pos.x);
    pl.y = (int)(1.252*pos.y);
    pl.c = GetPixel(hd, pl.x, pl.y);
    pl.next = NULL;

    return pl;
}
int CheckPoint(PointList *pl)
{
    while(pl!=NULL)
    {
        if((pl->checkmethod)==METHOD_IS)
        {
            if(GetPixel(hd, pl->x,pl->y)!=(pl->c))
            {
                return 0;
            }
        }
        else
        {
            if(GetPixel(hd, pl->x,pl->y)==(pl->c))
            {
                return 0;
            }
        }
        pl = pl->next;
    }
    return 1;
}
int main(void)
{
    PointList pp;
    PointList PL[12][4];
    
    PL[11][0].x=1010; PL[11][0].y=232;PL[11][0].c=0X00FFFFFF;PL[11][0].checkmethod=METHOD_IS;PL[11][0].next=&(PL[11][1]);
    PL[11][1].x=948;  PL[11][1].y=225;  PL[11][1].c=0X00FFFFFF;PL[11][1].checkmethod=METHOD_IS;PL[11][1].next=&(PL[11][2]);
    PL[11][2].x=874;  PL[11][2].y=227;  PL[11][2].c=0X00FFFFFF;PL[11][2].checkmethod=METHOD_IS;PL[11][2].next=NULL;
   
    hd = GetDC(NULL);
    KeySignal = 0;
    ClipCursor(NULL);

    tid_keylisten = CreateThread(NULL,0,(long unsigned int(*)(void *))KeyListen,NULL,0,NULL);
    while(KeySignal!=-1)
    {
        Sleep(0);
        if(KeySignal==1)
        {
            KeySignal = 0;

            pp = FindPiont();
            printf("%d,%d:%6x\n",pp.x,pp.y,pp.c);

            if(CheckPoint(&PL[11][0]))
            {
                printf("OK\n");
            }
            else
            {
                printf("FAILD\n");
            }

            printf("\a");
        }

    }
    WaitForSingleObject(tid_keylisten,INFINITE);
    DeleteObject(hd);
}