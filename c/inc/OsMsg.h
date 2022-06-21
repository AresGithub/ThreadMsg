#ifndef _OS_MSG_H_
#define _OS_MSG_H_

#include <pthread.h>
#include "OsEvent.h"

typedef struct _Msg {
    int value;
    void* data;
} Msg;

typedef struct _MsgPool {
    Msg* queue;
    pthread_mutex_t mutex;
    int size;
    int rp;
    int wp;
} MsgPool;

typedef struct _MsgBox {
    MsgPool mMsgPool;
    OSEvent mEvent;
} MsgBox;

void MsgBoxInit(MsgBox* box);
void MsgBoxUnInit(MsgBox* box);
int MsgBoxSendMsg(MsgBox* box, Msg msg);
int MsgBoxRecvMsg(MsgBox* box, Msg* msg);

#endif