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
    // PointList PL[12][4];

    // PL[7][0].x=459; PL[7][0].y=995; PL[7][0].c=0X00748AFF;PL[7][0].checkmethod=METHOD_IS;PL[7][0].next=&(PL[7][1]);
    // PL[7][1].x=1840;PL[7][1].y=996; PL[7][1].c=0X004BE1FF;PL[7][1].checkmethod=METHOD_IS;PL[7][1].next=NULL;

    // PL[8][0].x=841; PL[8][0].y=755; PL[8][0].c=0X00748AFF;PL[8][0].checkmethod=METHOD_IS;PL[8][0].next=&(PL[8][1]);
    // PL[8][1].x=1066;PL[8][1].y=759; PL[8][1].c=0X004BE1FF;PL[8][1].checkmethod=METHOD_IS;PL[8][1].next=NULL;

    // PL[10][0].x=841;PL[10][0].y=755; PL[10][0].c=0X00748AFF;PL[10][0].checkmethod=METHOD_NOT;PL[10][0].next=&(PL[10][1]);
    // PL[10][1].x=1066; PL[10][1].y=759; PL[10][1].c=0X004BE1FF;PL[10][1].checkmethod=METHOD_NOT;PL[10][1].next=&(PL[10][2]);
    // PL[10][2].x=1082; PL[10][2].y=383; PL[10][2].c=0X00FFFFFF;PL[10][2].checkmethod=METHOD_NOT;PL[10][2].next=&(PL[10][3]);
    // PL[10][3].x=1020;PL[10][3].y=393; PL[10][3].c=0X00FFFFFF;PL[10][3].checkmethod=METHOD_NOT;PL[10][3].next=NULL;
   
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

            // if(CheckPoint(&PL[10][0]))
            // {
            //     printf("OK\n");
            // }
            // else
            // {
            //     printf("FAILD\n");
            // }

            // printf("\a");
        }

    }
    WaitForSingleObject(tid_keylisten,INFINITE);
    DeleteObject(hd);
}