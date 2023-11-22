#ifndef DRIVER_FDJD_H
#define DRIVER_FDJD_H

#include <stdio.h>
#include <windows.h>
#include <time.h>
#include <conio.h>
#include <stdlib.h>
#include "Type_FDJD.h"

HDC hd;
void DriverInit(void);
void DriverExit(void);
void MouseClick(long x, long y);
void KeyPress(char k);
void KeyDown(char k);
void KeyUp(char k);
int CheckPoint(PointList *pl);
void MySleepInit(int *par);
void MySleep(int n);

int *TimeFlags;

#endif