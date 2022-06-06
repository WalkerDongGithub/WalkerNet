//
// Created by Walker on 2022/6/4.
//
#include "PhysicalLayer.h"
using std::cout;
void PhysicalLayer::Test() {
    for (;;) {
        Packet packet = Get();
        printf("length : %d ", packet.len);;
        printf("name : %s ", interfaces[packet.index - 1].name.c_str());
        printf("index : %d ", packet.index);
        printf("head : ");
        for (int j = 0; j < 14 + 20; j++) {
            printf("%02x ", packet.content[j]);
        } printf("\n");
    }
}

void PhysicalLayer::BeginReceive(PhysicalLayer &physicalLayer, int index) {

    for(;;) {
        PacketInfo hdr;
        ConstBuffer buffer = pcap_next(physicalLayer.interfaces[index].pcapHandle, &hdr);
        if (buffer && hdr.caplen < PACKET_SIZE) {
            Packet packet = Packet(buffer, hdr.caplen, index);
            physicalLayer.packets.push(packet);
        }
    }
}


/**
 * Initialize the physical layer, such as the interfaces' information.
 * and create the receive thread.
 * @param interfacesInfo
 */
PhysicalLayer::PhysicalLayer(const InterfacesInfo &interfacesInfo) {

    InterfaceAddress *ifaddr;
    if (getifaddrs(&ifaddr) < 0) {
        throw std::runtime_error("Cannot get mac address from interface");
    }

    for (const auto & i : interfacesInfo) {
        std::string if_name = i.first;
        std::string if_ip   = i.second;

        IpAddress ip;
        inet_aton(if_ip.c_str(), (in_addr *) &ip);

        for (InterfaceAddress *ifa = ifaddr; ifa ; ifa = ifa->ifa_next) {

            if (ifa->ifa_addr == nullptr) continue;
            if (ifa->ifa_addr->sa_family == AF_PACKET
                && ifa->ifa_name == if_name) {
                MacAddress mac;
                memcpy(&mac, ((sockaddr_ll *) ifa->ifa_addr)->sll_addr, sizeof(ether_addr));
                interfaces.emplace_back(if_name, ip, mac);
                interfaces[interfaces.size() - 1].receiveThread =
                        std::thread(PhysicalLayer::BeginReceive, std::ref(*this), interfaces.size() - 1);
                interfaces[interfaces.size() - 1].receiveThread.detach();
            }
        }
    }

    if (NumInterface() == 0) {
        throw std::runtime_error("no interface get.");
    }

}

int PhysicalLayer::Send(const Packet& packet) {
    Packet sendPacket = packet;
    sendPacket.index --;
    return pcap_inject(interfaces[sendPacket.index].pcapHandle, sendPacket.content, sendPacket.len);
}

Packet PhysicalLayer::Get() {
    return packets.pop();
}

int PhysicalLayer::NumInterface() const {
    return interfaces.size();
}

/**
 *              set an interface's name, ip, mac, pcap handle,
 *              and set the interface nonblock to false.
 */
PhysicalLayer::Interface::Interface(std::string name, IpAddress ip, MacAddress mac) {
    this->name = name;
    this->ipAddress = ip;
    this->macAddress = mac;
    char error_buffer[256];
    pcapHandle = pcap_open_live(name.c_str(), BUFSIZ, 1, 1, error_buffer);
    if (!pcapHandle) {
        throw std::runtime_error("interface " + name + " not found or permission denied.");
    }
    pcap_setnonblock(pcapHandle, 0, error_buffer);
}
