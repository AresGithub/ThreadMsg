#include "OsEvent.h"
#include <time.h>

void OSEventInit(OSEvent* evt)
{
    pthread_condattr_t cond_attr;
    pthread_mutex_init(&evt->mMutex, NULL);
    pthread_condattr_setclock(&cond_attr, CLOCK_MONOTONIC);
    pthread_cond_init(&evt->mCond, &cond_attr);
}

void OSEventUnInit(OSEvent* evt)
{
    pthread_mutex_destroy(&evt->mMutex);
    pthread_cond_destroy(&evt->mCond);
}

void OSEventTriggle(OSEvent* evt)
{
    pthread_cond_signal(&evt->mCond);
}

int OSEventWait(OSEvent* evt, PFUN_COND pCondFun, void* data, unsigned int timeout_ms)
{
    int ret = 0;
    if (pCondFun == NULL) {
        ret = -2;
        return ret;
    }
    pthread_mutex_lock(&evt->mMutex);
    if (timeout_ms == OSEVENT_WAIT_FOREVER) {
        while (!(ret = pCondFun(data))) {
            pthread_cond_wait(&evt->mCond, &evt->mMutex);
        }
    } else if (timeout_ms == OSEVENT_WAIT_NONE) {
        ret = pCondFun(data);
        pthread_mutex_unlock(&evt->mMutex);
        return ret;
    } else {
        struct timespec tv;
        clock_gettime(CLOCK_MONOTONIC, &tv);
        tv.tv_sec += timeout_ms/1000;
        tv.tv_nsec += (timeout_ms % 1000) * 1000000;
        while (!(ret = pCondFun(data))) {
            if (pthread_cond_timedwait(&evt->mCond, &evt->mMutex, &tv)) {
                ret = -1;
                pthread_mutex_unlock(&evt->mMutex);
                return ret;
            }
        }
    }
    pthread_mutex_unlock(&evt->mMutex);
    return ret;
}
