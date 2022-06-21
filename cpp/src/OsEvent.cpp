#include "OsEvent.hpp"

void OsEvent::notify()
{
    // std::unique_lock<std::mutex> lck(mtx); // 其实没必要在此处加锁 
    // cv.notify_all();
    cv.notify_one();
}

int OsEvent::wait(PFUN_COND pCondFun, void* data, unsigned int timeout_ms)
{
    int ret = 0;
    if (pCondFun == NULL) {
        ret = -2;
        return ret;
    }
    std::unique_lock<std::mutex> lck(mtx);
    if (timeout_ms == OSEVENT_WAIT_FOREVER) {
        while (!(ret = pCondFun(data))) {
            cv.wait(lck);
        }
        return ret;
    } else if (timeout_ms == OSEVENT_WAIT_NONE) {
        ret = pCondFun(data);
        return ret;
    } else {
        while (!(ret = pCondFun(data))) {
            auto r = cv.wait_for(lck, std::chrono::milliseconds(timeout_ms));
            if (r == std::cv_status::timeout) {
                ret = -1;
                return ret;
            }
        }
        return ret;
    }
}