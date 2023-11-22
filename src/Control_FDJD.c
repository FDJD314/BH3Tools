#include "Control_FDJD.h"

void* StateChange(void* par)
{
    State *current = (State *)par;
    clock_t t_exit;
    unsigned char piontstate;

    t_exit = clock();
    while (1)
    {
        Sleep(1);
        if (TaskSignal==SIGNAL_STOP || (clock()-t_exit)/CLOCKS_PER_SEC>TIME_EXIT) 
        {
            TaskSignal = SIGNAL_STOP;
            break;
        }

        piontstate = 0;
        if(CheckPoint(current->last->pl)==CONDITION_ON) piontstate|=0X01;
        if(CheckPoint(current->pl)==CONDITION_ON) piontstate|=0X02;
        if(CheckPoint(current->next->pl)==CONDITION_ON) piontstate|=0X04;
        if ((piontstate&0X04)!=0)
        {
            if(TaskSignal==SIGNAL_STOP) break;
            // if(TaskSignal!=SIGNAL_REFLASH) printf("reflash\n");
            TaskSignal = SIGNAL_REFLASH;
            t_exit = clock();
            current = current->next;
            CurrentTask = current->task;
            continue;
        }
        if(CurrentTask==NULL)
        {
            if (piontstate==0X00)
            {
                if(TaskSignal==SIGNAL_STOP) break;
                // if(TaskSignal!=SIGNAL_PAUSE) printf("pause\n");
                TaskSignal = SIGNAL_PAUSE;
            }
            else if(piontstate==0X02)
            {
                if(TaskSignal==SIGNAL_STOP) break;
                // if(TaskSignal!=SIGNAL_NULL) printf("null\n");
                TaskSignal = SIGNAL_NULL;
            }
        }
    }
    return NULL;
}
void TaskAssigned(Task *tasklist)
{
    static Task *st, *mt, *et,sth,mth,eth;

    sth.next=NULL;mth.next=NULL;eth.next=NULL;
    st=&sth;mt=&mth;et=&eth;
    while(tasklist!=NULL)
    {
        switch(tasklist->type)
        {
            case TT_STATE_START:
                st->next = (Task *)malloc(sizeof(Task));
                st = st->next;
                *st = *tasklist;
                st->next = NULL;
                break;
            case TT_MAIN:
                mt->next = (Task *)malloc(sizeof(Task));
                mt = mt->next;
                *mt = *tasklist;
                mt->next = NULL;
                break;
            case TT_STATE_END:
                et->next = (Task *)malloc(sizeof(Task));
                et = et->next;
                *et = *tasklist;
                et->next = NULL;
                break;
            default:
                break;
        }
        tasklist = tasklist->next;
    }
    TL_S = sth.next;
    mt->next = mth.next;
    TL_M = mth.next;
    TL_E = eth.next;
}
void StartTaskPro(Task *p)
{
    static Task *ct,*nt;
    static int i;

    // printf("StartTask\n");
    ct = p;
    while(ct!=NULL)
    {
        nt = ct->next;
        ct->content();
        free(ct);
        ct = nt;
    }
}
void MainTaskPro(Task *p)
{
    static Task *ct,*nt;
    static int i;

    // printf("MainTask\n");
    ct = p;
    i=0;
    while(TaskSignal!=SIGNAL_REFLASH && TaskSignal!=SIGNAL_STOP)
    {
        Sleep(1);
        if(TaskSignal!=SIGNAL_PAUSE)
        {
            ct->content();
            // i++;
            // if(i>100) break;
        }
        ct = ct->next;
    }
    nt = ct->next;
    ct->next = NULL;
    ct = nt;
    while(ct!=NULL)
    {
        nt = ct->next;
        free(ct);
        ct = nt;
    }
}
void EndTaskPro(Task *p)
{
    static Task *ct,*nt;
    static int i;

    // printf("EndTask\n");
    ct = p;
    while(ct!=NULL)
    {
        nt = ct->next;
        ct->content();
        free(ct);
        ct = nt;
    }
}
void Toolexit(void)
{
    Task *t;

    printf("exit\n");
    WaitForSingleObject(tid,INFINITE);
    WaitForSingleObject(tid_keylisten,INFINITE);
    while(TL_S!=NULL)
    {
        t = TL_S->next;
        free(TL_S);
        TL_S = t;
    }
    if(TL_M!=NULL)
    {
        t = TL_M;
        TL_M = t->next;
        t->next = NULL;
    }
    while(TL_M!=NULL)
    {
        t = TL_M->next;
        free(TL_M);
        TL_M = t;
    }
    while(TL_E!=NULL)
    {
        t = TL_E->next;
        free(TL_E);
        TL_E = t;
    }
    UserTaskEnd();
    DriverExit();
    system("pause");
}
LRESULT CALLBACK KeyPrroc(int nCode,WPARAM wParam, LPARAM lParam)
{
    static KBDLLHOOKSTRUCT LastlParam;
    KBDLLHOOKSTRUCT *p = (KBDLLHOOKSTRUCT *)lParam;

    if(nCode==HC_ACTION)
    {
        // printf("%c\n",p->vkCode);
        if(p->vkCode==' ') 
        {
            printf("keyexit\n",PostThreadMessage(GetThreadId(tid_keylisten),WM_QUIT,0,0));
            TaskSignal = SIGNAL_STOP;
        }
    }
    return CallNextHookEx(NULL,nCode,wParam,lParam);
}
void KeyListen(void)
{
    MSG msg;

    keyHook = SetWindowsHookEx(WH_KEYBOARD_LL,KeyPrroc,GetModuleHandle(NULL),0);
    while(1)
    {
        if(TaskSignal==SIGNAL_STOP) break;
        if(PeekMessageA(&msg,NULL,0,0,PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    UnhookWindowsHookEx(keyHook);
    TaskSignal = SIGNAL_STOP;
    return;
}
int main(void)
{
    LoopCounter = 0;
    TaskSignal = SIGNAL_NULL;
    CurrentTask = NULL;

    // signal(SIGABRT, Toolexit);
    // atexit((void (*)(void))Toolexit);

    MySleepInit(&TaskSignal);
    DriverInit();
    UserTaskInit(&TaskSignal);printf("UserTask Read Finish\n");
    StateLoop = StateLoopGenerate();

    tid = CreateThread(NULL,0,(long unsigned int(*)(void *))StateChange,StateLoop,0,NULL);
    tid_keylisten = CreateThread(NULL,0,(long unsigned int(*)(void *))KeyListen,NULL,0,NULL);

    printf("StartLoop...\n");
    while(1)
    {
        Sleep(1);
        if(TaskSignal==SIGNAL_STOP) break;
        if(CurrentTask!=NULL && TL_S==NULL && TL_M==NULL && TL_E==NULL){TaskAssigned(CurrentTask);CurrentTask = NULL;}
        if(TaskSignal==SIGNAL_NULL && TL_S!=NULL){StartTaskPro(TL_S);TL_S=NULL;}
        if(TaskSignal==SIGNAL_NULL && TL_S==NULL && TL_M!=NULL){MainTaskPro(TL_M);TL_M=NULL;}
        if(TL_S==NULL && TL_M==NULL && TL_E!=NULL && TaskSignal==SIGNAL_REFLASH){EndTaskPro(TL_E);TL_E=NULL;}
    }
    Toolexit();
}