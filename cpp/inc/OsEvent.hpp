#ifndef _OS_EVENT_HPP_
#define _OS_EVENT_HPP_

#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>

#define OSEVENT_WAIT_NONE       0x00000000
#define OSEVENT_WAIT_FOREVER    0xFFFFFFFF

typedef int (*PFUN_COND)(void* );

class OsEvent {
public:
    OsEvent() = default;
    ~OsEvent() = default;
    void notify();
    int wait(PFUN_COND pCondFun, void* data, unsigned int timeout_ms);

private:
    std::mutex mtx;
    std::condition_variable cv;
};

#endif