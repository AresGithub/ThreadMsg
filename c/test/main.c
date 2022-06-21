#include <stdio.h>
#include "OsMsg.h"

MsgBox mMsgBox1;
MsgBox mMsgBox2;

void* _1_waitformsg_threadroutine(void* arg)
{
    int ret = 0;
    Msg msg;
    while (1) {
        ret = MsgBoxRecvMsg(&mMsgBox1, &msg);
        if (ret == 1) {
            printf("RecvMessage[mMsgBox1]: value = %d. \r\n", msg.value);
        }
    }
}

void* _2_waitformsg_threadroutine(void* arg)
{
    int ret = 0;
    Msg msg;
    while (1) {
        ret = MsgBoxRecvMsg(&mMsgBox2, &msg);
        if (ret == 1) {
            printf("RecvMessage[mMsgBox2]: value = %d. \r\n", msg.value);
        }
    }
}

int main(int argc, char* argv[])
{
    int cmd = 0;
    int i = 0;
    int value = 1;
    Msg msg;
    pthread_t tid;
    MsgBoxInit(&mMsgBox1);
    pthread_create(&tid, NULL, _1_waitformsg_threadroutine, NULL);
    MsgBoxInit(&mMsgBox2);
    pthread_create(&tid, NULL, _2_waitformsg_threadroutine, NULL);

    while (1) {
        printf("input command:");
        cmd = 0;
        scanf("%d", &cmd);
        switch (cmd) {
            case 1:
                for (i = 0; i < 10; i++) {
                    msg.value = value++;
                    msg.data = NULL;
                    MsgBoxSendMsg(&mMsgBox1, msg);
                    MsgBoxSendMsg(&mMsgBox2, msg);
                }
                break;
        }
    }
    
    return 0;
}