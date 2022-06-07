//
// Created by Walker on 2022/6/7.
//

#include "ErrorHighPacketQueue.h"
#include <random>
#include <ctime>

std::default_random_engine dre(time(nullptr));
std::uniform_int_distribution<int> uid(1, ERROR_PROBABILITY);

void ErrorHighPacketQueue::push(const Packet &t) {

    {
        std::unique_lock<std::mutex> ul(m);
        if ((back + 1) % QUEUE_ARRAY_SIZE != (front % QUEUE_ARRAY_SIZE)) {
            if (uid(dre) != 1) {
                q[back] = t;
                back = (back + 1) % QUEUE_ARRAY_SIZE;
            }
        }
    } cv.notify_one();
}

Packet ErrorHighPacketQueue::pop() {
    Packet res;
    std::unique_lock<std::mutex> ul(m);
    cv.wait(ul, [&]{ return back != front;});
    res = q[front];
    res.index++;
    front++;
    return res;
}