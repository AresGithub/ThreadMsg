#include "MsgBox.hpp"
#include <iostream>

using namespace std;

int MsgBox::_msgbox_cond_func(void* data)
{
    MsgBox* pMsgBox = (MsgBox*)data;
    return !pMsgBox->MsgQueueIsEmpty();
}

bool MsgBox::MsgQueueIsEmpty()
{
    return mMsgQueue.IsEmpty();
}

int MsgBox::SendMsg(Msg msg)
{
    mMsgQueue.Put(msg);
    mOsEvt.notify();
    return 0;
}

int MsgBox::RecvMsg(Msg& msg)
{
    int ret = mOsEvt.wait(_msgbox_cond_func, (void*)this, OSEVENT_WAIT_FOREVER);
    if (ret < 0) {
        std::cout << __func__ << ": OsEvent.wait error, ret = " << ret << std::endl;
        return ret;
    } else {
        Msg msg;
        if (mMsgQueue.Get(msg)) {
            std::cout << __func__ << ": Get msg.value = " << msg.value << std::endl;
        } else {
            std::cout << __func__ << ": Get msg failed." << std::endl;
        }
    }
    return 0;
}