#ifndef CONTROL_FDJD_H
#define CONTROL_FDJD_H

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <signal.h>
#include "Type_FDJD.h"
#include "TaskGenerate_FDJD.h"
#include "Driver_FDJD.h"

void* StateChange(void* par);
void TaskAssigned(Task *tasklist);
void StartTaskPro(Task *p);
void MainTaskPro(Task *p);
void EndTaskPro(Task *p);
void Toolexit(void);

Task *CurrentTask,*TL_S,*TL_M,*TL_E;
int TaskSignal;
State *StateLoop;
HANDLE tid,tid_keylisten;
HHOOK keyHook;
int LoopCounter;

#endif