//
// Created by xilinx_0 on 1/7/22.
//

#ifndef WALKERNET_PHYSICAL_LAYER_H
#define WALKERNET_PHYSICAL_LAYER_H

#include "PhysicalLayerDefinition.h"
#include "PacketQueue.h"


/**
 * @Date: 2022/06/04
 * @Author: Walker
 * @Desc:
 *
 *          Physical Layer simulate the rx/tx behaviour of a switch.
 */


class PhysicalLayer {


    /**
     *          Input the ifconfig, you can see all of the available interfaces,
     *          Interface contains its name, ip, mac.
     *          We use pcap handle to operate this interface, such as open, recv, send, etc.
     *          We create a thread for listening this interface called receive thread.
     */
    struct Interface {

        std::string          name;
        IpAddress       ipAddress;
        MacAddress     macAddress;
        PcapHandle     pcapHandle;
        std::thread receiveThread;
        Interface(std::string name, IpAddress ip, MacAddress mac);

    };
    std::vector<Interface> interfaces;




    /**
     *          packets queue, when an interface receives a packet, it will push it
     *          to the packet queue, when the switch is ready to process a packet,
     *          it just get one away from the front of the packet queue.
     */
    PacketQueue packets;



    /* number of the interfaces */
    inline int NumInterface() const;

public:

    /**
     *
     *          for the unified interfaces,
     *          constructor only receives vector<pair<string, string>> as parameter.
     *          parameter interfacesInfo is just like [{"name": "lo", "ip": "127.0.0.1"}] (for json).
     *
     * */
    PhysicalLayer(const InterfacesInfo& interfacesInfo);


    /**
     *      function GetInterfacesInfo() means pop one packet from packet queue when packet queue is not empty.
     */
    inline Packet Get();


    /**
     *      Send a packet, this packet contains the will-send interface, length, and the content.
     */
    inline int Send(const Packet& packet);
private:


    /**
     *      The Begin to receive function is the receive thread's task.
     */
    static void BeginReceive(PhysicalLayer& physicalLayer, int index);
public:
    void Test();
};
#endif
