#ifndef KEYBOARDHOOK_FDJD_H
#define KEYBOARDHOOK_FDJD_H

#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <time.h>
#include <pthread.h>

typedef struct actrecord
{
    char key;
    int state;
    clock_t time;
    struct actrecord *next;
}ActRecord;

ActRecord *AR_end, *AR_start;
int flag;

#endif