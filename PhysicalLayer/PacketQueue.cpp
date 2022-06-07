//
// Created by Walker on 2022/6/4.
//
#include "PacketQueue.h"

static void init(Packet* packet, ConstBuffer buf, int len, int index) {
    memcpy(packet->content, buf, len);
    packet->len = len;
    packet->index = index;
}

Packet::Packet(ConstBuffer buf, int len, int index) {
    init(this, buf, len, index);
}

Packet::Packet(const Packet &packet) {
    init(this, packet.content, packet.len, packet.index);
}

Packet &Packet::operator=(const Packet &packet) {
    init(this, packet.content, packet.len, packet.index);
    return *this;
}

#include "PacketQueue/UtopiaPacketQueue.h"
#include "PacketQueue/ErrorFreePacketQueue.h"
#include "PacketQueue/ErrorHighPacketQueue.h"

UtopiaPacketQueue utopiaPacketQueue;
ErrorFreePacketQueue errorFreePacketQueue;
ErrorHighPacketQueue errorHighPacketQueue;

PacketQueue& PacketQueue::GetPacketQueue(int type) {
    switch (type) {
        case UTOPIA:
            return utopiaPacketQueue;
        case ERROR_FREE:
            return errorFreePacketQueue;
        case ERROR_HIGH:
            return errorHighPacketQueue;
        default:
            return utopiaPacketQueue;
    }
}