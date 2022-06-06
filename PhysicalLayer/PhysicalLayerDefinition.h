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
const int PACKET_SIZE = 1700;

using InterfacesInfo = std::vector<std::pair<std::string, std::string>>;

#endif //WALKERNET_PHYSICALLAYERDEFINITION_H
