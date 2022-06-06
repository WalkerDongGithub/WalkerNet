# Physical Layer

Physical Layer simulates the behaviour of the network devices.

This module uses pcap to rx/tx the packets.

No main function in this module, if you want to test this module, you need enter Test directory and create a main function.
Create a Physical Layer class, finally run 'test' function.

## How to use:

Physical Layer receives InterfacesInfo as the only parameter.
The parameter is created by Mapper, you just need to write json (now I want set all config via json) so that you can tell the physical layer which interfaces it should listen.
```C++
int main() {
    Mapper& mapper = Mapper::CreateMapper();
    PhysicalLayer(mapper.GetInterfacesInfo("/home/walker/WalkerNet/Test/test.json")).Test();
}
```
The json configuration just like :
```json
{
  "interfaces": [
    {
      "name": "eth0",
      "ip": "172.31.169.217"
    }
  ]
}
```
You can just write the interface name, and ip address, then the physical layer will look up the interface's mac address automatically.

What interface it can listen? 'ifconfig' on linux you will see.

```shell
eth0: flags=4163<UP,BROADCAST,RUNNING,MULTICAST>  mtu 1500
        inet 172.31.169.217  netmask 255.255.240.0  broadcast 172.31.175.255
        inet6 fe80::215:5dff:feaa:d06c  prefixlen 64  scopeid 0x20<link>
        ether 00:15:5d:aa:d0:6c  txqueuelen 1000  (Ethernet)
        RX packets 80465  bytes 95254834 (95.2 MB)
        RX errors 0  dropped 0  overruns 0  frame 0
        TX packets 29170  bytes 2524875 (2.5 MB)
        TX errors 0  dropped 0 overruns 0  carrier 0  collisions 0

lo: flags=73<UP,LOOPBACK,RUNNING>  mtu 65536
        inet 127.0.0.1  netmask 255.0.0.0
        inet6 ::1  prefixlen 128  scopeid 0x10<host>
        loop  txqueuelen 1000  (Local Loopback)
        RX packets 15919  bytes 3470092 (3.4 MB)
        RX errors 0  dropped 0  overruns 0  frame 0
        TX packets 15919  bytes 3470092 (3.4 MB)
        TX errors 0  dropped 0 overruns 0  carrier 0  collisions 0

```
When Physical Layer is created, it begins receiving the packets.

You just call function 'GetInterfacesInfo()' so you can get a packet that the physical layer received.

Test() function simulate the packet-catching procedure simply. GetInterfacesInfo() return a Packet which contains length, content, and index number.
By the way, the index is increased by 1, zero is used to represent the null interface.
Therefore, if you want to get the real interface, you can get 'interfaces[index - 1]'
```C++
void PhysicalLayer::Test() {
    for (;;) {
        Packet packet = GetInterfacesInfo();
        printf("length : %d ", packet.len);;
        printf("name : %s ", interfaces[packet.index - 1].name.c_str());
        printf("index : %d ", packet.index);
        printf("head : ");
        for (int j = 0; j < 14 + 20; j++) {
            printf("%02x ", packet.content[j]);
        } printf("\n");
    }
}
```

