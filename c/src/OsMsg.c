#include "OsMsg.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MSG_QUEUE_SIZE  10

static void MsgPoolInit(MsgPool* pool, int size)
{
    pool->rp = 0;
    pool->wp = 0;
    pool->size = size;
    pthread_mutex_init(&pool->mutex, NULL);
    pool->queue = (Msg*)malloc(sizeof(Msg)*size);
}

static void MsgPoolUnInit(MsgPool* pool)
{
    if (pool) {
        pthread_mutex_unlock(&pool->mutex);
        free(pool->queue);
    }
}

static int MsgPoolIsEmpty(MsgPool* pool)
{
    int ret = 0;
    pthread_mutex_lock(&pool->mutex);
    ret = (pool->rp==pool->wp)?1:0;
    pthread_mutex_unlock(&pool->mutex);
    return ret;
}

static int MsgPoolPutMsg(MsgPool* pool, Msg msg)
{
    int wp, rp, size;
    pthread_mutex_lock(&pool->mutex);
    wp = pool->wp;
    rp = pool->rp;
    size = pool->size;
    if ((wp+1)%size == rp) {
        if (msg.data)
            free(msg.data);
        pthread_mutex_unlock(&pool->mutex);
        return -1;
    } else {
        memcpy(&pool->queue[wp], &msg, sizeof(Msg));
        pool->wp = (pool->wp + 1) % size;
        pthread_mutex_unlock(&pool->mutex);
        return 0;
    }
}

void MsgBoxInit(MsgBox* box)
{
    OSEventInit(&box->mEvent);
    MsgPoolInit(&box->mMsgPool, MSG_QUEUE_SIZE);
}

void MsgBoxUnInit(MsgBox* box)
{
    OSEventUnInit(&box->mEvent);
    MsgPoolUnInit(&box->mMsgPool);
}

int MsgBoxSendMsg(MsgBox* box, Msg msg)
{
    MsgPoolPutMsg(&box->mMsgPool, msg);
    OSEventTriggle(&box->mEvent);
}

int _msgbox_wait(void* data)
{
    MsgBox* box = (MsgBox*)data;
    return !MsgPoolIsEmpty(&box->mMsgPool);
}

int MsgBoxRecvMsg(MsgBox* box, Msg* msg)
{
    int ret = 0, wp, rp, size;
    ret = OSEventWait(&box->mEvent, _msgbox_wait, (void*)box, OSEVENT_WAIT_FOREVER);
    if (ret < 0) {
        printf("%s: OSEventWait Error.\r\n", __func__);
        return -1;
    } else if (ret == 1) {
        pthread_mutex_lock(&box->mMsgPool.mutex);
        wp = box->mMsgPool.wp;
        rp = box->mMsgPool.rp;
        size = box->mMsgPool.size;
        if (wp != rp) {
            memcpy(msg, &box->mMsgPool.queue[rp], sizeof(Msg));
            rp = (rp + 1)%size;
            box->mMsgPool.rp = rp;
        }
        pthread_mutex_unlock(&box->mMsgPool.mutex);
        return 1;
    } else {
        return 0;
    }
}