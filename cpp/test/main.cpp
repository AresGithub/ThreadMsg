#include <iostream>
#include <thread>
#include <unistd.h>
#include "MsgBox.hpp"

using namespace std;

MsgBox mMsgBox;

void TestThread_Routine(int arg)
{
    while (1) {
        Msg msg;
        mMsgBox.RecvMsg(msg);
    }
}

void TestThread_SendMsg_Routine(int arg)
{
    int value = arg;
    while (1) {
        Msg msg;
        msg.value = value++;
        mMsgBox.SendMsg(msg);
        // sleep(1);
        usleep(10*1000);
    }
}

int main(int argc, char* argv[])
{
    int cmd = 0;
    int tmp_value = 1;
    std::thread mThread_Recv;
    std::thread mThread_Sends[3];
    mThread_Recv = std::thread(TestThread_Routine, 0);
    mThread_Sends[0] = std::thread(TestThread_SendMsg_Routine, 0);
    mThread_Sends[1] = std::thread(TestThread_SendMsg_Routine, 100);
    mThread_Sends[2] = std::thread(TestThread_SendMsg_Routine, 200);
    mThread_Recv.detach();
    mThread_Sends[0].detach();
    mThread_Sends[1].detach();
    mThread_Sends[2].detach();

    while (1) {
        std::cout << "Input command :";
        cmd = 0;
        std::cin >> cmd;
        switch (cmd) {
            case 1:
                Msg msg;
                msg.value = tmp_value++;
                mMsgBox.SendMsg(msg);
                break;
        }
    }
    return 0;
}