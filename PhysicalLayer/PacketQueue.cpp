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

void PacketQueue::push(const Packet &t) {
    {
        std::unique_lock<std::mutex> ul(m);
        q.push(t);
    } cv.notify_one();
}

Packet PacketQueue::pop() {
    Packet res;
    std::unique_lock<std::mutex> ul(m);
    cv.wait(ul, [&]{ return !q.empty();});
    res = q.front();
    res.index++;
    q.pop();
    return res;
}
