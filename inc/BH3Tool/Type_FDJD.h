#ifndef TYPE_FDJD_H
#define TYPE_FDJD_H
#include <windows.h>

#define TT_STATE_START 0
#define TT_STATE_END 1
#define TT_MAIN 2
#define CONDITION_OFF 0
#define CONDITION_ON 1
#define SIGNAL_STOP -1
#define SIGNAL_NULL 0
#define SIGNAL_REFLASH 1
#define SIGNAL_PAUSE 2
#define SIGNAL_TIME_MAX 3
#define TIME_EXIT 120
#define TIME_NULL SIGNAL_NULL
#define METHOD_IS 0
#define METHOD_NOT 1

typedef struct pointlist
{
   int x;
   int y;
   COLORREF c;
   int checkmethod;
   struct pointlist *next;
}PointList;
typedef struct task
{
    int type;
    void (*content)(void);
    struct task *next;
}Task;
typedef struct state
{
    PointList *pl;
    Task *task;
    struct state *last;
    struct state *next;
}State;
typedef struct taskfunction
{
    void (*function)(DWORD);
    DWORD para;
    struct taskfunction *next;
}TaskFunction;

#endif