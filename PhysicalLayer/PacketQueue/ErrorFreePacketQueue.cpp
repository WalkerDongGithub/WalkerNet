//
// Created by Walker on 2022/6/7.
//
#include "ErrorFreePacketQueue.h"


void ErrorFreePacketQueue::push(const Packet &t) {

    {
        std::unique_lock<std::mutex> ul(m);
        if ((back + 1) % QUEUE_ARRAY_SIZE != (front % QUEUE_ARRAY_SIZE)) {
            q[back] = t;
            back = (back + 1) % QUEUE_ARRAY_SIZE;
        } else {
            printf("packet loss\n");
        }
    } cv.notify_one();
}

Packet ErrorFreePacketQueue::pop() {
    Packet res;
    std::unique_lock<std::mutex> ul(m);
    cv.wait(ul, [&]{ return back != front;});
    res = q[front];
    res.index++;
    front = (front + 1) % QUEUE_ARRAY_SIZE;
    return res;
}