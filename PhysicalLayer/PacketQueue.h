//
// Created by Walker on 2022/6/4.
//

#ifndef WALKERNET_PACKET_H
#define WALKERNET_PACKET_H

#include "PhysicalLayerDefinition.h"

/**
 * @Date: 2022/06/04
 * @Author: Walker
 *
 *          Packet represent a packet that received from an interface.
 *
 */
struct Packet {
    byte content[PACKET_SIZE];
    int len;
    int index;
    Packet(ConstBuffer buf, int len, int index);
    Packet(const Packet& packet);
    Packet() {}
    Packet& operator=(const Packet& packet);
};


/**
 * @Date: 2022/06/04
 * @Author: Walker
 *
 *          It's a thread-safe queue, in order to make sure that
 *          every interface will push the packets into this queue without conflict.
 *          If a thread is in ready state to process packets, just pop a packet.
 *          Every index of packets pop out of the queue will increase 1, so that zero
 *          can be used to represent the drop-mark.
 *
 */
struct PacketQueue {
private:
    std::queue<Packet> q;
    std::mutex m;
    std::condition_variable cv;
public:
    void push(const Packet& t);
    Packet pop();
};


#endif //WALKERNET_PACKET_H
