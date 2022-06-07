//
// Created by Walker on 2022/6/7.
//
#include "UtopiaPacketQueue.h"


void UtopiaPacketQueue::push(const Packet &t) {
    {
        std::unique_lock<std::mutex> ul(m);
        q.push(t);
    } cv.notify_one();
}

Packet UtopiaPacketQueue::pop() {
    Packet res;
    std::unique_lock<std::mutex> ul(m);
    cv.wait(ul, [&]{ return !q.empty();});
    res = q.front();
    res.index++;
    q.pop();
    return res;
}