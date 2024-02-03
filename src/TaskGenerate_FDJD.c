#include "TaskGenerate_FDJD.h"

void Task_User_0(void)
{
    static TaskFunction *sf;
    sf = User_ST;
    while(sf!=NULL && (*TimeFlag)==TIME_NULL)
    {
        sf->function(sf->para);
        sf = sf->next;
    }
}
void Task_User_1(void)
{
    static TaskFunction *mf;
    mf = User_MT;
    while(mf!=NULL && (*TimeFlag)==TIME_NULL)
    {
        mf->function(mf->para);
        mf = mf->next;
    }
}
void Task_User_2(void)
{
    static TaskFunction *ef;
    ef = User_ET;
    while(ef!=NULL && (*TimeFlag)==TIME_NULL)
    {
        ef->function(ef->para);
        ef = ef->next;
    }
}
void Task_0_0(void){MouseClick(1810, 70);}
void Task_1_0(void){MouseClick(1670, 850);}
void Task_2_0(void){MouseClick(1693, 1008);}
void Task_3_0(void){KeyDown('W');}
void Task_3_1(void){KeyPress(0X1B);}
void Task_4_0(void){MouseClick(1802, 973);MySleep(50);}
void Task_5_0(void){KeyPress('8');MySleep(200);KeyPress('I');MySleep(200);}
void Task_6_0(void){MySleep(100);KeyPress('R');}
void Task_6_1(void){KeyUp('W');}
void Task_7_0(void){KeyPress('J');}
void Task_8_0(void){KeyPress('U');MySleep(1000);}
void Task_8_1(void){MouseClick(954, 70);MySleep(500);}

