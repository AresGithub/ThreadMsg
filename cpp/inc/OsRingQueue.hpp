#ifndef _OS_RING_QUEUE_HPP_
#define _OS_RING_QUEUE_HPP_

#include <mutex>
#include <thread>
#include <assert.h>

#define OS_RING_QUEUE_SIZE    256

template<typename T>

class OsRingQueue {

private:
    T *queue;
    std::mutex mMutex;
    int mSize;
    int mRp;
    int mWp;

public:
    // 构造函数
    OsRingQueue(int size = OS_RING_QUEUE_SIZE) {
        queue = new T[size];
        assert(queue!=NULL);
        mSize = size;
        mWp = mRp = 0;
    }

    // 拷贝构造函数

    // 重载赋值运算符

    // 析构函数
    ~OsRingQueue()
    {
        delete[] queue;
    }

    bool IsEmpty() 
    {
        bool ret = false;
        std::unique_lock<std::mutex> lck(mMutex);
        ret = (mWp==mRp)?true:false;
        return ret;
    }

    bool Put(T e)
    {
        int wp, rp, size;
        std::unique_lock<std::mutex> lck(mMutex);
        wp = mWp;
        rp = mRp;
        size = mSize;
        if ((wp+1)%size == rp) 
            return false;
        else {
            *(queue+wp) = e;
            wp = (wp + 1) % size;
            mWp = wp;
            return true;
        }
    }

    bool Get(T &e)
    {
        int wp, rp, size;
        std::unique_lock<std::mutex> lck(mMutex);
        wp = mWp;
        rp = mRp;
        size = mSize;
        if (wp == rp) 
            return false;
        else {
            e = *(queue + rp);
            rp = (rp + 1) % size;
            mRp = rp;
            return true;
        }
    }
};

#endif