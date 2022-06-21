#ifndef _MSG_BOX_HPP_
#define _MSG_BOX_HPP_

#include "OsEvent.hpp"
#include "OsRingQueue.hpp"
#include <string.h>

class Msg {
public:
    Msg (int val = 0, unsigned char *dat = NULL, size_t size = 0) {
        if (size && dat) {
            data = new unsigned char[size+1];
            assert(data!=NULL);
            memcpy(data, dat, size);
            data[size] = 0;
        }
        value = val;
    }

    ~Msg() {
        if (data) {
            delete[] data;
        }
    }

    int value = 0;
    unsigned char* data = NULL;
}; 

class MsgBox {
public:
    MsgBox() = default;
    ~MsgBox() = default;
    static int _msgbox_cond_func(void* data);
    int SendMsg(Msg msg);
    int RecvMsg(Msg& msg);
    bool MsgQueueIsEmpty();
private:
    OsEvent mOsEvt;
    OsRingQueue<Msg> mMsgQueue;
};


#endif