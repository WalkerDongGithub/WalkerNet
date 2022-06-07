//
// Created by Walker on 2022/6/7.
//

#ifndef WALKERNET_UTOPIAPACKETQUEUE_H
#define WALKERNET_UTOPIAPACKETQUEUE_H

#include "../PacketQueue.h"
/**
 * @Date: 2022/06/07
 * @Author: Walker
 *
 *          Utopia queue, error free and no packet-dropping.
 *          use stl queue to receive the packets.
 */
class UtopiaPacketQueue : public PacketQueue {
private:
    std::queue<Packet> q;
    std::mutex m;
    std::condition_variable cv;
public:
    void push(const Packet& t) override;
    Packet pop() override;
};

#endif //WALKERNET_UTOPIAPACKETQUEUE_H
