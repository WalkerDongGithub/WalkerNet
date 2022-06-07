//
// Created by Walker on 2022/6/7.
//

#ifndef PHYSICALLAYER_ERRORHIGHPACKETQUEUE_H
#define PHYSICALLAYER_ERRORHIGHPACKETQUEUE_H

#include "../PacketQueue.h"
/**
 * @Date: 2022/06/07
 * @Author: Walker
 *
 *          Error high queue, may cause packet loss.
 */
class ErrorHighPacketQueue : public PacketQueue {
private:
    Packet q[QUEUE_ARRAY_SIZE];
    int front = 0;
    int back = 0;
    std::mutex m;
    std::condition_variable cv;
public:
    void push(const Packet& t) override;
    Packet pop() override;
};
#endif //PHYSICALLAYER_ERRORHIGHPACKETQUEUE_H
