#ifndef TASKGENERATE_FDJD_H
#define TASKGENERATE_FDJD_H

#include <stdio.h>
#include <string.h>
#include "Type_FDJD.h"
#include "Driver_FDJD.h"

State SL[16];
Task TL[10][3],TL_User_0,TL_User_1,TL_User_2;
PointList PL[12][4];
TaskFunction *User_ST,*User_MT,*User_ET;
int *TimeFlag;

State* StateLoopGenerate(void);
void UserTaskInit(int *par);
void UserTaskEnd(void);

#endif