void UserTaskInit(int *par)
{
    FILE *f;
    char buffer[50],*stmp;
    int funcnum;
    TaskFunction func[3],*pfunc[3];
    unsigned long t;

    TimeFlag = par;

    memset(buffer,'\0',sizeof(char)*50);
    funcnum = -1;
    User_ST=NULL;User_MT=NULL;User_ET=NULL;
    func[0].next=NULL;func[1].next=NULL;func[2].next=NULL;
    pfunc[0]=&func[0];pfunc[1]=&func[1];pfunc[2]=&func[2];

    f = fopen("config.txt","r");
    while(1)
    {
        do{
            memset(buffer,'\0',sizeof(char)*50);
            fgets(buffer,50,f);
        }while(buffer[0]=='#' || buffer[0]=='\n');
        if (feof(f)) break;
        if(strchr(buffer,':')!=NULL)
        {
            if(!strncmp(buffer,"StartTask:",10))
            {
                funcnum = 0;
            }
            else if(!strncmp(buffer,"MainTask:",9))
            {
                funcnum = 1;
            }
            else if(!strncmp(buffer,"EndTask:",8))
            {
                funcnum = 2;
            }
        }
        else if(strchr(buffer,'(')!=NULL)
        {
            pfunc[funcnum]->next = (TaskFunction *)malloc(sizeof(TaskFunction));
            pfunc[funcnum] = pfunc[funcnum]->next;
            pfunc[funcnum]->next = NULL;
            if(!strncmp(buffer,"\tSleep",6))
            {
                t = 0;
                sscanf(buffer,"\tSleep(%ld)\n",&t);
                pfunc[funcnum]->function = (void (*)(DWORD))MySleep;
                pfunc[funcnum]->para = t;
            }
            else if(!strncmp(buffer,"\tPress",6))
            {
                pfunc[funcnum]->function = (void (*)(DWORD))KeyPress;
                pfunc[funcnum]->para = buffer[7];
            }
            else if(!strncmp(buffer,"\tDown",5))
            {
                pfunc[funcnum]->function = (void (*)(DWORD))KeyDown;
                pfunc[funcnum]->para = buffer[6];
            }
            else if(!strncmp(buffer,"\tUp",3))
            {
                pfunc[funcnum]->function = (void (*)(DWORD))KeyUp;
                pfunc[funcnum]->para = buffer[4];
            }
        }
    }
    User_ST=func[0].next;User_MT=func[1].next;User_ET=func[2].next;
    fclose(f);
}
void UserTaskEnd(void)
{
    TaskFunction *ct,*nt;

    ct = User_ST;
    while (ct!=NULL)
    {
        nt = ct->next;
        free(ct);
        ct = nt;
    }
    ct = User_MT;
    while (ct!=NULL)
    {
        nt = ct->next;
        free(ct);
        ct = nt;
    }
    ct = User_ET;
    while (ct!=NULL)
    {
        nt = ct->next;
        free(ct);
        ct = nt;
    }
}
State* StateLoopGenerate(void)
{
    // 大厅界面
    PL[0][0].x=210; PL[0][0].y=926; PL[0][0].c=0X00000000;PL[0][0].checkmethod=METHOD_IS;PL[0][0].next=&(PL[0][1]);
    PL[0][1].x=1797;PL[0][1].y=467; PL[0][1].c=0X00FFFFFF;PL[0][1].checkmethod=METHOD_IS;PL[0][1].next=NULL;
    TL[0][0].type=TT_STATE_START;TL[0][0].content=Task_0_0;TL[0][0].next=NULL;
    // 难度选择
    PL[1][0].x=1800;PL[1][0].y=847; PL[1][0].c=0X0085C9F4;PL[1][0].checkmethod=METHOD_IS;PL[1][0].next=&(PL[1][1]);
    PL[1][1].x=349; PL[1][1].y=215; PL[1][1].c=0X006390AE;PL[1][1].checkmethod=METHOD_IS;PL[1][1].next=NULL;
    TL[1][0].type=TT_STATE_START;;TL[1][0].content=Task_1_0;TL[1][0].next=NULL;
    // 人物选择
    PL[2][0].x=817; PL[2][0].y=173; PL[2][0].c=0X00FFC300;PL[2][0].checkmethod=METHOD_IS;PL[2][0].next=&(PL[2][1]);
    PL[2][1].x=1544;PL[2][1].y=1004;PL[2][1].c=0X004BE1FF;PL[2][1].checkmethod=METHOD_IS;PL[2][1].next=NULL;
    TL[2][0].type=TT_STATE_START;TL[2][0].content=Task_2_0;TL[2][0].next=NULL;
    // 战斗场景1
    PL[3][0].x=180; PL[3][0].y=171;PL[3][0].c=0X00FFFFFF;PL[3][0].checkmethod=METHOD_IS;PL[3][0].next=&(PL[3][1]);
    PL[3][1].x=1788;  PL[3][1].y=42;  PL[3][1].c=0X00e6d4cd;PL[3][1].checkmethod=METHOD_IS;PL[3][1].next=&(PL[3][2]);
    PL[3][2].x=222; PL[3][2].y=170; PL[3][2].c=0X00FFFFFF;PL[3][2].checkmethod=METHOD_NOT;PL[3][2].next=NULL;
    TL[3][0].type=TT_STATE_START;TL[3][0].content=Task_3_0;TL[3][0].next=&(TL[3][1]);
    TL[3][1].type=TT_MAIN;TL[3][1].content=Task_6_0;TL[3][1].next=&(TL[3][2]);
    TL[3][2].type=TT_STATE_END;TL[3][2].content=Task_6_1;TL[3][2].next=NULL;
    // 战斗场景2
    PL[9][0].x=222; PL[9][0].y=181;PL[9][0].c=0X00FFFFFF;PL[9][0].checkmethod=METHOD_IS;PL[9][0].next=&(PL[9][1]);
    PL[9][1].x=190;  PL[9][1].y=160;  PL[9][1].c=0X00FFFFFF;PL[9][1].checkmethod=METHOD_IS;PL[9][1].next=&(PL[9][2]);
    PL[9][2].x=1010; PL[9][2].y=232;PL[9][2].c=0X00FFFFFF;PL[9][2].checkmethod=METHOD_NOT;PL[9][2].next=&(PL[9][3]);
    PL[9][3].x=948;  PL[9][3].y=225;  PL[9][3].c=0X00FFFFFF;PL[9][3].checkmethod=METHOD_NOT;PL[9][3].next=NULL;
    TL[9][0].type=TT_STATE_START;TL[9][0].content=Task_3_1;TL[9][0].next=NULL;
    TL_User_0.type=TT_STATE_START;TL_User_0.content=Task_User_0;TL_User_0.next=&TL_User_1;
    TL_User_1.type=TT_MAIN;TL_User_1.content=Task_User_1;TL_User_1.next=&TL_User_2;
    TL_User_2.type=TT_STATE_END;TL_User_2.content=Task_User_2;TL_User_2.next=NULL;
    // 人物对话
    PL[4][0].x=1641;PL[4][0].y=67;  PL[4][0].c=0X00FFFFFF;PL[4][0].checkmethod=METHOD_IS;PL[4][0].next=&(PL[4][1]);
    PL[4][1].x=1802;PL[4][1].y=973; PL[4][1].c=0X00313131;PL[4][1].checkmethod=METHOD_IS;PL[4][1].next=&(PL[4][2]);
    PL[4][2].x=186; PL[4][2].y=61;  PL[4][2].c=0X00FFFFFF;PL[4][2].checkmethod=METHOD_IS;PL[4][2].next=NULL;
    TL[4][0].type=TT_MAIN;TL[4][0].content=Task_4_0;TL[4][0].next=NULL;
    // 选择刻印
    PL[5][0].x=1026;PL[5][0].y=152; PL[5][0].c=0X0088CEFB;PL[5][0].checkmethod=METHOD_IS;PL[5][0].next=&(PL[5][1]);
    PL[5][1].x=1850;PL[5][1].y=905; PL[5][1].c=0X00443933;PL[5][1].checkmethod=METHOD_IS;PL[5][1].next=&(PL[5][2]);
    PL[5][2].x=1609;PL[5][2].y=1026;PL[5][2].c=0X00403F3F;PL[5][2].checkmethod=METHOD_IS;PL[5][2].next=NULL;
    TL[5][0].type=TT_MAIN;TL[5][0].content=Task_5_0;TL[5][0].next=NULL;
    // 确认锚点
    PL[6][0].x=456; PL[6][0].y=387; PL[6][0].c=0X004BE1FF;PL[6][0].checkmethod=METHOD_IS;PL[6][0].next=NULL;
    TL[6][0].type=TT_STATE_START;TL[6][0].content=Task_6_0;TL[6][0].next=NULL;
    // 退出界面
    PL[7][0].x=459; PL[7][0].y=995; PL[7][0].c=0X00748AFF;PL[7][0].checkmethod=METHOD_IS;PL[7][0].next=&(PL[7][1]);
    PL[7][1].x=1840;PL[7][1].y=996; PL[7][1].c=0X004BE1FF;PL[7][1].checkmethod=METHOD_IS;PL[7][1].next=NULL;
    TL[7][0].type=TT_STATE_START;TL[7][0].content=Task_7_0;TL[7][0].next=NULL;
    // 确认退出
    PL[8][0].x=841; PL[8][0].y=755; PL[8][0].c=0X00748AFF;PL[8][0].checkmethod=METHOD_IS;PL[8][0].next=&(PL[8][1]);
    PL[8][1].x=1066;PL[8][1].y=759; PL[8][1].c=0X004BE1FF;PL[8][1].checkmethod=METHOD_IS;PL[8][1].next=NULL;
    TL[8][0].type=TT_STATE_START;TL[8][0].content=Task_8_0;TL[8][0].next=NULL;
    // 战斗失败
    PL[10][0].x=841;PL[10][0].y=755; PL[10][0].c=0X00748AFF;PL[10][0].checkmethod=METHOD_NOT;PL[10][0].next=&(PL[10][1]);
    PL[10][1].x=1066; PL[10][1].y=759; PL[10][1].c=0X004BE1FF;PL[10][1].checkmethod=METHOD_NOT;PL[10][1].next=&(PL[10][2]);
    PL[10][2].x=210; PL[10][2].y=926; PL[10][2].c=0X00000000;PL[10][2].checkmethod=METHOD_NOT;PL[10][2].next=&(PL[10][3]);
    PL[10][3].x=1797;PL[10][3].y=467; PL[10][3].c=0X00FFFFFF;PL[10][3].checkmethod=METHOD_NOT;PL[10][3].next=NULL;
    TL[8][1].type=TT_MAIN;TL[8][1].content=Task_8_1;TL[8][1].next=NULL;
    // 已保存
    PL[11][0].x=1010; PL[11][0].y=232;PL[11][0].c=0X00FFFFFF;PL[11][0].checkmethod=METHOD_IS;PL[11][0].next=&(PL[11][1]);
    PL[11][1].x=948;  PL[11][1].y=225;  PL[11][1].c=0X00FFFFFF;PL[11][1].checkmethod=METHOD_IS;PL[11][1].next=&(PL[11][2]);
    PL[11][2].x=874;  PL[11][2].y=227;  PL[11][2].c=0X00FFFFFF;PL[11][2].checkmethod=METHOD_IS;PL[11][2].next=NULL;
    TL[9][0].type=TT_STATE_START;TL[9][0].content=Task_3_1;TL[9][0].next=NULL;


    SL[0].last=&SL[13];SL[0].pl=&PL[0][0];SL[0].task=&TL[0][0];SL[0].next=&SL[1];// 大厅界面
    SL[1].last=&SL[0];SL[1].pl=&PL[1][0];SL[1].task=&TL[1][0];SL[1].next=&SL[2];// 难度选择
    SL[2].last=&SL[1];SL[2].pl=&PL[2][0];SL[2].task=&TL[2][0];SL[2].next=&SL[3];// 人物选择
    SL[3].last=&SL[2];SL[3].pl=&PL[3][0];SL[3].task=NULL;SL[3].next=&SL[4];// 战斗场景1
    SL[4].last=&SL[3];SL[4].pl=&PL[4][0];SL[4].task=&TL[4][0];SL[4].next=&SL[5];// 人物对话
    SL[5].last=&SL[4];SL[5].pl=&PL[5][0];SL[5].task=&TL[5][0];SL[5].next=&SL[6];// 选择刻印
    SL[6].last=&SL[5];SL[6].pl=&PL[3][0];SL[6].task=&TL[3][0];SL[6].next=&SL[8];// 战斗场景1
    // SL[7].last=&SL[6];SL[7].pl=&PL[6][0];SL[7].task=&TL[6][0];SL[7].next=&SL[8];// 确认锚点
    SL[8].last=&SL[6];SL[8].pl=&PL[9][0];SL[8].task=&TL_User_0;SL[8].next=&SL[9];// 战斗场景2
    SL[9].last=&SL[8];SL[9].pl=&PL[4][0];SL[9].task=&TL[4][0];SL[9].next=&SL[10];// 人物对话
    SL[10].last=&SL[9];SL[10].pl=&PL[5][0];SL[10].task=&TL[5][0];SL[10].next=&SL[11];// 选择刻印
    SL[11].last=&SL[10];SL[11].pl=&PL[9][0];SL[11].task=&TL[9][0];SL[11].next=&SL[12];// 战斗场景2
    SL[12].last=&SL[11];SL[12].pl=&PL[7][0];SL[12].task=&TL[7][0];SL[12].next=&SL[13];// 退出界面
    SL[13].last=&SL[12];SL[13].pl=&PL[8][0];SL[13].task=&TL[8][0];SL[13].next=&SL[14];// 确认退出
    SL[14].last=&SL[13];SL[14].pl=&PL[10][0];SL[14].task=&TL[8][1];SL[14].next=&SL[0];// 战斗失败

    return &SL[14];
}