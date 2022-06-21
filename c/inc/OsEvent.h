#ifndef _OS_EVENT_H_
#define _OS_EVENT_H_

#include <pthread.h>

#define OSEVENT_WAIT_NONE       0x00000000
#define OSEVENT_WAIT_FOREVER    0xFFFFFFFF

typedef int (*PFUN_COND)(void* );

typedef struct _OSEvent {
    pthread_cond_t  mCond;
    pthread_mutex_t mMutex;
} OSEvent;

void OSEventInit(OSEvent* evt);
void OSEventUnInit(OSEvent* evt);
void OSEventTriggle(OSEvent* evt);
int OSEventWait(OSEvent* evt, PFUN_COND pCondFun, void* data, unsigned int timeout_ms);

#endif