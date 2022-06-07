//
// Created by Walker on 2022/6/4.
//

#ifndef WALKERNET_PHYSICALLAYERDEFINITION_H
#define WALKERNET_PHYSICALLAYERDEFINITION_H

#include <iostream>
#include <cstring>
#include <memory>
#include <linux/ip.h>
#include <arpa/inet.h>
#include <net/ethernet.h>
#include <cinttypes>
#include <cmath>
#include <queue>
#include <mutex>
#include <thread>
#include <glog/logging.h>
#include <pcap/pcap.h>
#include <ifaddrs.h>
#include <linux/if_packet.h>
#include <utility>
#include <vector>
#include <map>
#include <unordered_map>
#include <condition_variable>
#include <fstream>

using IpAddress  = in_addr_t;
using MacAddress = ether_addr;
using PcapHandle = pcap_t*;
using InterfaceAddress = ifaddrs;
using PacketInfo = pcap_pkthdr;

using byte = unsigned char;
using ConstBuffer = const byte*;
using Buffer = byte*;

/** MTU in a packet, if packet size is bigger than this number, switch will drop it.*/
const int PACKET_SIZE = 65536;

/** Queue Capacity, if you use the error free or error high queue. */
const int QUEUE_CAPACITY = 10;

/** Array size, we will use static queue, queue size is capacity + 1*/
constexpr const int QUEUE_ARRAY_SIZE = QUEUE_CAPACITY + 1;

/** If you use error-high physical layer, loss will happen, the probability is 1/(ERROR_PROBABILITY)*/
const int ERROR_PROBABILITY = 1000;

using InterfacesInfo = std::vector<std::pair<std::string, std::string>>;

const int UTOPIA = 1;
const int ERROR_FREE = 2;
const int ERROR_HIGH = 3;
#endif //WALKERNET_PHYSICALLAYERDEFINITION_H
