#include "keyBoardHook.h"

LRESULT CALLBACK KeyPrroc(int nCode,WPARAM wParam, LPARAM lParam)
{
    static KBDLLHOOKSTRUCT LastlParam;
    KBDLLHOOKSTRUCT *p = (KBDLLHOOKSTRUCT *)lParam;

    if(nCode==HC_ACTION && (LastlParam.vkCode!=p->vkCode || LastlParam.flags!=p->flags))
    {
        if(AR_start==NULL)
        {
            AR_end = (ActRecord *)malloc(sizeof(ActRecord));
            AR_start = AR_end;
        }
        else
        {
            AR_end = (ActRecord *)malloc(sizeof(ActRecord));
        }
        AR_end->next = NULL;
        AR_end->time = p->time;
        AR_end->key = (char)(p->vkCode);
        AR_end->state = p->flags;
        AR_end = AR_end->next;
        LastlParam = *p;
    }
    return CallNextHookEx(NULL,nCode,wParam,lParam);
}
void OutRecod(void *id)
{
    ActRecord *AR_current;
    clock_t last_t;
    FILE *f;

    last_t = 0;
    AR_current = NULL;
    f = fopen("keyListen.txt","w");
    while(1)
    {
        while(AR_start==AR_end || AR_start==NULL){Sleep(0);};
        AR_current = AR_start;
        if(AR_current->key=='\e') break;
        fprintf(f,"\tSleep(%d)\n\t%s(%c)\n",1000*(AR_current->time-last_t)/CLOCKS_PER_SEC,AR_current->state==0?"Down":"Up",AR_current->key);
        last_t = AR_current->time;
        AR_start = AR_start->next;
        free(AR_current);
    }
    fclose(f);
    free(AR_current);
    if(PostThreadMessage(*((DWORD*)id),WM_QUIT,0,0)==0)
    {
        printf("%d,Post\n",GetLastError());
    }
}
int main(void)
{
    HHOOK keyHook;
    MSG msg;
    HANDLE tid;
    DWORD t;

    AR_start = NULL;
    AR_end = NULL;
    keyHook = NULL;

    keyHook = SetWindowsHookEx(WH_KEYBOARD_LL,KeyPrroc,GetModuleHandle(NULL),0);
    t = GetCurrentThreadId();
    tid = CreateThread(NULL,0,(long unsigned int(*)(void *))OutRecod,&t,0,NULL);
    // pthread_create(&tid, NULL, (void *(*)(void *))OutRecod, &t);

    while(GetMessage(&msg,NULL,0,0)!=0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    UnhookWindowsHookEx(keyHook);
    WaitForSingleObject(tid,INFINITE);
    // pthread_join(tid,NULL);
    printf("quit\n");

    return 0;
}